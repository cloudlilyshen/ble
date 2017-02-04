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
 *  本地变量
 */

static uint8_t ble_heap[BLE_HEAP_SIZE];
#if QN_NVDS_WRITE
static uint8_t nvds_tmp_buf[NVDS_TMP_BUF_SIZE];
#endif



#ifdef CFG_DBG_PRINT
/*********************************************************************************************************
** Function name:           assert_err
** Descriptions:            断言
** input parameters:        condition：条件   file：出错的文件     line：出错代码的行号
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
** Descriptions:            断言
** input parameters:        param0：参数0   param1：参数1       file：出错的文件     line：出错代码的行号
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
** Descriptions:            断言
** input parameters:        condition：条件   file：出错的文件     line：出错代码的行号
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
** Descriptions:            硬件错误中断处理函数
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
    syscon_SetCRSS(QN_SYSCON, SYSCON_MASK_REBOOT_SYS);                  /* 重启系统                     */
}
#endif

/*********************************************************************************************************
** Function name:           prf_register
** Descriptions:            协议注册函数，此函数将应用程序所需要的所有协议注册到BLE协议栈中
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
    prf_init_reg(prf_init);                                             /* 注册协议初始化函数到协议栈   */

    /*
     *  注册协议断开连接回调函数到BLE协议栈
     */
    prf_disp_disconnect_reg(prf_dispatch_disconnect);                   
}
#endif

/*********************************************************************************************************
** Function name:           main
** Descriptions:            实现BLE透传功能
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
     *  晶振加载电容设置，
     *  xadd_c = 1时 加载电容 = 10 + xcsel*0.32pf   
     *  xadd_c = 0时 加载电容 = 6 + xcsel*0.3pf  
     *  xcsel的值存在NVDS中
   */
    syscon_SetAdditionCRWithMask(QN_SYSCON, SYSCON_MASK_XADD_C, MASK_ENABLE);/*Add extra capacitor in the crystal tank*/

   
    dc_dc_enable(QN_DC_DC_ENABLE);                                      /* DC-DC设置                    */

#if (QN_32K_LOW_POWER_MODE_EN==TRUE)
    enable_32k_mode();
#endif

    /*
     *  平台初始化
     */
#if QN_NVDS_WRITE
    plf_init(QN_POWER_MODE, __XTAL, QN_32K_RCO, nvds_tmp_buf, NVDS_TMP_BUF_SIZE);
#else
    plf_init(QN_POWER_MODE, __XTAL, QN_32K_RCO, NULL, 0);
#endif

    SystemInit();                                                       /* 系统初始化                   */

#if (QN_WORK_MODE != WORK_MODE_HCI)
    prf_register();                                                     /* 协议注册                     */
#endif

    /*
     *  BLE协议栈初始化    若使能QN_TEST_CTRL_PIN且QN_TEST_CTRL_PIN引脚为低电平，则程序将进入
     *  测试模式（ontroller mode），否侧将进入usr_config.h中定义的模式，BLE透传测试需进入
     *  WORK_MODE_SOC模式。
     */
#if (defined(QN_TEST_CTRL_PIN))
    if(gpio_read_pin(QN_TEST_CTRL_PIN) == GPIO_HIGH)
    {
#endif
        ble_init((enum WORK_MODE)QN_WORK_MODE, QN_HCI_PORT, QN_HCI_RD, 
                  QN_HCI_WR, ble_heap, BLE_HEAP_SIZE, QN_BLE_SLEEP);    /* 工作模式在usr_config.h中定义 */
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

    set_max_sleep_duration(QN_BLE_MAX_SLEEP_DUR);                       /* 设置睡眠最大持续时间         */ 

#if (QN_WORK_MODE == WORK_MODE_SOC)
    app_init();                                                         /* 应用程序初始化               */
#endif

    usr_init();                                                         /* 用户初始化函数               */

    sleep_init();                                                       /* 低功耗初始化                 */
    wakeup_by_sleep_timer(__32K_TYPE);                                  /* 设置低功耗定时器唤醒         */

    GLOBAL_INT_START();                                                 /* 使能总中断                   */
//    APPBLE_Active();                                                    /* 设置用户应用程序的功耗模式   */ 
    APPBLE_Sleep();
    while(1)
    {
        APPUartDealData();                                              /* BLE发送uart接收到的数据      */
        
        ke_schedule();                                                  /* 执行消息调度管理             */

        GLOBAL_INT_DISABLE_WITHOUT_TUNER();                             /* 关闭除射频tx、rx中断外的中断 */

        usr_sleep_st = usr_sleep();                                     /* 获取用户程序设定的功耗模式   */

        /*
         *  如果用户程序进入sleep或deep sleep模式，则执行下面的程序，进行LED状态指示，
         *  并结合BLE协议栈允许进入的功耗模式，进入相应的功耗模式，判断依据为下表：
         *   MCU进入的功耗模式及条件：
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
            
            ble_sleep_st = ble_sleep(usr_sleep_st);                     /* 获取BLE低功耗模式的状态      */

            if(((ble_sleep_st == PM_IDLE) || (usr_sleep_st == PM_IDLE))
             && (ble_sleep_st != PM_ACTIVE))                            /* 判断MCU是否可以进入idle模式  */
            {
                enter_sleep(SLEEP_CPU_CLK_OFF,
                            WAKEUP_BY_ALL_IRQ_SOURCE, NULL);            /* 进入idle模式                 */  
            }
            else if((ble_sleep_st == PM_SLEEP) && 
                    (usr_sleep_st == PM_SLEEP))                         /* 判断MCU是否可以进入sleep模式 */
            {
//            QPRINTF("enter sleep\r\n");
                
                enter_sleep(SLEEP_NORMAL,
                            (WAKEUP_BY_OSC_EN | WAKEUP_BY_GPIO),
                            sleep_cb);                                  /* 进入sleep模式                */
            }

            else if((ble_sleep_st == PM_SLEEP) && 
                     (usr_sleep_st == PM_DEEP_SLEEP))                   /* 判断MCU是否可以进入deep sleep*/
            {
                enter_sleep(SLEEP_DEEP,
                            WAKEUP_BY_GPIO, sleep_cb);                  /* 进入deep sleep，GPIO唤醒     */
            }
        }
        
        GLOBAL_INT_RESTORE_WITHOUT_TUNER();                             /* 恢复之前关闭的中断           */
    }
}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/

