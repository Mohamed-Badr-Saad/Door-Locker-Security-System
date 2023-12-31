/*
 *    File name: HMI_ECU_functions.c
 *   Created on: Nov 6, 2022
 *       Author: Mohamed Badr
 *  Description: Source file for HMI_ECU functions and configurations
 */

/*******Module header file**********/
#include "HMI_ECU_functions.h"
/***********************************/

/**********Other Modules header files used by this module*************/
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer1.h"
/*****************************************************************/

/**********Other header files used by this module*************/
#include <util/delay.h>
#include <avr/io.h>
/*****************************************************************/

/*******Global variables***********/
TIMER1_ConfigType g_TIMER1_configurations;
uint8 g_3trialsPasswordFlag = PASSWORDS_NOT_IDENTICAL;/*in case g_3trialsPasswordFlag=1 then the 2 passwords are identical */
volatile uint8 g_tick = 0;
/***********************************/

/*********Interrupt Service Routine**********/

/********************************************/

/***************Function Prototypes For private Functions**********************/

/******************************************************************************/

/*************************Functions definitions**************************/

/*
 * Description : Function to set UART dynamic configurations
 */
void UART_setDynamicConfig (UART_config *UART_configurations)
{
	/********UART dynamic configurations**********/
	UART_configurations->UART_baudRate = 9600;
	UART_configurations->UART_nBitsDataMode = UART_8_BIT_DATA_MODE;
	UART_configurations->UART_parity = UART_PARITY_OFF;
	UART_configurations->UART_stopBits = UART_1_STOP_BIT;
	UART_configurations->UART_transmissionSpeed = UART_doubleSpeed;
	/**********************************************/
}

/*
 * Description : Function to set TIMER1 dynamic configurations
 */
void TIMER1_setDynamicConfig (TIMER1_ConfigType *TIMER1_configurations)
{
	TIMER1_configurations->mode = TIMER1_CTC_MODE;
	TIMER1_configurations->initial_value = 0;
	TIMER1_configurations->compare_value = 7812;
	TIMER1_configurations->prescaler = TIMER1_F_CPU_1024;
	TIMER1_configurations->TIMER1_OutputPinsNumberValue = 0;
	TIMER1_configurations->TIMER1_ctcModeOutputValueOnOC1A =
			TIMER1_CTC_MODE_NO_OUTPUT;
	TIMER1_configurations->TIMER1_ctcModeOutputValueOnOC1B =
			TIMER1_CTC_MODE_NO_OUTPUT;
}

/*
 * Description : Function to
 * 1) get the password from keypad(user)
 * 2) send the password to CONTROL_ECU
 */
void doorLock_sendPassword (void)
{
	uint8 count = 0, transmittedPass;

	for (count = 0 ; count < PASSWORD_SIZE ; count++)
	{
		do
		{
			transmittedPass = KEYPAD_getPressedKey();/*take an input from keypad*/
		} while (! (transmittedPass >= 0 && transmittedPass <= 9));/*In case the user clicks on a wrong button ==> wait for another input*/

		LCD_displayCharacter('*');
		UART_sendByte(transmittedPass);/*send the password byte by byte through UART*/
		_delay_ms(CLICK_PERIOD_IN_SEC);/*any click for 0.5sec will be counted as one click*/
	}

	do/*wait for enter button*/
	{
		transmittedPass = KEYPAD_getPressedKey();/*take an input from keypad*/
		_delay_ms(CLICK_PERIOD_IN_SEC);/*all clicks for 0.5sec will be counted as one click*/

	} while (transmittedPass != 13);/*ASCII of enter*/
	UART_sendByte(transmittedPass);/*send enter to make CONTROL_ECU know that the user finished entering the password*/

}

/*
 * Description : Function to wait until the user enters the required option and send this option to CONTROL_ECU
 * and return the option to HMI_ECU as well
 */
uint8 doorLock_chooseOption (void)
{
	uint8 option = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn("+ : Open Door", 0, 0);
	LCD_displayStringRowColumn("- : Change Pass", 1, 0);
	do
	{
		option = KEYPAD_getPressedKey();
	} while (option != '+' && option != '-');/*In case the user clicks on a wrong button ==> wait for another input*/
	UART_sendByte(option);/*send the required option to CONTROL_ECU*/
	return option;
}

/*
 * Description : Function to increment g_tick every compare match [callback function]
 */
void tickIncrement (void)
{
	g_tick++;
}

/*
 * Description : Function to make a delay for specific time in seconds
 */
void delayInSec (uint16 timeInSec)
{
	/*************wait for timeInSec***********/
	while (g_tick < timeInSec)
	{
	}
	g_tick = 0;
	/*****************************************/
}

/*
 * Description : Function to display the door state
 */
void doorLock_openDoor (void)
{
	TIMER1_init(&g_TIMER1_configurations);/*it will call the callback function every 1sec*/
	LCD_clearScreen();
	LCD_diplaystring("Door is Unlocking");
	delayInSec(15);/*wait for 15sec*/
	LCD_clearScreen();
	LCD_diplaystring("Door is Opened");
	delayInSec(3);/*hold for 3sec*/
	LCD_clearScreen();
	LCD_diplaystring("Door is Locking ");
	delayInSec(15);/*wait for 15sec*/
	TIMER1_deInit();
}

/*
 * Description : Function to create new password
 */
void doorLock_createNewPassword (void)
{
	/****************************repeat this loop in case the entered 2 passwords are not identical****************************/
	do
	{
		LCD_clearScreen();
		LCD_diplaystring("plz enter pass:");
		LCD_moveCursor(1, 0);
		doorLock_sendPassword();/*send the password for the first time*/
		LCD_displayStringRowColumn("plz re-enter the", 0, 0);
		LCD_displayStringRowColumn("same pass: ", 1, 0);
		doorLock_sendPassword();/*send the password for the second time*/
	} while (UART_receiveByte() == PASSWORDS_NOT_IDENTICAL);/*receive the state of comparing 2 passwords from CONTROL_ECU*/
	/************************************************************************************************************************/
	/****************wait until CONTROL_ECU is ready****************/
	while (UART_receiveByte() != CONTROL_ECU_READY)
	{
	}
	/****************************************************************/
}

/*
 * Description : Function to check the entered password(for the third time) in order to do specific option
 * in case the user failed to enter the password correctly in the 3 trials then we have to return to the main options
 */
uint8 doorLock_3trialsPasswordCheck (void)
{
	uint8 count, passwordState = DUMMY_VALUE;
	/* we make the fourth loop
	 * in order to check the password that it is entered for the third time*/
	for (count = 0 ; count <= NUMBER_OF_PASSWORD_TRIALS ; count++)
	{
		if (passwordState == PASSWORDS_IDENTICAL)
		{
			g_3trialsPasswordFlag = PASSWORDS_IDENTICAL;
			break;
		}
		else if (count < 3)/*in case the 2 passwords are not identical and count<3*/
		{
			LCD_clearScreen();
			LCD_diplaystring("plz enter pass:");
			LCD_moveCursor(1, 0);
			doorLock_sendPassword();/*send the password to CONTROL_ECU*/
			passwordState = UART_receiveByte();
		}
	}
	return g_3trialsPasswordFlag;
}

/*****************************************************************/
