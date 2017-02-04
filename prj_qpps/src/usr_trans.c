/****************************************Copyright (c)****************************************************
**                               Guangzhou ZLG MCU Technology Co., Ltd.
**
**                                     http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
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
** Descriptions:            ���ļ���Ҫʵ�ֽ����ڽ��յ������ݴ�ŵ����ڻ������У�Ȼ���ٽ�����ͨ��QPPЭ��
**                          ���ͳ�ȥ�����ڻ������Ի��ζ��еķ�ʽ���д�ȡ���ݡ�
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
*********************************************************************************************************/

#include <usr_trans.h>

/*
 * ���崮�ڱ���
 */

volatile uint8_t  __GucBLE_Connected = 0;
volatile uint16_t __GusUartBufhead   = 0;                               /* �������������ݸ���            */
volatile uint16_t __GusUartBufwear   = 0;                               /* ��������ȡ���ݸ���            */
volatile uint8_t  __GucUartBuf[UART_BUF_MAX_LEN];                       /* ����������                    */

/*
 * ���崮�ڻ������
 */
volatile uint8_t  __GucBLE_GetDataTemp[4];

/*
 * ���崮�ڽ��մ��������ݱ���
 */
volatile uint8_t   __GucUartDealFlag = 0;
volatile uint16_t  __GusUartTimeout  = APP_UART_RXTIMEOUT;
volatile uint8_t   __GucRFSendBuf[APP_SEND_PACKET_BYTES];
volatile uint16_t  __GusRFSendBufLen = 0;
volatile uint8_t   __GucRFSendFlag   = 0;

/*
 * ����˯�߱���
 */
volatile uint8_t   __GucSleepState   = 2;

/*********************************************************************************************************
** Function name:           APPSetBLEConnect
** Descriptions:            BLE����״̬��־λ��λ
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
** Descriptions:            ���BLE����״̬��־λ
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
** Descriptions:            ��ȡBLE����״̬��־λ
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
** Descriptions:            �����û�Ӧ�ó���Ĺ���ģʽΪPM_SLEEP
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
    sleep_set_pm(PM_SLEEP);                                            /* �����û�Ӧ�ó���Ĺ���ģʽ   */
}

/*********************************************************************************************************
** Function name:           APPBLE_Active
** Descriptions:            �����û�Ӧ�ó���Ĺ���ģʽΪPM_ACTIVE
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
    sleep_set_pm(PM_ACTIVE);                                            /* �����û�Ӧ�ó���Ĺ���ģʽ   */
}

/*********************************************************************************************************
** Function name:           APPBLE_UartInit
** Descriptions:            ���ڳ�ʼ�������ڽ���Ϊ�жϷ�ʽ������Ϊ��ѯ��ʽ
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
    uart_tx_enable(APP_UART, MASK_ENABLE);                              /* ���ڷ���ʹ��                 */
    uart_rx_enable(APP_UART, MASK_ENABLE);                              /* ���ڽ���ʹ��                 */
    uart_rx_int_enable(APP_UART, MASK_ENABLE);                          /* ���ڽ����ж�ʹ��             */
}

/*********************************************************************************************************
** Function name:           APPUartDealData
** Descriptions:            ͨ��BLE����uart���յ�������
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
     *  ���BLE״̬����δ���ӣ�������������
     */
    if (bAPPGetBLEConnectState() == FALSE) {
        __GusUartBufwear = 0;
        __GusUartBufhead = 0;
        return;
    }

    if (__GucRFSendFlag) {                                  
        return;                                                         /* ���ݻ�û�з�����ɣ�����     */
    }

    /*
     *  ��ʼ��������ʱ�������ճ�ʱ���
     */
    if (__GucUartDealFlag == 0  && (__GusRFSendBufLen != 0)) {        
        __GucUartDealFlag = 1;                                          /* ��λuart���ݴ����־λ       */
        ke_timer_set(APP_SYS_UARTRX_TIMER, TASK_APP, 
           __GusUartTimeout);                                           /* ����uart���ճ�ʱ��Ϣ         */
    }    
    
    
    while (__GusUartBufwear != __GusUartBufhead) {                      /* �жϴ��ڻ������Ƿ���δ������ */
        
        /*
         *  �ж��Ƿ��ȡ����������������һ��ֵ��
         */
        if ((__GusUartBufwear + 1) >= UART_BUF_MAX_LEN) {               /* ��ȡ����Ϊ�������������һ�� */
            __GusUartBufwear = 0;                                       /* ����ȡ������0                */
        }
        else {
            __GusUartBufwear++;                                         /* ��ȡ���ݸ�����1              */
        }
        

       
        /*
         *  ��BLE�����д���APP_SEND_PACKET_BYTES�����ݺ�BLE��ʼ�������ݣ�����������ÿ��uart���յ����ݾ�
         *  ����һ��BLE���ͣ���������˷��͵�Ч�ʡ����uart��ʱ����ղ���APP_SEND_PACKET_BYTES�����ݣ�
         *  ��ᷢ��uart���ճ�ʱ��Ϣ����uart���ճ�ʱ��������BLEҲ�ᷢ�����ݣ���֤uart���յ�������ȫ
         *  ������
         */
        if (__GusRFSendBufLen >= APP_SEND_PACKET_BYTES) {               
            app_BLE_sendUart_data(1, (uint8_t *)__GucRFSendBuf,
                                  __GusRFSendBufLen);                   /* ����BLE��������              */
            ke_timer_set(APP_SYS_SENDDATA_TIMER, TASK_APP, 
                         APP_RFSEND_TIMEOUT);                           /* ����BLE���ͳ�ʱ��Ϣ          */
//            __GucRFSendFlag   = 1;                                      /* BLE�������ݱ�־λ��1         */
            __GusRFSendBufLen = 0;                                      /* BLE���ͻ��������ݳ�������    */
            __GucUartDealFlag = 0;                                      /* ���uart���ݴ����־λ       */
            __GucRFSendBuf[__GusRFSendBufLen++] = __GucUartBuf[__GusUartBufwear];
            ke_timer_clear(APP_SYS_UARTRX_TIMER, TASK_APP);             /* ���uart���ճ�ʱ��Ϣ         */
            return;
        }
        else {
            __GucRFSendBuf[__GusRFSendBufLen++] = 
                            __GucUartBuf[__GusUartBufwear];             /* �����ݴ���BLE���ͻ�����      */
        }
    }
}

