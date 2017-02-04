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

#include "stdint.h"
#include "app_config.h"
#include "ble.h"
#include "intc.h"
#include "lib.h"
#if (QN_WORK_MODE != WORK_MODE_HCI)
    #include "prf_utils.h"
#endif
#if (QN_WORK_MODE == WORK_MODE_SOC)
    #include "app_env.h"
#endif

#include "usr_design.h"
#include "system.h"
#include "uart.h"
#include "spi.h"
#include "sleep.h"
#include "led.h"
#include "usr_trans.h"

/*
 *  ���ر���
 */

static uint8_t ble_heap[BLE_HEAP_SIZE];
#if QN_NVDS_WRITE
static uint8_t nvds_tmp_buf[NVDS_TMP_BUF_SIZE];
#endif



#ifdef CFG_DBG_PRINT
/*********************************************************************************************************
** Function name:           assert_err
** Descriptions:            ����
** input parameters:        condition������   file��������ļ�     line�����������к�
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void assert_err(const char *condition, const char * file, int line)
{
    QPRINTF("ASSERT_ERR(%s), in %s at line %d\r\n", condition, file, line);
    GLOBAL_INT_STOP();
    while(1);
}
/*********************************************************************************************************
** Function name:           assert_param
** Descriptions:            ����
** input parameters:        param0������0   param1������1       file��������ļ�     line�����������к�
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void assert_param(int param0, int param1, const char * file, int line)
{
    GLOBAL_INT_STOP();
    while(1);
}
/*********************************************************************************************************
** Function name:           assert_warn
** Descriptions:            ����
** input parameters:        condition������   file��������ļ�     line�����������к�
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void assert_warn(const char *condition, const char * file, int line)
{
    QPRINTF("ASSERT_WARN(%s), in %s at line %d\r\n", condition, file, line);
}
#endif 

/*********************************************************************************************************
** Function name:           HardFault_Handler
** Descriptions:            Ӳ�������жϴ�����
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
#if (defined(QN_SW_RELEASE))
void HardFault_Handler(void)
{
    syscon_SetCRSS(QN_SYSCON, SYSCON_MASK_REBOOT_SYS);                  /* ����ϵͳ                     */
}
#endif

/*********************************************************************************************************
** Function name:           prf_register
** Descriptions:            Э��ע�ắ�����˺�����Ӧ�ó�������Ҫ������Э��ע�ᵽBLEЭ��ջ��
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
#if (QN_WORK_MODE != WORK_MODE_HCI)
static void prf_register(void)
{
    prf_init_reg(prf_init);                                             /* ע��Э���ʼ��������Э��ջ   */

    /*
     *  ע��Э��Ͽ����ӻص�������BLEЭ��ջ
     */
    prf_disp_disconnect_reg(prf_dispatch_disconnect);                   
}
#endif

/*********************************************************************************************************
** Function name:           main
** Descriptions:            ʵ��BLE͸������
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
int main(void)
{
    int ble_sleep_st, usr_sleep_st;

    /*
     *  ������ص������ã�
     *  xadd_c = 1ʱ ���ص��� = 10 + xcsel*0.32pf   
     *  xadd_c = 0ʱ ���ص��� = 6 + xcsel*0.3pf  
     *  xcsel��ֵ����NVDS��
   */
    syscon_SetAdditionCRWithMask(QN_SYSCON, SYSCON_MASK_XADD_C, MASK_ENABLE);/*Add extra capacitor in the crystal tank*/

   
    dc_dc_enable(QN_DC_DC_ENABLE);                                      /* DC-DC����                    */

#if (QN_32K_LOW_POWER_MODE_EN==TRUE)
    enable_32k_mode();
#endif

    /*
     *  ƽ̨��ʼ��
     */
#if QN_NVDS_WRITE
    plf_init(QN_POWER_MODE, __XTAL, QN_32K_RCO, nvds_tmp_buf, NVDS_TMP_BUF_SIZE);
#else
    plf_init(QN_POWER_MODE, __XTAL, QN_32K_RCO, NULL, 0);
#endif

    SystemInit();                                                       /* ϵͳ��ʼ��                   */

#if (QN_WORK_MODE != WORK_MODE_HCI)
    prf_register();                                                     /* Э��ע��                     */
#endif

    /*
     *  BLEЭ��ջ��ʼ��    ��ʹ��QN_TEST_CTRL_PIN��QN_TEST_CTRL_PIN����Ϊ�͵�ƽ������򽫽���
     *  ����ģʽ��ontroller mode������ཫ����usr_config.h�ж����ģʽ��BLE͸�����������
     *  WORK_MODE_SOCģʽ��
     */
