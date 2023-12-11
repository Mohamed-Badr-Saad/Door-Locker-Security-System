/*
 *    File name: buzzer.c
 *   Created on: Nov 3, 2022
 *       Author: Mohamed Badr
 *  Description: Source file for BUZZER driver
 */

/*******Module header file**********/
#include "buzzer.h"
/***********************************/

/**********Other Modules header files used by this module*************/
#include "gpio.h"
/*****************************************************************/

/**********Other header files used by this module*************/

/*****************************************************************/

/*******Global variables***********/

/***********************************/

/*********Interrupt Service Routine**********/

/********************************************/

/***************Function Prototypes For private Functions**********************/

/******************************************************************************/

/*************************Functions definitions**************************/
/*
 * Description : Function to set the pin direction that is connected to the BUZZER
 */
void BUZZER_init (void)
{
	/*****Setup the direction for the buzzer pin as output pin through the GPIO driver******/
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	/**************************************************************************************/
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_OFF);/*Turn off the buzzer through the GPIO*/

}

/*
 * Description : Function to turn on the BUZZER
 */
void BUZZER_on(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_ON);/*Write 1 on the BUZZER pin to make it ON*/
}

/*
 * Description : Function to turn off the BUZZER
 */
void BUZZER_off(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_OFF);/*Write 0 on the BUZZER pin to make it OFF*/

}
/*****************************************************************/
