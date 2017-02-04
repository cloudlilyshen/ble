/**
 ****************************************************************************************
 *
 * @file led.h
 *
 * @brief Header file of led driver for qn evb.
 *
 * Copyright(C) 2015 NXP Semiconductors N.V.
 * All rights reserved.
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef _LED_H_
#define _LED_H_


/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#if !defined(QN_9021_MINIDK)

//#define LED1_PIN    (GPIO_P26)//(GPIO_P05)
//#define LED2_PIN    (GPIO_P27)//(GPIO_P04)
//#define LED3_PIN    (GPIO_P31)//(GPIO_P03)
#define BUZZER_PIN    (GPIO_P26)
#define LED6_PIN    (GPIO_P31)
#define MOTORIA_PIN    (GPIO_P10)
#define MOTORIB_PIN    (GPIO_P11)
#define ADCCTRL_PIN    (GPIO_P27)



#else

//#define LED1_PIN    (GPIO_P03)
//#define LED2_PIN    (GPIO_P13)
//#define LED3_PIN    (GPIO_P02)  // no pin in QN9021
//#define LED4_PIN    (GPIO_P02)  // no pin in QN9021
//#define LED5_PIN    (GPIO_P02)  // no pin in QN9021

#endif

/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */

enum led_st
{
    LED_ON = 0,
    LED_OFF = (int)0xffffffff
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern void led_init(void);
//extern void led_matrix(uint32_t matrix);
//extern void led_set(uint32_t idx, enum led_st enable);
extern enum led_st led_get(uint32_t idx);


#define BUZZER_ON()            gpio_write_pin(BUZZER_PIN, GPIO_HIGH)
#define BUZZER_OFF()            gpio_write_pin(BUZZER_PIN, GPIO_LOW)

#define MOTOR_OPEN()            gpio_write_pin(MOTORIA_PIN, GPIO_HIGH);gpio_write_pin(MOTORIB_PIN, GPIO_LOW)
#define MOTOR_CLOSE()           gpio_write_pin(MOTORIA_PIN, GPIO_LOW);gpio_write_pin(MOTORIB_PIN, GPIO_HIGH)
#define MOTOR_STOP()           gpio_write_pin(MOTORIA_PIN, GPIO_LOW);gpio_write_pin(MOTORIB_PIN, GPIO_LOW)
#define ADC_POWERON()           gpio_write_pin(ADCCTRL_PIN, GPIO_HIGH)
#define ADC_POWEROFF()           gpio_write_pin(ADCCTRL_PIN, GPIO_LOW)


#endif
