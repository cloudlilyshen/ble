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
#include <stdint.h>
#include "app_env.h"
#include "led.h"
#include "uart.h"
#include "lib.h"
#include "usr_design.h"
#include "gpio.h"
#include "button.h"
#include "sleep.h"
#include "usr_trans.h"

/*
 *  �궨��
 */

#define LED_ON_DUR_ADV_FAST   2                                         /* LED�ڿ��ٹ㲥ʱ�ĵ���ʱ��    */
#define LED_OFF_DUR_ADV_FAST  (uint16_t)((GAP_ADV_FAST_INTV2*0.625)/10) /* LED�ڿ��ٹ㲥ʱ��Ϩ��ʱ��    */
#define LED_ON_DUR_ADV_SLOW   2                                         /* LED�����ٹ㲥ʱ�ĵ���ʱ��    */
#define LED_OFF_DUR_ADV_SLOW  (uint16_t)((GAP_ADV_SLOW_INTV*0.625)/10)  /* LED�����ٹ㲥ʱ��Ϩ��ʱ��    */
#define LED_ON_DUR_CON        0xffff                                    /* LED������ʱ�ĵ���ʱ��        */
#define LED_OFF_DUR_CON       0                                         /* LED������ʱ��Ϩ��ʱ��        */
#define LED_ON_DUR_IDLE       0                                         /* LED�ڿ���ʱ�ĵ���ʱ��        */
#define LED_OFF_DUR_IDLE      0xffff                                    /* LED�ڿ���ʱ��Ϩ��ʱ��        */

#define EVENT_MOTOR_IS_ARRIVED_ID 0                                        /* ����1�����¼���ID            */
#define EVENT_ALARM_START_ID 1                                      
#define EVENT_ALARM_STOP_ID 2                                      




//#define IOS_CONN_INTV_MAX      0x0010                                   /* ���ӵ������               */
//#define IOS_CONN_INTV_MIN      0x0008                                   /* ���ӵ���С���               */

#define IOS_CONN_INTV_MAX      0x0190                                   /* ���ӵ������               */
#define IOS_CONN_INTV_MIN      0x0190                                   /* ���ӵ���С���               */

#define IOS_SLAVE_LATENCY      0x0000                                   /* �ӻ���Ǳ��ʱ��               */
#define IOS_STO_MULT           0x012c                                   /* ��ⳬʱʱ����������         */

/*
 *  ȫ�ֱ�������
 */

struct usr_env_tag usr_env = {LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE};

/*********************************************************************************************************
** Function name:           usr_led4_set
** Descriptions:            LED״̬�趨
** input parameters:        timer_on��LED4�ĵ���ʱ��   timer_off��LED4��Ϩ��ʱ��
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
static void usr_led4_set(uint16_t timer_on, uint16_t timer_off)
{
    usr_env.led4_on_dur = timer_on;                                     /* ��ʼ��LED4�ĵ���ʱ��         */
    usr_env.led4_off_dur = timer_off;                                   /* ��ʼ��LED4��Ϩ��ʱ��         */

    if (timer_on == 0 || timer_off == 0)
    {
        if (timer_on == 0)
        {
//            led_set(4, LED_OFF);
        }
        if (timer_off == 0)
        {
//            led_set(4, LED_ON);
        }
        ke_timer_clear(APP_SYS_LED_4_TIMER, TASK_APP);                  /* �ͷŶ�ʱ��Ϣ                 */
    }
    else
    {
//        led_set(4, LED_OFF);
        ke_timer_set(APP_SYS_LED_4_TIMER, TASK_APP, timer_off);         /* ע��һ����ʱ��Ϣ             */
    }
}

