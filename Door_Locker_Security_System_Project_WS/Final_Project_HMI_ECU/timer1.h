/*
 *    File name: timer1.h
 *   Created on: Oct 26, 2022
 *       Author: Mohamed Badr
 *  Description: Header file for the ATmega32 TIMER1 driver
 */

#ifndef TIMER1_H_
#define TIMER1_H_

/************Common Header Files***************/
#include "std_types.h"
/**********************************************/

/*************Static Configurations***************/

/*************************************************/

/************Definitions***************/

/**************************************/

/***********User defined Data Types***************/

typedef enum
{
	TIMER1_NO_CLOCK,
	TIMER1_F_CPU_1,
	TIMER1_F_CPU_8,
	TIMER1_F_CPU_64,
	TIMER1_F_CPU_256,
	TIMER1_F_CPU_1024,
	TIMER1_EXTERNAL_CLOCK_FALLING,/*External clock source on T1 pin. Clock on falling edge*/
	TIMER1_EXTERNAL_CLOCK_RISING/*External clock source on T1 pin. Clock on rising edge*/
} TIMER1_Prescaler;

typedef enum
{
	TIMER1_NORMAL_MODE = 0,/*WGM13=0,WGM12=0,WGM11=0,WGM10=0 ==> TOP=0xFFFF*/
	TIMER1_CTC_MODE = 0b0100,/*WGM13=0,WGM12=1,WGM11=0,WGM10=0 ==> TOP stored in OCR1A*/
	TIMER1_FAST_PWM_MODE = 0b1111 /*WGM13=1,WGM12=1,WGM11=1,WGM10=1 ==> TOP stored in OCR1A*/
} TIMER1_Mode;

typedef enum
{
	TIMER1_CTC_MODE_NO_OUTPUT,/*Normal port operation, OC1A/OC1B disconnected.*/
	TIMER1_CTC_MODE_TOGGLE , /*Toggle OC1A/OC1B on each compare match*/
	TIMER1_CTC_MODE_CLEAR, /*Clear OC1A/OC1B on each compare match (Set output to low level)*/
	TIMER1_CTC_MODE_SET /*Set OC1A/OC1B on each compare match (Set output to high level)*/
} TIMER1_ctcModeOutput;/**OC1A/OC1B outputs in case of CTC mode**/

typedef enum
{
	TIMER1_OUTPUT_PIN_OC1B_ONLY=0b01, /*In this case output will be on OC1B only*/
	TIMER1_OUTPUT_PINS_OC1B_OC1A=0b11/*In this case output will be on both OC1B and OC1A */

} TIMER1_OutputPinsNumber;
typedef struct
{
	uint16 initial_value;
	uint16 compare_value; /* it will be used in CTC & PWM mode*/
	TIMER1_Prescaler prescaler;
	TIMER1_Mode mode;
	TIMER1_OutputPinsNumber TIMER1_OutputPinsNumberValue; /* it will be used in CTC & PWM mode*/
	TIMER1_ctcModeOutput TIMER1_ctcModeOutputValueOnOC1A;/* it will be used in CTC & PWM mode*/
	TIMER1_ctcModeOutput TIMER1_ctcModeOutputValueOnOC1B;/* it will be used in CTC & PWM mode*/

} TIMER1_ConfigType; /*data structure to set timer1 dynamic configurations*/
/*************************************************/

/**********Extern Public Global Variable************/

/***************************************************/

/***************Function Prototypes For Public Functions**********************/
/*
 * Description :Function to set dynamic configurations of Timer1
 *	1)TIMER1 initial value
 *	2)TIMER1 compare value //in case of compare mode
 *	3)TIMER1 prescaler value
 *	4)TIMER1 operating mode
 *	5)TIMER1 output pins number ( OC1B only or (OC1A & OC1B) ) //in case of compare mode
 *	6)TIMER1 output pins value (Toggle/clear/set on compare) //in case of compare mode
 */
void TIMER1_init (const TIMER1_ConfigType *Config_Ptr);


/*
 * Description : Function that takes callback function address (set the function to be called at every interrupt)
 */
void TIMER1_setCallBackFunction (void (*callBackFunctionPtr) (void));

/*
 * Description : Function to stop TIMER1
 */

void TIMER1_deInit (void);
/****************************************************************************/

/***********Macros***********/

/****************************/

#endif /* TIMER1_H_ */
