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
 *  宏定义
 */

#define LED_ON_DUR_ADV_FAST   2                                         /* LED在快速广播时的点亮时间    */
#define LED_OFF_DUR_ADV_FAST  (uint16_t)((GAP_ADV_FAST_INTV2*0.625)/10) /* LED在快速广播时的熄灭时间    */
#define LED_ON_DUR_ADV_SLOW   2                                         /* LED在慢速广播时的点亮时间    */
#define LED_OFF_DUR_ADV_SLOW  (uint16_t)((GAP_ADV_SLOW_INTV*0.625)/10)  /* LED在慢速广播时的熄灭时间    */
#define LED_ON_DUR_CON        0xffff                                    /* LED在连接时的点亮时间        */
#define LED_OFF_DUR_CON       0                                         /* LED在连接时的熄灭时间        */
#define LED_ON_DUR_IDLE       0                                         /* LED在空闲时的点亮时间        */
#define LED_OFF_DUR_IDLE      0xffff                                    /* LED在空闲时的熄灭时间        */

#define EVENT_MOTOR_IS_ARRIVED_ID 0                                        /* 按键1按下事件的ID            */
#define EVENT_ALARM_START_ID 1                                      
#define EVENT_ALARM_STOP_ID 2                                      




//#define IOS_CONN_INTV_MAX      0x0010                                   /* 连接的最大间隔               */
//#define IOS_CONN_INTV_MIN      0x0008                                   /* 连接的最小间隔               */

#define IOS_CONN_INTV_MAX      0x0190                                   /* 连接的最大间隔               */
#define IOS_CONN_INTV_MIN      0x0190                                   /* 连接的最小间隔               */

#define IOS_SLAVE_LATENCY      0x0000                                   /* 从机的潜伏时间               */
#define IOS_STO_MULT           0x012c                                   /* 检测超时时间增益因子         */

/*
 *  全局变量定义
 */

struct usr_env_tag usr_env = {LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE};

/*********************************************************************************************************
** Function name:           usr_led4_set
** Descriptions:            LED状态设定
** input parameters:        timer_on：LED4的点亮时间   timer_off：LED4的熄灭时间
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
    usr_env.led4_on_dur = timer_on;                                     /* 初始化LED4的点亮时间         */
    usr_env.led4_off_dur = timer_off;                                   /* 初始化LED4的熄灭时间         */

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
        ke_timer_clear(APP_SYS_LED_4_TIMER, TASK_APP);                  /* 释放定时消息                 */
    }
    else
    {
//        led_set(4, LED_OFF);
        ke_timer_set(APP_SYS_LED_4_TIMER, TASK_APP, timer_off);         /* 注册一个定时消息             */
    }
}

