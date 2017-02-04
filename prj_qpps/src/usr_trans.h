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
** Descriptions:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
*********************************************************************************************************/
#ifndef USR_TRANS_H_
#define USR_TRANS_H_

#include <stdint.h>
#include "app_env.h"
#include "led.h"
#include "uart.h"
#include "lib.h"
#include "usr_design.h"
#include "gpio.h"
#include "button.h"
#include "sleep.h"
#include "app_sys.h"


#define  APP_UART           QN_UART0
#define  APP_UART_BAUDE     UART_38400
//#define  UART_BUF_MAX_LEN   (1024*10)
//#define  UART_BUF_MAX_LEN   (1024)
#define  UART_BUF_MAX_LEN   (256)
 
extern volatile uint8_t  __GucBLE_Connected;
extern volatile uint16_t __GusUartBufhead;
extern volatile uint16_t __GusUartBufwear;
extern volatile uint8_t  __GucUartBuf[];

/*
 * ���崮�ڽ��մ��������ݱ���
 */
#define APP_UART_RXTIMEOUT     5
#define APP_SEND_PACKET_BYTES  20                                       /* ����BLEÿ�η���20������      */
#define APP_RFSEND_TIMEOUT     5
extern volatile uint8_t   __GucUartDealFlag;
extern volatile uint16_t  __GusUartTimeout;
extern volatile uint8_t   __GucRFSendBuf[];
extern volatile uint16_t  __GusRFSendBufLen;
extern volatile uint8_t   __GucRFSendFlag;
/*
 * ���崮�ڻ������
 */
extern volatile uint8_t  __GucBLE_GetDataTemp[];

/*
 * ����˯�߱���
 */
extern volatile uint8_t   __GucSleepState;

void APPSetBLEConnect(void);
void APPSetBLEDisConnect(void);
bool bAPPGetBLEConnectState(void);
void APPBLE_Active(void);
void APPBLE_Sleep(void);
void APPBLE_UartInit(void);
void APPUartReadData(void);
void APPUartDealData(void);

extern int app_SendData_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int app_UartRXTimeOut_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
void app_BLE_sendUart_data(uint8_t index, uint8_t* s, uint16_t len);

#endif
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
