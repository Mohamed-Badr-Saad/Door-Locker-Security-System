/*
 *    File name: Final_Project_CONTROL_ECU.c
 *   Created on: Nov 4, 2022
 *       Author: Mohamed Badr
 *  Description: Source file that controls all the functions performed by CONTROL_ECU(main.c)
 */

/*******Module header file**********/
#include "CONTROL_ECU_functions.h"
/***********************************/

/**********Other Modules header files used by this module*************/
#include "uart.h"
#include "twi.h"
#include "timer1.h"
#include<avr/io.h>

#include "../../HAL/include/buzzer.h"
#include "../../HAL/include/dc-motor.h"
/*****************************************************************/

int main (void)
{
	uint8 option, count;
	SREG |= (1 << 7);/*enable global interrupt*/
	/********UART dynamic configurations**********/
	UART_config UART_configurations;
	UART_setDynamicConfig(&UART_configurations);/*Set UART dynamic configurations*/
	UART_init(&UART_configurations);/*initialize UART*/
	/**********************************************/

	/**************TWI dynamic configurations************************/
	TWI_config TWI_configurations;
	TWI_setDynamicConfig(&TWI_configurations);
	TWI_init(&TWI_configurations);
	/****************************************************************/

	/*********TIMER1 dynamic configurations****************//*it will call the callback function every 1sec*/
	TIMER1_setDynamicConfig(&g_TIMER1_configurations);
	TIMER1_setCallBackFunction(tickIncrement);
	/*******************************************************/

	/*****initialize pins direction in the micro-controller to make the motor operate in the required state*****/
	DcMotor_init();
	/***********************************************************************************************************/

	/******initialize BUZZER pin direction******/
	BUZZER_init();
	/******************************************/

	UART_sendByte(CONTROL_ECU_READY);/*to inform HMI_ECU that CONTROL_ECU is ready to receive data*/

	doorLock_receiveAndStoreNewPassword();/*receive newly created password and store it in EEPROM*/

	while (1)
	{
		option = doorLock_receiveOption();/*receive the required option from HMI_ECU*/
		count = doorLock_3trialsPasswordCheck();/*give the user 3 trials only to enter the password correctly*/

		/***************in case the user entered the correct password in any of the 3 trials*****************/
		if (g_3trialsPasswordFlag)
		{
			g_3trialsPasswordFlag = PASSWORDS_NOT_IDENTICAL;
			switch (option)
			{
			case OPEN_DOOR :
				doorLock_openDoor();

				break;

			case CHANGE_PASSWORD :
				doorLock_receiveAndStoreNewPassword();/*receive newly created password and store it in EEPROM*/

				break;
			}
		}
		/**********************************************************************************************************/
		/********************in case the user entered a wrong password for more than 3 trials*******************/
		else
		{
			TIMER1_init(&g_TIMER1_configurations);
			BUZZER_on();
			delayInSec(60);/*delay for 1min*/
			TIMER1_deInit();
			BUZZER_off();
		}
		/***********************************************************************************************************/

	}
}