/*********************************************************************************************************
** Function name:           app_SendData_timer_handler
** Descriptions:            BLE�������ݳ�ʱ����
** input parameters:        msgid��APP_SYS_SENDDATA_TIMER      param��none      
**                          dest_id��TASK_APP                  src_id��TASK_APP
** output parameters:       none
** Returned value:          ��Ϣ�Ƿ�ִ��
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
             ke_timer_clear(APP_SYS_SENDDATA_TIMER, TASK_APP);          /* ���BLE���ͳ�ʱ��Ϣ          */
             __GucRFSendFlag = 0;                                       /* ���BLE���ͱ�־λ            */
            break;

        default:
            ASSERT_ERR(0);
            break;
    }

    return (KE_MSG_CONSUMED);
}

/*********************************************************************************************************
** Function name:           app_UartRXTimeOut_timer_handler
** Descriptions:            ���ڽ������ݳ�ʱ������
** input parameters:        msgid��APP_SYS_UARTRX_TIMER        param��none
**                          dest_id��TASK_APP                  src_id��TASK_APP
** output parameters:       none
** Returned value:          ��Ϣ�Ƿ�ִ��
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
                                      __GusRFSendBufLen);               /* BLE��������                  */
                ke_timer_set(APP_SYS_SENDDATA_TIMER, TASK_APP, 
                             APP_RFSEND_TIMEOUT);                       /* ����BLE���ͳ�ʱ��Ϣ          */ 
                __GusRFSendBufLen = 0;                                  /* ���������ݳ�������           */
                __GucUartDealFlag = 0;                                  /* ���uart���ݴ����־λ       */
                ke_timer_clear(APP_SYS_UARTRX_TIMER, TASK_APP);         /* ���uart���ճ�ʱ��Ϣ         */
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
** Descriptions:            BLE���ʹ�������
** input parameters:        index�����͵��������    s�����������ݵ�ָ��
**                          len���������ݵĳ���
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
** Descriptions:            ����0�����жϴ�����
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
    else if ( reg & UART_MASK_PE_IF ) {                                 /* ��żУ������ж�             */
        uart_uart_ClrIntFlag(QN_UART0, UART_MASK_PE_IF);                /* ����жϱ�־                 */
    }
    else if ( reg & UART_MASK_FE_IF ) {                                 /* ֡�����ж�                   */
        uart_uart_ClrIntFlag(QN_UART0, UART_MASK_FE_IF);                /* ����жϱ�־                 */
    } 
    else if ( reg & UART_MASK_OE_IF ) {                                 /* ��������־                 */
        uart_uart_ClrIntFlag(QN_UART0, UART_MASK_OE_IF);                /* ����жϱ�־                 */
    }
    else if ( reg & UART_MASK_RX_IF ) {                                 /* ����FIFO�ǿ��ж�             */
        __GucBLE_GetDataTemp[0] = uart_uart_GetRXD(QN_UART0);           /* ��ȡ����                     */

        if ((__GusUartBufhead + 1) >= UART_BUF_MAX_LEN) {               /* д�����ݸ����ﻺ���������   */
            if (__GusUartBufwear == 0) {
                return;                                                 /* ���������ˣ��������ݣ�����   */
            } else {
                
             /*
              *  �ӻ���������ͷ��ʼ��������
              */
             __GusUartBufhead = 0;                                   
             __GucUartBuf[__GusUartBufhead] = __GucBLE_GetDataTemp[0];
            }
        }
        else {                                                          /* д�����ݸ���δ�ﻺ��������� */
            if ((__GusUartBufhead + 1) == __GusUartBufwear) {
                return;                                                 /* ���������ˣ��������ݣ�����   */
            } else {

            /*
             *  �����򻺳����������� 
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