#if (defined(QN_TEST_CTRL_PIN))
    if(gpio_read_pin(QN_TEST_CTRL_PIN) == GPIO_HIGH)
    {
#endif
        ble_init((enum WORK_MODE)QN_WORK_MODE, QN_HCI_PORT, QN_HCI_RD, 
                  QN_HCI_WR, ble_heap, BLE_HEAP_SIZE, QN_BLE_SLEEP);    /* ����ģʽ��usr_config.h�ж��� */
#if (defined(QN_TEST_CTRL_PIN))
    }
    else
    {
        ble_init((enum WORK_MODE)WORK_MODE_HCI, QN_HCI_PORT, QN_HCI_RD, 
                    QN_HCI_WR, ble_heap, BLE_HEAP_SIZE, false);
        gpio_set_interrupt(QN_TEST_CTRL_PIN, GPIO_INT_HIGH_LEVEL);
        gpio_enable_interrupt(QN_TEST_CTRL_PIN);
    }
#endif

    set_max_sleep_duration(QN_BLE_MAX_SLEEP_DUR);                       /* ����˯��������ʱ��         */ 

#if (QN_WORK_MODE == WORK_MODE_SOC)
    app_init();                                                         /* Ӧ�ó����ʼ��               */
#endif

    usr_init();                                                         /* �û���ʼ������               */

    sleep_init();                                                       /* �͹��ĳ�ʼ��                 */
    wakeup_by_sleep_timer(__32K_TYPE);                                  /* ���õ͹��Ķ�ʱ������         */

    GLOBAL_INT_START();                                                 /* ʹ�����ж�                   */
//    APPBLE_Active();                                                    /* �����û�Ӧ�ó���Ĺ���ģʽ   */ 
    APPBLE_Sleep();
    while(1)
    {
        APPUartDealData();                                              /* BLE����uart���յ�������      */
        
        ke_schedule();                                                  /* ִ����Ϣ���ȹ���             */

        GLOBAL_INT_DISABLE_WITHOUT_TUNER();                             /* �رճ���Ƶtx��rx�ж�����ж� */

        usr_sleep_st = usr_sleep();                                     /* ��ȡ�û������趨�Ĺ���ģʽ   */

        /*
         *  ����û��������sleep��deep sleepģʽ����ִ������ĳ��򣬽���LED״ָ̬ʾ��
         *  �����BLEЭ��ջ�������Ĺ���ģʽ��������Ӧ�Ĺ���ģʽ���ж�����Ϊ�±�
         *   MCU����Ĺ���ģʽ��������
         *   +--------+--------+--------+--------+--------+
         *   |    USR |        |        |        |        |
         *   | BLE    | ACTIVE | IDLE   | SLEEP  | DEEP   |
         *   +--------+--------+--------+--------+--------+
         *   | ACTIVE | active | active | active | active |
         *   | IDLE   | active | idle   | idle   | idle   |
         *   | SLEEP  | active | idle   | sleep  | deep   |
         *   +--------+--------+--------+--------+--------+
         */

//            QPRINTF("usr_sleep_st: 0x%X.\r\n",usr_sleep_st);
        
        if(usr_sleep_st != PM_ACTIVE)                                   
        {
//            QPRINTF("enter sleep\r\n");
            
            ble_sleep_st = ble_sleep(usr_sleep_st);                     /* ��ȡBLE�͹���ģʽ��״̬      */

            if(((ble_sleep_st == PM_IDLE) || (usr_sleep_st == PM_IDLE))
             && (ble_sleep_st != PM_ACTIVE))                            /* �ж�MCU�Ƿ���Խ���idleģʽ  */
            {
                enter_sleep(SLEEP_CPU_CLK_OFF,
                            WAKEUP_BY_ALL_IRQ_SOURCE, NULL);            /* ����idleģʽ                 */  
            }
            else if((ble_sleep_st == PM_SLEEP) && 
                    (usr_sleep_st == PM_SLEEP))                         /* �ж�MCU�Ƿ���Խ���sleepģʽ */
            {
//            QPRINTF("enter sleep\r\n");
                
                enter_sleep(SLEEP_NORMAL,
                            (WAKEUP_BY_OSC_EN | WAKEUP_BY_GPIO),
                            sleep_cb);                                  /* ����sleepģʽ                */
            }

            else if((ble_sleep_st == PM_SLEEP) && 
                     (usr_sleep_st == PM_DEEP_SLEEP))                   /* �ж�MCU�Ƿ���Խ���deep sleep*/
            {
                enter_sleep(SLEEP_DEEP,
                            WAKEUP_BY_GPIO, sleep_cb);                  /* ����deep sleep��GPIO����     */
            }
        }
        
        GLOBAL_INT_RESTORE_WITHOUT_TUNER();                             /* �ָ�֮ǰ�رյ��ж�           */
    }
}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/