/*********************************************************************************************************
** Function name:           usr_led4_process
** Descriptions:            LED4���û��������ʹLED4������˸
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
static void usr_led4_process(void)
{
    if(led_get(4) == LED_ON)
    {
//        led_set(4, LED_OFF);
   //     ke_timer_set(APP_SYS_LED_4_TIMER, TASK_APP, usr_env.led4_off_dur);
    }
    else
    {
//        led_set(4, LED_ON);
 //       ke_timer_set(APP_SYS_LED_4_TIMER, TASK_APP, usr_env.led4_on_dur);
    }
}
/*********************************************************************************************************
** Function name:           app_task_msg_hdl
** Descriptions:            Ӧ�ó����������Ϣ������
** input parameters:        msgid����ϢID   param����Ϣ����
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
        case GAP_SET_MODE_REQ_CMP_EVT:                                  /* ����ģʽ�������             */
            if(APP_IDLE == ke_state_get(TASK_APP))                      /* Ӧ�ó��������Ƿ�Ϊ����״̬   */
            {
           //     usr_led4_set(LED_ON_DUR_ADV_FAST,LED_OFF_DUR_ADV_FAST); /* ����LED4��״̬               */
            //    ke_timer_set(APP_ADV_INTV_UPDATE_TIMER, TASK_APP, 
                  //           30 * 100);                                 /* ����30s����TASK_APP����Ϣ    */
            }
            else if(APP_ADV == ke_state_get(TASK_APP))                  /* Ӧ�ó��������Ƿ�Ϊ�㲥״̬   */
            {
          //      usr_led4_set(LED_ON_DUR_ADV_SLOW,LED_OFF_DUR_ADV_SLOW); /* ����LED4��״̬               */
            }
            break;

        case GAP_ADV_REQ_CMP_EVT:                                       /* �㲥�������                 */
       //     usr_led4_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
            ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);        /* ȡ����ʱ��Ϣ                 */
            break;

        case GAP_DISCON_CMP_EVT:                                        /* �Ͽ��������                 */
       //     usr_led4_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
            APPSetBLEDisConnect();                                      /* ������ӱ�־λ               */
            /*
             *  ���͹㲥
             */
            app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                    app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                    app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                    GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);
            break;

        case GAP_LE_CREATE_CONN_REQ_CMP_EVT:                            /* ���������������             */
            if(((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.status == CO_ERROR_NO_ERROR)
            {
                if(GAP_PERIPHERAL_SLV == app_get_role())                /* �ж��Ƿ�Ϊ�ӻ�               */
                {
                    ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);
            //        usr_led4_set(LED_ON_DUR_CON, LED_OFF_DUR_CON);

                    /*
                     *  �������Ӳ���
                     */
                    struct gap_conn_param_update conn_par;
                    conn_par.intv_min = IOS_CONN_INTV_MIN;          /* ������С����趨             */
                    conn_par.intv_max = IOS_CONN_INTV_MAX;          /* ����������趨             */
                    conn_par.latency = IOS_SLAVE_LATENCY;           /* �ӻ�Ǳ��ʱ���趨             */
                    conn_par.time_out = IOS_STO_MULT;               /* ��ⳬʱ�趨,Time=N *10 msec */
                    app_gap_param_update_req(((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.conhdl,
                                                 &conn_par);        /* ���ʹ��豸�������Ӳ�������   */
                    
                    APPSetBLEConnect();                             /* ��λ���ӱ�־λ               */
                }
            }
            break;

        case QPPS_DISABLE_IND:
            break;

        case QPPS_CFG_INDNTF_IND:
            break;

        default:
            break;
    }
}

/*********************************************************************************************************
** Function name:           app_led_timer_handler
** Descriptions:            ����LED4��״̬�趨��Ϣ
** input parameters:        msgid��APP_SYS_LED_4_TIMER        param����Ϣ����
**                          dest_id��TASK_APP                 src_id��TASK_APP
** output parameters:       none
** Returned value:          ��Ϣ�Ƿ�ִ��
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
int app_led_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(msgid == APP_SYS_LED_4_TIMER)
    {
       // usr_led4_process();
    }

    return (KE_MSG_CONSUMED);
}

/*********************************************************************************************************
** Function name:           app_gap_adv_intv_update_timer_handler
** Descriptions:            ����㲥ģʽʱ���¼��������������֪ͨӦ�ó���㲥ģʽ�ĵ�һ�׶��Ѿ���ʱ
** input parameters:        msgid��APP_ADV_INTV_UPDATE_TIMER    param  ��None  
**                          dest_id��TASK_APP                   src_id ��TASK_APP
** output parameters:       none
** Returned value:          ��Ϣ�Ƿ�ִ��
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
int app_gap_adv_intv_update_timer_handler(ke_msg_id_t const msgid, void const *param,
                                          ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(APP_ADV == ke_state_get(TASK_APP))
    {
    //    usr_led4_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);                /* ����LED4��״̬              */
        
        /*
         *  ���¹㲥����
         */
       app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE, 
                                app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE), 
                                app_env.scanrsp_data, 
                                app_set_scan_rsp_data(app_get_local_service_flag()),
                                GAP_ADV_SLOW_INTV, GAP_ADV_SLOW_INTV);
    }

    return (KE_MSG_CONSUMED);
}
/*********************************************************************************************************
** Function name:           usr_sleep_restore
** Descriptions:            ���Ѻ����³�ʼ������
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void usr_sleep_restore(void)
{
#if QN_DBG_PRINT
    uart_init(QN_DEBUG_UART, USARTx_CLK(0), UART_38400);                /* ��ʼ�����ڴ�ӡ������Ϣ�Ĵ��� */
    uart_tx_enable(QN_DEBUG_UART, MASK_ENABLE);
    uart_rx_enable(QN_DEBUG_UART, MASK_ENABLE);
