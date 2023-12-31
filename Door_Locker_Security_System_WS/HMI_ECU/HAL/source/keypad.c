/*
 *    File name: keypad.c
 *   Created on: Oct 10, 2022
 *       Author: Mohamed Badr
 *  Description: Source file of the keypad driver
 */

/*******Module header file**********/
#include "keypad.h"
/***********************************/

/**********Other Modules header files used by this module*************/
#include "gpio.h"
/*****************************************************************/

/**********Other header files used by this module*************/
#include "common_macros.h"
#include <util/delay.h>
/*****************************************************************/

/*******Global variables***********/

/***********************************/

/*********Interrupt Service Routine**********/

/********************************************/

/***************Function Prototypes For private Functions**********************/
#if KEYPAD_STANDARD == 0

#if KEYPAD_COLUMNS_NUMBER == 3
static uint8 KEYPAD_4X3_adjustKeyNumber (uint8 buttonNum);
#endif

#if KEYPAD_COLUMNS_NUMBER == 4
static uint8 KEYPAD_4X4_adjustKeyNumber (uint8 buttonNum);
#endif

#endif
/******************************************************************************/

/*************************Functions definitions**************************/
/*
 * Description : Function to scan the keypad and return value of the pressed button
 */

uint8 KEYPAD_getPressedKey (void)
{
	uint8 row, column, buttonNum;

	/*************configure column pins as output pins*************/
	GPIO_setupPinDirection(KEYPAD_COLUMN_PORT_ID,
	KEYPAD_COLUMN_START_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COLUMN_PORT_ID,
	KEYPAD_COLUMN_START_PIN_ID + 1, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COLUMN_PORT_ID,
	KEYPAD_COLUMN_START_PIN_ID + 2, PIN_INPUT);
#if KEYPAD_COLUMNS_NUMBER == 4
	GPIO_setupPinDirection(KEYPAD_COLUMN_PORT_ID,
	KEYPAD_COLUMN_START_PIN_ID + 3, PIN_INPUT);
#endif
	/*****************************************************************/

	/*************configure row pins as output pins*************/
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,
	KEYPAD_ROW_START_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,
	KEYPAD_ROW_START_PIN_ID + 1, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,
	KEYPAD_ROW_START_PIN_ID + 2, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,
	KEYPAD_ROW_START_PIN_ID + 3, PIN_INPUT);
	/*****************************************************************/
	while (1)
	{
		for (row = 0 ; row < KEYPAD_ROWS_NUMBER ; row++)
		{
			/***********make the row output pin  and set or clear the row pin according to pull-up or pull-down***********/
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,
			KEYPAD_ROW_START_PIN_ID + row, PIN_OUTPUT);
			GPIO_writePin(KEYPAD_ROW_PORT_ID,
			KEYPAD_ROW_START_PIN_ID + row, KEYPAD_BUTTON_PRESSED);

			_delay_ms(60);/*To make Keypad work properly with F_CPU higher than 2Mhz*/

			/************************************************************************************************************/
			for (column = 0 ; column < KEYPAD_COLUMNS_NUMBER ; column++)
			{
				/*check for every column if the the button is pressed*/
				if (GPIO_readPin(KEYPAD_COLUMN_PORT_ID,
				KEYPAD_COLUMN_START_PIN_ID + column) == KEYPAD_BUTTON_PRESSED)
				{
					buttonNum = (row * KEYPAD_COLUMNS_NUMBER) + column + 1;
#if KEYPAD_STANDARD == 1 /*in case we want to return the button number */
					return buttonNum;
#else
#if KEYPAD_COLUMNS_NUMBER == 3
				return KEYPAD_4X3_adjustKeyNumber(buttonNum);
#elif KEYPAD_COLUMNS_NUMBER == 4
				return KEYPAD_4X4_adjustKeyNumber(buttonNum);
#endif
#endif
				}
			}
			/************make the pins row input again after checking this row***********/
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,
			KEYPAD_ROW_START_PIN_ID + row, PIN_INPUT);
			/************************************************************************/
		}

	}

}
#if KEYPAD_STANDARD == 0
/*
 * Description : Function to take the value of the keypad button number and return the keypad actual button value in case of 4x4 keypad
 */
#if KEYPAD_COLUMNS_NUMBER == 4
static uint8 KEYPAD_4X4_adjustKeyNumber (uint8 buttonNum)
{
	uint8 keypadButton = 0;
	switch (buttonNum)
	{
	case 1 :
		keypadButton = 7;
		break;
	case 2 :
		keypadButton = 8;
		break;
	case 3 :
		keypadButton = 9;
		break;
	case 4 :
		keypadButton = '%'; // ASCII Code of %
		break;
	case 5 :
		keypadButton = 4;
		break;
	case 6 :
		keypadButton = 5;
		break;
	case 7 :
		keypadButton = 6;
		break;
	case 8 :
		keypadButton = '*'; /* ASCII Code of '*' */
		break;
	case 9 :
		keypadButton = 1;
		break;
	case 10 :
		keypadButton = 2;
		break;
	case 11 :
		keypadButton = 3;
		break;
	case 12 :
		keypadButton = '-'; /* ASCII Code of '-' */
		break;
	case 13 :
		keypadButton = 13; /* ASCII of Enter */
		break;
	case 14 :
		keypadButton = 0;
		break;
	case 15 :
		keypadButton = '='; /* ASCII Code of '=' */
		break;
	case 16 :
		keypadButton = '+'; /* ASCII Code of '+' */
		break;
	default :
		keypadButton = buttonNum;
		break;
	}
	return keypadButton;
}
#endif

/*
 * Description : Function to take the value of the keypad button number and return the keypad actual button value in case of 4x3 keypad
 */
#if KEYPAD_COLUMNS_NUMBER == 3
static uint8 KEYPAD_4X3_adjustKeyNumber (uint8 buttonNum)
{
	uint8 keypadButton = 0;
	switch (buttonNum)
	{
	case 10 :
		keypadButton = '*';
		break;
	case 11 :
		keypadButton = 0;
		break;
	case 12 :
		keypadButton = '#';
		break;
	default :
		keypadButton = buttonNum;
	}

	return keypadButton;

}
#endif

#endif
/*****************************************************************/
