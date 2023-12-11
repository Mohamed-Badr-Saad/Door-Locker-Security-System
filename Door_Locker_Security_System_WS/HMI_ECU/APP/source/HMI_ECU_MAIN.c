/*
 *    File name: Final_Project_HMI_ECU.c
 *   Created on: Oct 26, 2022
 *       Author: Mohamed Badr
 *  Description: Source file that controls all the functions performed by HMI_ECU(main.c)
 */

/*******Module header file**********/
#include "HMI_ECU_functions.h"
/***********************************/

/**********Other Modules header files used by this module*************/
#include "lcd.h"
#include "uart.h"
#include "timer1.h"
/*****************************************************************/

/**********Other header files used by this module*************/
#include <avr/io.h>
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

	/*********TIMER1 dynamic configurations****************//*it will call the callback function every 1sec*/
	TIMER1_setDynamicConfig(&g_TIMER1_configurations);
	TIMER1_setCallBackFunction(tickIncrement);
	/*******************************************************/

	/******LCD initialization*******/
	LCD_init();
	/*********************************/

	/****************wait until CONTROL_ECU is ready****************/
	while (UART_receiveByte() != CONTROL_ECU_READY)
	{
	}
	/****************************************************************/

	doorLock_createNewPassword();

	while (1)
	{

		option = doorLock_chooseOption();/*take the required option from user*/
		count = doorLock_3trialsPasswordCheck();/*give the user 3 trials only to enter the password correctly*/

		if (g_3trialsPasswordFlag)/*in case the user entered the correct password in any of the 3 trials*/
		{
			g_3trialsPasswordFlag = PASSWORDS_NOT_IDENTICAL;
			switch (option)
			{
			case OPEN_DOOR :
				doorLock_openDoor();

				break;

			case CHANGE_PASSWORD :
				doorLock_createNewPassword();

				break;
			}
		}
		else/*in case the user entered a wrong password for more than 3 trials*/
		{
			LCD_clearScreen();
			TIMER1_init(&g_TIMER1_configurations);
			LCD_diplaystring("Wrong Password");
			delayInSec(60);/*delay for 1min*/
			TIMER1_deInit();
			LCD_clearScreen();

		}

	}
}
