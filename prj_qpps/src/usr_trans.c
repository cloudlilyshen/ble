/****************************************Copyright (c)****************************************************
**                               Guangzhou ZLG MCU Technology Co., Ltd.
**
**                                     http://www.zlgmcu.com
**
**      广州周立功单片机科技有限公司所提供的所有服务内容旨在协助客户加速产品的研发进度，在服务过程中所提供
**  的任何程序、文档、测试结果、方案、支持等资料和信息，都仅供参考，客户有权不使用或自行参考修改，本公司不
**  提供任何的完整性、可靠性等保证，若在客户使用过程中因任何原因造成的特别的、偶然的或间接的损失，本公司不
**  承担任何责任。
**                                                                        ——广州周立功单片机科技有限公司
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Main.c
** Last modified date:      2015-09-04
** Last version:            V1.0
** Description:             
**
**--------------------------------------------------------------------------------------------------------
** Created by:              
** Created date:            2015-09-04
** Version:                 V1.0
** Descriptions:            本文件主要实现将串口接收到的数据存放到串口缓冲区中，然后再将数据通过QPP协议
**                          发送出去，串口缓冲区以环形队列的方式进行存取数据。
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
*********************************************************************************************************/

#include <usr_trans.h>

/*
 * 定义串口变量
 */

volatile uint8_t  __GucBLE_Connected = 0;
volatile uint16_t __GusUartBufhead   = 0;                               /* 缓冲区存入数据个数            */
volatile uint16_t __GusUartBufwear   = 0;                               /* 缓冲区读取数据个数            */
volatile uint8_t  __GucUartBuf[UART_BUF_MAX_LEN];                       /* 缓冲区数组                    */

/*
 * 定义串口缓存变量
 */
volatile uint8_t  __GucBLE_GetDataTemp[4];

/*
 * 定义串口接收处理发送数据变量
 */
volatile uint8_t   __GucUartDealFlag = 0;
volatile uint16_t  __GusUartTimeout  = APP_UART_RXTIMEOUT;
volatile uint8_t   __GucRFSendBuf[APP_SEND_PACKET_BYTES];
volatile uint16_t  __GusRFSendBufLen = 0;
volatile uint8_t   __GucRFSendFlag   = 0;

/*
 * 定义睡眠变量
 */
volatile uint8_t   __GucSleepState   = 2;

/*********************************************************************************************************
** Function name:           APPSetBLEConnect
** Descriptions:            BLE连接状态标志位置位
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void APPSetBLEConnect(void)
{
    __GucBLE_Connected = 1;

}

/*********************************************************************************************************
** Function name:           APPSetBLEDisConnect
** Descriptions:            清除BLE连接状态标志位
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void APPSetBLEDisConnect(void)
{
    __GucBLE_Connected = 0;
    
}
/*********************************************************************************************************
** Function name:           bAPPGetBLEConnectState
** Descriptions:            获取BLE连接状态标志位
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
bool bAPPGetBLEConnectState(void)
{
    return __GucBLE_Connected > 0 ? true : false;
}
/*********************************************************************************************************
** Function name:           APPBLE_Sleep
** Descriptions:            设置用户应用程序的功耗模式为PM_SLEEP
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void APPBLE_Sleep(void)
{
    sleep_set_pm(PM_SLEEP);                                            /* 设置用户应用程序的功耗模式   */
}

/*********************************************************************************************************
** Function name:           APPBLE_Active
** Descriptions:            设置用户应用程序的功耗模式为PM_ACTIVE
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void APPBLE_Active(void)
{
    sleep_set_pm(PM_ACTIVE);                                            /* 设置用户应用程序的功耗模式   */
}

/*********************************************************************************************************
** Function name:           APPBLE_UartInit
** Descriptions:            串口初始化，串口接收为中断方式，发送为查询方式
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
MOTOR_OPEN** Modified date:
*********************************************************************************************************/
void APPBLE_UartInit(void)
{
    uart_init(APP_UART, USARTx_CLK(0), APP_UART_BAUDE);
    uart_tx_enable(APP_UART, MASK_ENABLE);                              /* 串口发送使能                 */
    uart_rx_enable(APP_UART, MASK_ENABLE);                              /* 串口接收使能                 */
    uart_rx_int_enable(APP_UART, MASK_ENABLE);                          /* 串口接收中断使能             */
}

