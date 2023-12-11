/****************************************************
 *    File name: timer0.h
 *   Created on: Oct 6, 2022
 *       Author: Mohamed Badr
 *  Description: Header file for atmega32 TIMER0_PWM_MODE driver
 ****************************************************/

#ifndef TIMER0_H_
#define TIMER0_H_
/************Common Header Files***************/
#include "std_types.h"
/**********************************************/

/*************Static Configurations***************/
#define TIMER0_CLOCK_PRESCALER TIMER0_CLOCK_PRESCALER_F_CPU_64
/*************************************************/

/************Definitions***************/
#define TIMER0_CLOCK_PRESCALER_F_CPU_8 (0b0010)
#define TIMER0_CLOCK_PRESCALER_F_CPU_64 (0b0011)
/**************************************/

/***********User defined Data Types***************/

/*************************************************/

/**********Extern Public Global Variable************/

/***************************************************/

/***************Function Prototypes For Public Functions**********************/
/*
 * Description :Function to initialize timer0 in PWM mode
 * 	TIMER0 CONFIGURATION
 *
 * PWM MODE ==>  FOC0=0,WGM00=1,WGM01=1
 * NON-INVERTING MODE ==> COM00=0,COM01=1
 * Ftimer0=Fcpu/256*N
 * N=64 ==> CS00=0,CS01=1,CS02=1
 */
void PWM_Timer0_Start (uint8 dutyCycle);
/****************************************************************************/

/***********Macros***********/

/****************************/

#endif /* TIMER0_H_ */
