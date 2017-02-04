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
 * 定义串口接收处理发送数据变量
 */
#define APP_UART_RXTIMEOUT     5
#define APP_SEND_PACKET_BYTES  20                                       /* 定义BLE每次发送20个数据      */
#define APP_RFSEND_TIMEOUT     5
extern volatile uint8_t   __GucUartDealFlag;
extern volatile uint16_t  __GusUartTimeout;
extern volatile uint8_t   __GucRFSendBuf[];
extern volatile uint16_t  __GusRFSendBufLen;
extern volatile uint8_t   __GucRFSendFlag;
/*
 * 定义串口缓存变量
 */
extern volatile uint8_t  __GucBLE_GetDataTemp[];

/*
 * 定义睡眠变量
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