/*********************************************************************************************************
** Function name:           APPUartDealData
** Descriptions:            通过BLE发送uart接收到的数据
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void APPUartDealData(void)
{
    /*
     *  检查BLE状态，若未连接，则丢弃所有数据
     */
    if (bAPPGetBLEConnectState() == FALSE) {
        __GusUartBufwear = 0;
        __GusUartBufhead = 0;
        return;
    }

    if (__GucRFSendFlag) {                                  
        return;                                                         /* 数据还没有发送完成，返回     */
    }

    /*
     *  开始发送数据时启动接收超时检测
     */
    if (__GucUartDealFlag == 0  && (__GusRFSendBufLen != 0)) {        
        __GucUartDealFlag = 1;                                          /* 置位uart数据处理标志位       */
        ke_timer_set(APP_SYS_UARTRX_TIMER, TASK_APP, 
           __GusUartTimeout);                                           /* 设置uart接收超时消息         */
    }    
    
    
    while (__GusUartBufwear != __GusUartBufhead) {                      /* 判断串口缓冲区是否有未读数据 */
        
        /*
         *  判断是否读取到缓冲区数组的最后一个值，
         */
        if ((__GusUartBufwear + 1) >= UART_BUF_MAX_LEN) {               /* 读取数据为缓冲区数组最后一个 */
            __GusUartBufwear = 0;                                       /* 将读取个数清0                */
        }
        else {
            __GusUartBufwear++;                                         /* 读取数据个数加1              */
        }
        

       
        /*
         *  当BLE缓存中存入APP_SEND_PACKET_BYTES个数据后BLE开始发送数据，这样避免了每次uart接收到数据就
         *  进行一次BLE发送，大大增加了发送的效率。如果uart长时间接收不到APP_SEND_PACKET_BYTES个数据，
         *  便会发送uart接收超时消息，在uart接收超时处理函数中BLE也会发送数据，保证uart接收到的数据全
         *  部发送
         */
        if (__GusRFSendBufLen >= APP_SEND_PACKET_BYTES) {               
            app_BLE_sendUart_data(1, (uint8_t *)__GucRFSendBuf,
                                  __GusRFSendBufLen);                   /* 启动BLE发送数据              */
            ke_timer_set(APP_SYS_SENDDATA_TIMER, TASK_APP, 
                         APP_RFSEND_TIMEOUT);                           /* 设置BLE发送超时消息          */
//            __GucRFSendFlag   = 1;                                      /* BLE发送数据标志位置1         */
            __GusRFSendBufLen = 0;                                      /* BLE发送缓冲区数据长度清零    */
            __GucUartDealFlag = 0;                                      /* 清除uart数据处理标志位       */
            __GucRFSendBuf[__GusRFSendBufLen++] = __GucUartBuf[__GusUartBufwear];
            ke_timer_clear(APP_SYS_UARTRX_TIMER, TASK_APP);             /* 清除uart接收超时消息         */
            return;
        }
        else {
            __GucRFSendBuf[__GusRFSendBufLen++] = 
                            __GucUartBuf[__GusUartBufwear];             /* 将数据存入BLE发送缓存中      */
        }
    }
}

/*********************************************************************************************************
** Function name:           app_SendData_timer_handler
** Descriptions:            BLE发送数据超时处理
** input parameters:        msgid：APP_SYS_SENDDATA_TIMER      param：none      
**                          dest_id：TASK_APP                  src_id：TASK_APP
** output parameters:       none
** Returned value:          消息是否被执行
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
int app_SendData_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    switch(msgid)
    {
        case APP_SYS_SENDDATA_TIMER:
             ke_timer_clear(APP_SYS_SENDDATA_TIMER, TASK_APP);          /* 清除BLE发送超时消息          */
             __GucRFSendFlag = 0;                                       /* 清除BLE发送标志位            */
            break;

        default:
            ASSERT_ERR(0);
            break;
    }

    return (KE_MSG_CONSUMED);
}

