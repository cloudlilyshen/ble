/**
 ****************************************************************************************
 *
 * @file button.h
 *
 * @brief Header file of button driver for qn evb.
 *
 * Copyright(C) 2015 NXP Semiconductors N.V.
 * All rights reserved.
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef _BUTTON_H_
#define _BUTTON_H_

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
#if !defined(QN_9021_MINIDK)

#if !defined(QN_EXT_FLASH)

#if defined(CFG_FCC_CE_TEST)
//#define BUTTON1_PIN    (CFG_FCC_CE_CTRL_PIN)
#define MOTOR_RUN_ARRIVE_PIN    (GPIO_P12)
#else
#define MOTOR_RUN_ARRIVE_PIN    (GPIO_P12)
#endif
#define LOCKED_PIN    (GPIO_P13)
#else

#if defined(CFG_FCC_CE_TEST)
//#define BUTTON1_PIN    (CFG_FCC_CE_CTRL_PIN)
#define MOTOR_RUN_ARRIVE_PIN    (GPIO_P12)
#else
//#define BUTTON1_PIN    (GPIO_P16)
#define MOTOR_RUN_ARRIVE_PIN    (GPIO_P12)

#endif
//#define BUTTON2_PIN    (GPIO_P15)	// Can't use in QN9022
#define LOCKED_PIN    (GPIO_P13)


#endif

#else

#if defined(CFG_FCC_CE_TEST)
//#define BUTTON1_PIN    (CFG_FCC_CE_CTRL_PIN)
#define MOTOR_RUN_ARRIVE_PIN    (GPIO_P12)
#else
//#define BUTTON1_PIN    (GPIO_P12)
#define MOTOR_RUN_ARRIVE_PIN    (GPIO_P12)

#endif
//#define BUTTON2_PIN    (GPIO_P10)
#define LOCKED_PIN    (GPIO_P13)
#endif

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern void button_init(void);
extern int check_button_state(int btn);

#endif

