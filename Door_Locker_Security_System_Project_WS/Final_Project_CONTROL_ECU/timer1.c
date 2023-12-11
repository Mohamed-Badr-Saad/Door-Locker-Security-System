/*
 *    File name: timer1.c
 *   Created on: Oct 26, 2022
 *       Author: Mohamed Badr
 *  Description: Source file for the ATmega32 TIMER1 driver
 */

/*******Module header file**********/
#include "timer1.h"
/***********************************/

/**********Other Modules header files used by this module*************/
#include "gpio.h"
/*****************************************************************/

/**********Other header files used by this module*************/
#include <avr/io.h>
#include <avr/interrupt.h>
/*****************************************************************/

/*******Global variables***********/
static volatile void (*g_TIMER1_callBackFunctionPtr) (void) = NULL_PTR; /*global pointer to function*/
/***********************************/

/*********Interrupt Service Routine**********/
ISR(TIMER1_OVF_vect)
{
	g_TIMER1_callBackFunctionPtr();/*Call the callback function at every interrupt(at every overflow)*/
}

ISR(TIMER1_COMPA_vect)/*in case TCNT1 matches OCR1A*/
{
	g_TIMER1_callBackFunctionPtr();/*Call the callback function at every interrupt(at every Compare match between TCNT1 & OCR1A)*/

}
/********************************************/

/***************Function Prototypes For private Functions**********************/

/******************************************************************************/

/*************************Functions definitions**************************/
/*
 * Description :Function to set dynamic configurations of Timer1
 *	1)TIMER1 initial value
 *	2)TIMER1 compare value //in case of compare mode
 *	3)TIMER1 prescaler value
 *	4)TIMER1 operating mode
 *	5)TIMER1 output pins number ( OC1B only or (OC1A & OC1B) ) //in case of compare mode
 *	6)TIMER1 output pins value (Toggle/clear/set on compare) //in case of compare mode
 */
void TIMER1_init (const TIMER1_ConfigType *Config_Ptr)
{
	switch (Config_Ptr->mode)
	{
	case TIMER1_NORMAL_MODE :
		/********************************************************************************************************
		 *  FOC1A=1 , FOC1B=1  ===> because we do not use PWM mode
		 * COM1A1=0 , COM1A0=0 , COM1B1=0 , COM1B0=0  ===> as we use normal mode,so OC1A & OC1B are disconnected
		 * WGM13=0 , WGM12=0 , WGM11=0 , WGM10=0  ===> Normal mode (overflow mode) - Top=0xFFFF(end/overflow value)
		 * CS12,CS11,CS10  ===> To set prescaler value according to dynamic configurations
		 * TCNT1 ===> has a value indicating when the timer will start its count(start value)
		 * TOIE1=1 ===> To enable TIMER1 overflow interrupt
		 *******************************************************************************************************/

		TCCR1A = (LOGIC_HIGH << FOC1A) | (LOGIC_HIGH << FOC1B);/*No PWM mode*//*Set WGM11=0 , WGM10=0 */
		TCCR1B = Config_Ptr->prescaler; /*Set prescaler value*//*Set WGM13=0 , WGM12=0 */
		TCNT1 = Config_Ptr->initial_value; /*Start value*/
		TIMSK = (LOGIC_HIGH << TOIE1);/*To enable overflow interrupt*/

		break;

	case TIMER1_CTC_MODE :
		/********************************************************************************************************
		 *  FOC1A=1 , FOC1B=1  ===> because we do not use PWM mode
		 * COM1A1,COM1A0,COM1B1,COM1B0  ===> To set OC1A/OC1B outputs in case of CTC mode according to dynamic configurations
		 * WGM13=0 , WGM12=1 , WGM11=0 , WGM10=0 ===> CTC mode and we will store the compare(Top) value OCR1A
		 * CS12,CS11,CS10  ===> To set prescaler value according to dynamic configurations
		 * TCNT1 ===> has a value indicating when the timer will start its count(start value)
		 * OCR1A ===> has a value indicating when the timer will stop its count(compare(Top) value)
		 * OCIE1A=1 ===> To generate interrupt at every compare match with OCR1A
		 *******************************************************************************************************/
		TCCR1A = (LOGIC_HIGH << FOC1A) | (LOGIC_HIGH << FOC1B);/*No PWM mode*//*Set WGM11=0 , WGM10=0 */
		TCCR1B = Config_Ptr->prescaler; /*Set prescaler value*/
		TCNT1 = Config_Ptr->initial_value; /*Start value*/
		TCCR1B = (TCCR1B & 0xE7) | (TIMER1_OUTPUT_PIN_OC1B_ONLY << 3);/*Set WGM13=0 , WGM12=1 ==> OCR1A=Top*/
		OCR1A = Config_Ptr->compare_value;/*Set compare(Top) value when an interrupt will occur*/
		TIMSK = (LOGIC_HIGH << OCIE1A);/*To generate interrupt at every compare match with OCR1A */

		switch (Config_Ptr->TIMER1_OutputPinsNumberValue)
		{
		case TIMER1_OUTPUT_PIN_OC1B_ONLY :/*In this case we can use ICU because we store the compare(Top) value in OCR1A register*/
			TCCR1A = (TCCR1A & 0xCF)
					| ( (Config_Ptr->TIMER1_ctcModeOutputValueOnOC1B) << 4);/*Set COM1B1,COM1B0 according to dynamic configurations*/

			GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);/*Set OC1B as output pin*/

			break;
		case TIMER1_OUTPUT_PINS_OC1B_OC1A :/*In this case we can not use ICU because we store the compare(Top) value in ICR1 register*/

			TCCR1A = (TCCR1A & 0x0F)
					| ( (Config_Ptr->TIMER1_ctcModeOutputValueOnOC1B) << 4)
					| ( (Config_Ptr->TIMER1_ctcModeOutputValueOnOC1A) << 6);
			GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);/*Set OC1B as output pin*/
			GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);/*Set OC1A as output pin*/
			break;
		default :
			break;
		}
		break;

	default :
		break;
	}
}

/*
 * Description : Function that takes callback function address (set the function to be called at every interrupt)
 */
void TIMER1_setCallBackFunction (void (*callBackFunctionPtr) (void))
{
	g_TIMER1_callBackFunctionPtr = callBackFunctionPtr;
}

/*
 * Description : Function to stop TIMER1
 */

void TIMER1_deInit (void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK &= 0b11000011;
}

/*****************************************************************/