/*********************************************************************************************************
** Function name:           usr_led4_process
** Descriptions:            LED4的用户处理程序，使LED4不断闪烁
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
** Descriptions:            应用程序任务的消息处理函数
** input parameters:        msgid：消息ID   param：消息参数
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
        case GAP_SET_MODE_REQ_CMP_EVT:                                  /* 设置模式请求完成             */
            if(APP_IDLE == ke_state_get(TASK_APP))                      /* 应用程序任务是否为空闲状态   */
            {
           //     usr_led4_set(LED_ON_DUR_ADV_FAST,LED_OFF_DUR_ADV_FAST); /* 设置LED4的状态               */
            //    ke_timer_set(APP_ADV_INTV_UPDATE_TIMER, TASK_APP, 
                  //           30 * 100);                                 /* 设置30s后向TASK_APP发消息    */
            }
            else if(APP_ADV == ke_state_get(TASK_APP))                  /* 应用程序任务是否为广播状态   */
            {
          //      usr_led4_set(LED_ON_DUR_ADV_SLOW,LED_OFF_DUR_ADV_SLOW); /* 设置LED4的状态               */
            }
            break;

        case GAP_ADV_REQ_CMP_EVT:                                       /* 广播请求完成                 */
       //     usr_led4_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
            ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);        /* 取消定时消息                 */
            break;

        case GAP_DISCON_CMP_EVT:                                        /* 断开连接完成                 */
       //     usr_led4_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
            APPSetBLEDisConnect();                                      /* 清除连接标志位               */
            /*
             *  发送广播
             */
            app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                    app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                    app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                    GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);
            break;

        case GAP_LE_CREATE_CONN_REQ_CMP_EVT:                            /* 建立连接请求完成             */
            if(((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.status == CO_ERROR_NO_ERROR)
            {
                if(GAP_PERIPHERAL_SLV == app_get_role())                /* 判断是否为从机               */
                {
                    ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);
            //        usr_led4_set(LED_ON_DUR_CON, LED_OFF_DUR_CON);

                    /*
                     *  更新连接参数
                     */
                    struct gap_conn_param_update conn_par;
                    conn_par.intv_min = IOS_CONN_INTV_MIN;          /* 连接最小间隔设定             */
                    conn_par.intv_max = IOS_CONN_INTV_MAX;          /* 连接最大间隔设定             */
                    conn_par.latency = IOS_SLAVE_LATENCY;           /* 从机潜伏时间设定             */
                    conn_par.time_out = IOS_STO_MULT;               /* 检测超时设定,Time=N *10 msec */
                    app_gap_param_update_req(((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.conhdl,
                                                 &conn_par);        /* 发送从设备更新连接参数请求   */
                    
                    APPSetBLEConnect();                             /* 置位连接标志位               */
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
** Descriptions:            处理LED4的状态设定消息
** input parameters:        msgid：APP_SYS_LED_4_TIMER        param：消息参数
**                          dest_id：TASK_APP                 src_id：TASK_APP
** output parameters:       none
** Returned value:          消息是否被执行
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
** Descriptions:            处理广播模式时间事件，这个函数用于通知应用程序广播模式的第一阶段已经超时
** input parameters:        msgid：APP_ADV_INTV_UPDATE_TIMER    param  ：None  
**                          dest_id：TASK_APP                   src_id ：TASK_APP
** output parameters:       none
** Returned value:          消息是否被执行
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
    //    usr_led4_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);                /* 设置LED4的状态              */
        
        /*
         *  更新广播参数
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
** Descriptions:            唤醒后重新初始化外设
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
    uart_init(QN_DEBUG_UART, USARTx_CLK(0), UART_38400);                /* 初始化用于打印调试信息的串口 */
    uart_tx_enable(QN_DEBUG_UART, MASK_ENABLE);
    uart_rx_enable(QN_DEBUG_UART, MASK_ENABLE);
#endif
    APPBLE_UartInit();                                                  /* 初始化串口                   */
}

/*********************************************************************************************************
** Function name:           app_timer_handler
** Descriptions:            退出消抖后的按键处理程序
** input parameters:        msgid：消息ID            param ：None
**                          dest_id：TASK_APP        src_id：TASK_APP
** output parameters:       none
** Returned value:          消息是否被执行
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
            if(gpio_read_pin(MOTOR_RUN_ARRIVE_PIN) == GPIO_LOW)                  /* 确保按键被按下               */
            {
                MOTOR_STOP();
#if 0

                
                if(APP_IDLE == ke_state_get(TASK_APP))                  /* 应用程序任务是否处于空闲状态 */
                {
                    if(!app_qpps_env->enabled)
                    {
                        /*
                         *  开始广播
                         */
                        app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                                app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                                app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                                GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);

#if (QN_DEEP_SLEEP_EN)
//                        sleep_set_pm(PM_SLEEP);                         /* usr设置为sleep模式           */
#endif
                    }
                }
                else if(APP_ADV == ke_state_get(TASK_APP))              /* 应用程序任务是否处于广播状态 */
                {
                    app_gap_adv_stop_req();                             /* 停止广播                     */

#if (QN_DEEP_SLEEP_EN)
//                    sleep_set_pm(PM_DEEP_SLEEP);                        /* usr设置为deep sleep模式      */
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
** Descriptions:            消抖前的按键处理程序
** input parameters:        none
** output parameters:       none
** Returned value:          消息是否被执行
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
        wakeup_32k_xtal_start_timer();                                  /* 开始外部32k晶振定时器唤醒    */
    }
#endif

    ke_timer_set(APP_SYS_MOTOR_IS_ARRIVED_TIMER, TASK_APP, 2);                  /* 延时20ms消抖                 */
    ke_evt_clear(1UL << EVENT_MOTOR_IS_ARRIVED_ID);                        /* 清除按键按下事件             */
}

void app_event_alarm_started_handler(void)
{
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
    if (sleep_env.deep_sleep)
    {
        sleep_env.deep_sleep = false;
        wakeup_32k_xtal_start_timer();                                  /* 开始外部32k晶振定时器唤醒    */
    }
#endif

    ke_timer_set(APP_SYS_ALARM_START_TIMER, TASK_APP, 2);                
    ke_evt_clear(1UL << EVENT_ALARM_START_ID);                        
}

/*********************************************************************************************************
** Function name:           usr_button1_cb
** Descriptions:            按键回调函数
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
     *  如果BLE进入sleep模式，则唤醒
     */
    if(ble_ext_wakeup_allow())
    {
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
        if (sleep_env.deep_sleep)
        {
            wakeup_32k_xtal_switch_clk();
        }
#endif
        sw_wakeup_ble_hw();                                             /* 唤醒BLE硬件                  */
    }

    /*
     *  按键消抖：我们可以设置一个软件定时器进行消抖。当BLE唤醒后，定时器并不会被立即校准，当然也并不精确
     *   因此这里设置一个事件，在事件处理中再设置软件定时器
     */
    ke_evt_set(1UL << EVENT_MOTOR_IS_ARRIVED_ID);


    
}
/*********************************************************************************************************
** Function name:           gpio_interrupt_callback
** Descriptions:            所有的GPIO中断回调函数
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
            usr_button1_cb();                                           /* 按键回调函数                 */   
            break;


            

#if (defined(QN_TEST_CTRL_PIN))
        case QN_TEST_CTRL_PIN:
            
            /*
             *  当测试控制引脚变为低电平时，将会重启系统
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
** Descriptions:            用户初始化函数
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
                               app_event_motor_is_arrived_handler))        /* 注册按键事件回调函数         */
    {
        ASSERT_ERR(0);
    }

    if(KE_EVENT_OK != ke_evt_callback_set(EVENT_ALARM_START_ID, 
                               app_event_alarm_started_handler))        /* 注册按键事件回调函数         */
    {
        ASSERT_ERR(0);
    }


    
    APPBLE_UartInit();                                                  /* 串口初始化                   */
}


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/


