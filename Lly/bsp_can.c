#include "bsp_can.h"
FDCAN_TxHeaderTypeDef fdcan1_TxHeader;
FDCAN_RxHeaderTypeDef FDCAN1_RXFilter;

/**
  * 函数功能: 设置并初始化滤波器0,供FDCAN1使用
  * 输入参数: void
  * 返回值：  void
  */
void FDCAN1_RX_Filter_Init(void)
{
    FDCAN_FilterTypeDef hfdcan1_RX_Filter;   /* FDCAN1滤波器0对象句柄 */

    hfdcan1_RX_Filter.IdType = FDCAN_STANDARD_ID;              /* 只接收标准帧ID */
    hfdcan1_RX_Filter.FilterIndex = 0;                         /* 滤波器索引0 */
    hfdcan1_RX_Filter.FilterType = FDCAN_FILTER_RANGE;          /* 滤波器类型 */
    hfdcan1_RX_Filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;  /* 滤波器关联到RXFIFO0 */
    hfdcan1_RX_Filter.FilterID1 = 0x000;                       /* 滤波ID1: 0x00 */
    hfdcan1_RX_Filter.FilterID2 = 0x7FF; /* 滤波ID2: 0x00 */
    HAL_FDCAN_ConfigFilter(&hfdcan1,&hfdcan1_RX_Filter);       /* 看看滤波器有没有创建成功 */
    /* HAL_FDCAN_ConfigGlobalFilter()
     * 参数2：设置标准帧ID，接收的报文ID没有匹配上滤波器时，选择拒绝接收(没有匹配上时,可以选择放入FIFO0或者FIFO1)。
     * 参数3：设置拓展帧ID，接收的报文ID没有匹配上滤波器时，选择拒绝接收。
     * 参数4：设置是否拒绝远程标准帧，ENABLE代表拒绝接收。
     * 参数5：设置是否拒绝远程拓展帧，ENABLE代表拒绝接收。
     */
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,FDCAN_REJECT,FDCAN_REJECT,DISABLE,ENABLE); /* 设置FDCAN1滤波器0全局配置  */
    HAL_FDCAN_Start(&hfdcan1);
    HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0);

}

uint8_t FDCAN1_Send_Msg(uint16_t id,uint8_t* msg,uint32_t len)
{
    fdcan1_TxHeader.Identifier=id;                           //32位ID
    fdcan1_TxHeader.IdType=FDCAN_STANDARD_ID;                  //标准ID
    fdcan1_TxHeader.TxFrameType=FDCAN_DATA_FRAME;              //数据帧
    fdcan1_TxHeader.DataLength=len;                            //数据长度
    fdcan1_TxHeader.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
    fdcan1_TxHeader.BitRateSwitch=FDCAN_BRS_OFF;               //关闭速率切换
    fdcan1_TxHeader.FDFormat=FDCAN_CLASSIC_CAN;                //传统的CAN模式
    fdcan1_TxHeader.TxEventFifoControl=FDCAN_NO_TX_EVENTS;     //无发送事件
    fdcan1_TxHeader.MessageMarker=0;

    if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1,&fdcan1_TxHeader,msg)!=HAL_OK) return 1;//发送
    return 0;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    uint8_t i=0;
    uint8_t rxdata[8];
    FDCAN_RxHeaderTypeDef FDCAN1_RxHeader;
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    if((RxFifo0ITs&FDCAN_IT_RX_FIFO0_NEW_MESSAGE)!=RESET)   //FIFO0新数据中断
    {
        //提取FIFO0中接收到的数据
        HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0,&FDCAN1_RxHeader,rxdata);
    }
}