#endif
    APPBLE_UartInit();                                                  /* ��ʼ������                   */
}

/*********************************************************************************************************
** Function name:           app_timer_handler
** Descriptions:            �˳�������İ����������
** input parameters:        msgid����ϢID            param ��None
**                          dest_id��TASK_APP        src_id��TASK_APP
** output parameters:       none
** Returned value:          ��Ϣ�Ƿ�ִ��
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
int app_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    switch(msgid)
    {
        case APP_SYS_MOTOR_IS_ARRIVED_TIMER:
            if(gpio_read_pin(MOTOR_RUN_ARRIVE_PIN) == GPIO_LOW)                  /* ȷ������������               */
            {
                MOTOR_STOP();
#if 0

                
                if(APP_IDLE == ke_state_get(TASK_APP))                  /* Ӧ�ó��������Ƿ��ڿ���״̬ */
                {
                    if(!app_qpps_env->enabled)
                    {
                        /*
                         *  ��ʼ�㲥
                         */
                        app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                                app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                                app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                                GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);

#if (QN_DEEP_SLEEP_EN)
//                        sleep_set_pm(PM_SLEEP);                         /* usr����Ϊsleepģʽ           */
#endif
                    }
                }
                else if(APP_ADV == ke_state_get(TASK_APP))              /* Ӧ�ó��������Ƿ��ڹ㲥״̬ */
                {
                    app_gap_adv_stop_req();                             /* ֹͣ�㲥                     */

#if (QN_DEEP_SLEEP_EN)
//                    sleep_set_pm(PM_DEEP_SLEEP);                        /* usr����Ϊdeep sleepģʽ      */
#endif
                }

#endif                
            }
            break;


        case APP_SYS_ALARM_START_TIMER:
             QPRINTF("APP_SYS_ALARM_START_TIMER\r\n");
            ke_timer_set(APP_SYS_ALARM_ON_TIMER, TASK_APP, 2);                
        break;

        case APP_SYS_ALARM_ON_TIMER:
            BUZZER_ON();
            ke_timer_set(APP_SYS_ALARM_OFF_TIMER, TASK_APP, 20);                
        break;

        case APP_SYS_ALARM_OFF_TIMER:
            BUZZER_OFF();
            ke_timer_set(APP_SYS_ALARM_ON_TIMER, TASK_APP, 20);                
        break;


        case APP_SYS_ALARM_STOP_TIMER:
            BUZZER_OFF();
        break;


        case APP_SYS_OPEN_TIMER:
            MOTOR_OPEN();
            ke_timer_set(APP_SYS_STOP_TIMER, TASK_APP, 40);     
             QPRINTF("APP_SYS_OPEN_TIMER\r\n");
            
        break;


        case APP_SYS_CLOSE_TIMER:
            MOTOR_CLOSE();
            ke_timer_set(APP_SYS_STOP_TIMER, TASK_APP, 40);     
             QPRINTF("APP_SYS_CLOSE_TIMER\r\n");
            
        break;

        case APP_SYS_STOP_TIMER:
                MOTOR_STOP();
        break;

        default:
            ASSERT_ERR(0);
            break;
    }

    return (KE_MSG_CONSUMED);
}
/*********************************************************************************************************
** Function name:           app_event_motor_is_arrived_handler
** Descriptions:            ����ǰ�İ����������
** input parameters:        none
** output parameters:       none
** Returned value:          ��Ϣ�Ƿ�ִ��
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void app_event_motor_is_arrived_handler(void)
{
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
    if (sleep_env.deep_sleep)
    {
        sleep_env.deep_sleep = false;
        wakeup_32k_xtal_start_timer();                                  /* ��ʼ�ⲿ32k����ʱ������    */
    }