/*********************************************************************************************************
** Function name:           app_UartRXTimeOut_timer_handler
** Descriptions:            串口接收数据超时处理函数
** input parameters:        msgid：APP_SYS_UARTRX_TIMER        param：none
**                          dest_id：TASK_APP                  src_id：TASK_APP
** output parameters:       none
** Returned value:          消息是否被执行
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
int app_UartRXTimeOut_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    switch(msgid)
    {
        case APP_SYS_UARTRX_TIMER:
            if (__GucRFSendFlag == 0) {
                app_BLE_sendUart_data(2, (uint8_t *)__GucRFSendBuf,
                                      __GusRFSendBufLen);               /* BLE发送数据                  */
                ke_timer_set(APP_SYS_SENDDATA_TIMER, TASK_APP, 
                             APP_RFSEND_TIMEOUT);                       /* 设置BLE发送超时消息          */ 
                __GusRFSendBufLen = 0;                                  /* 待发送数据长度清零           */
                __GucUartDealFlag = 0;                                  /* 清除uart数据处理标志位       */
                ke_timer_clear(APP_SYS_UARTRX_TIMER, TASK_APP);         /* 清除uart接收超时消息         */
            }
            else {
                ke_timer_set(APP_SYS_UARTRX_TIMER, TASK_APP, __GusUartTimeout);
            }
            break;

        default:
            ASSERT_ERR(0);
            break;
    }

    return (KE_MSG_CONSUMED);
}

/*********************************************************************************************************
** Function name:           app_BLE_sendUart_data
** Descriptions:            BLE发送串口数据
** input parameters:        index：发送的特性序号    s：待发送数据的指针
**                          len：发送数据的长度
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void app_BLE_sendUart_data(uint8_t index, uint8_t* s, uint16_t len)
{
    app_qpps_data_send(app_qpps_env->conhdl, index, len, s);
}

/*********************************************************************************************************
** Function name:           UART0_RX_IRQHandler
** Descriptions:            串口0接收中断处理函数
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
#if (CONFIG_ENABLE_DRIVER_UART0==TRUE && CONFIG_UART0_RX_DEFAULT_IRQHANDLER==FALSE)
#if UART_RX_DMA_EN==FALSE

void UART0_RX_IRQHandler(void)
{
    uint32_t reg;

    reg = uart_uart_GetIntFlag(QN_UART0);
    if ( reg & UART_MASK_BE_IF) {  
        uart_uart_ClrIntFlag(QN_UART0, UART_MASK_BE_IF);
    }
    else if ( reg & UART_MASK_PE_IF ) {                                 /* 奇偶校验错误中断             */
        uart_uart_ClrIntFlag(QN_UART0, UART_MASK_PE_IF);                /* 清除中断标志                 */
    }
    else if ( reg & UART_MASK_FE_IF ) {                                 /* 帧错误中断                   */
        uart_uart_ClrIntFlag(QN_UART0, UART_MASK_FE_IF);                /* 清除中断标志                 */
    } 
    else if ( reg & UART_MASK_OE_IF ) {                                 /* 溢出错误标志                 */
        uart_uart_ClrIntFlag(QN_UART0, UART_MASK_OE_IF);                /* 清除中断标志                 */
    }
    else if ( reg & UART_MASK_RX_IF ) {                                 /* 接收FIFO非空中断             */
        __GucBLE_GetDataTemp[0] = uart_uart_GetRXD(QN_UART0);           /* 读取数据                     */

        if ((__GusUartBufhead + 1) >= UART_BUF_MAX_LEN) {               /* 写入数据个数达缓冲区最大数   */
            if (__GusUartBufwear == 0) {
                return;                                                 /* 缓冲区满了，丢弃数据，返回   */
            } else {
                
             /*
              *  从缓冲区数据头开始存入数据
              */
             __GusUartBufhead = 0;                                   
             __GucUartBuf[__GusUartBufhead] = __GucBLE_GetDataTemp[0];
            }
        }
        else {                                                          /* 写入数据个数未达缓冲区最大数 */
            if ((__GusUartBufhead + 1) == __GusUartBufwear) {
                return;                                                 /* 缓冲区满了，丢弃数据，返回   */
            } else {

            /*
             *  接着向缓冲区存入数据 
             */
            __GusUartBufhead++;
            __GucUartBuf[__GusUartBufhead] = __GucBLE_GetDataTemp[0];
            }
        }    
    }
}

#endif
#endif /* CONFIG_UART0_RX_DEFAULT_IRQHANDLER==FALSE */

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/

