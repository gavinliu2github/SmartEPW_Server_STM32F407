/*=============================================================================*
 *
 *   * @file     : linear_actuator.h
 *    * @author        : skyline
 *     * @data       : 2014/3/4
 *      * @brief   : linear_actuator.c header file
 *       *
 *        *============================================================================*/




#ifndef __LINEAR_ACTUATOR_H__
#define __LINEAR_ACTUATOR_H__


#define ACTU_PWM_PORT							GPIOB
#define ACTU_A_PWM_PIN							GPIO_Pin_4
#define ACTU_B_PWM_PIN							GPIO_Pin_5

#define ACTU_CWCCW_PORT							GPIOD

#define ACTU_A_IN1_PIN							GPIO_Pin_0
#define ACTU_A_IN2_PIN							GPIO_Pin_1
#define ACTU_B_IN3_PIN							GPIO_Pin_2
#define ACTU_B_IN4_PIN							GPIO_Pin_3



/*
 *Limit Switch
 */
#define LS_READ_PORT							GPIOC
#define LS_A_UPPER_PIN							GPIO_Pin_8
#define LS_A_LOWER_PIN							GPIO_Pin_9
#define LS_B_UPPER_PIN							GPIO_Pin_14
#define LS_B_LOWER_PIN							GPIO_Pin_15

enum{
	STOP,
	CW,
	CCW
};

#endif