#endif

    ke_timer_set(APP_SYS_MOTOR_IS_ARRIVED_TIMER, TASK_APP, 2);                  /* ��ʱ20ms����                 */
    ke_evt_clear(1UL << EVENT_MOTOR_IS_ARRIVED_ID);                        /* ������������¼�             */
}

void app_event_alarm_started_handler(void)
{
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
    if (sleep_env.deep_sleep)
    {
        sleep_env.deep_sleep = false;
        wakeup_32k_xtal_start_timer();                                  /* ��ʼ�ⲿ32k����ʱ������    */
    }
#endif

    ke_timer_set(APP_SYS_ALARM_START_TIMER, TASK_APP, 2);                
    ke_evt_clear(1UL << EVENT_ALARM_START_ID);                        
}

/*********************************************************************************************************
** Function name:           usr_button1_cb
** Descriptions:            �����ص�����
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void usr_button1_cb(void)
{
    /*
     *  ���BLE����sleepģʽ������
     */
    if(ble_ext_wakeup_allow())
    {
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
        if (sleep_env.deep_sleep)
        {
            wakeup_32k_xtal_switch_clk();
        }
#endif
        sw_wakeup_ble_hw();                                             /* ����BLEӲ��                  */
    }

    /*
     *  �������������ǿ�������һ�������ʱ��������������BLE���Ѻ󣬶�ʱ�������ᱻ����У׼����ȻҲ������ȷ
     *   �����������һ���¼������¼������������������ʱ��
     */
    ke_evt_set(1UL << EVENT_MOTOR_IS_ARRIVED_ID);


    
}
/*********************************************************************************************************
** Function name:           gpio_interrupt_callback
** Descriptions:            ���е�GPIO�жϻص�����
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void gpio_interrupt_callback(enum gpio_pin pin)
{
    switch(pin)
    {
        case MOTOR_RUN_ARRIVE_PIN:
            usr_button1_cb();                                           /* �����ص�����                 */   
            break;


            

#if (defined(QN_TEST_CTRL_PIN))
        case QN_TEST_CTRL_PIN:
            
            /*
             *  �����Կ������ű�Ϊ�͵�ƽʱ����������ϵͳ
             */
            gpio_disable_interrupt(QN_TEST_CTRL_PIN);
            syscon_SetCRSS(QN_SYSCON, SYSCON_MASK_REBOOT_SYS);
            break;
#endif

        case LOCKED_PIN:
            
        break;

        default:
            break;
    }
}

/*********************************************************************************************************
** Function name:           usr_init
** Descriptions:            �û���ʼ������
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created Date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void usr_init(void)
{
    if(KE_EVENT_OK != ke_evt_callback_set(EVENT_MOTOR_IS_ARRIVED_ID, 
                               app_event_motor_is_arrived_handler))        /* ע�ᰴ���¼��ص�����         */
    {
        ASSERT_ERR(0);
    }

    if(KE_EVENT_OK != ke_evt_callback_set(EVENT_ALARM_START_ID, 
                               app_event_alarm_started_handler))        /* ע�ᰴ���¼��ص�����         */
    {
        ASSERT_ERR(0);
    }


    
    APPBLE_UartInit();                                                  /* ���ڳ�ʼ��                   */
}


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/


