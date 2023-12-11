/****************************************************
 * File name: LCD.c
 * Created on: Oct 3, 2022
 *     Author: Mohamed Badr
 *Description: Source file for LCD driver
 ****************************************************/

/*******Module header file**********/
#include "../../HAL/include/lcd.h"

#include"gpio.h"
/*****************************************************************/

/**********header files used by this module*************/
#include"common_macros.h"
#include<util/delay.h>
/*****************************************************************/

/*******Global variables***********/

/***********************************/

/*********Interrupt Service Routine**********/

/********************************************/

/***************Function Prototypes For private Functions**********************/

/******************************************************************************/

/*************************Functions definitions**************************/
/*
 * Description : Function to initialize the LCD
 * 1-set the pin direction connected to RS,E and Data pins
 * 2-set the LCD data mode 4bits or 8bits
 */
void LCD_init (void)
{
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID, LCD_E_PIIN_ID, PIN_OUTPUT); /* Enable LCD E=1 */
	GPIO_setupPinDirection(LCD_RW_PORT_ID, LCD_RW_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(LCD_RW_PORT_ID, LCD_RW_PIN_ID, LOGIC_LOW);/*To make LCD operate in write mode*/
#if LCD_DATA_BITS_MODE==8
	GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);
	_delay_ms(20);
	LCD_sendCommand(LCD_2LINES_8_BITS_MODE); //to operate in 2 lines 8-bits mode
#else /*in case of 2lines 4bits mode*/
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, PIN_OUTPUT);
	_delay_ms(20);
	/***commands to initialize 2line 4bits mode*****/
	LCD_sendCommand(LCD_2LINES_4BITS_MODE_INIT1);
	LCD_sendCommand(LCD_2LINES_4BITS_MODE_INIT2);
	/***********************************************/
	LCD_sendCommand(LCD_2LINES_4_BITS_MODE); //to operate in 2 lines 4-bits mode
#endif
	LCD_sendCommand(LCD_TURN_CURSOR_OFF); //to turn off the cursor
	LCD_sendCommand(LCD_CLEAR_SCREEN); //to clear the screen
}

/*
 * Description : Function to send a command to the LCD
 */
void LCD_sendCommand (uint8 command)
{
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_LOW); //RS=0 to send command(ex:clear the screen,shift the screen)
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_HIGH); /* Enable LCD E=1 */
	_delay_ms(1);

	/****************************write the command to the LCD****************************/
#if LCD_DATA_BITS_MODE==8
	GPIO_writePort(LCD_DATA_PORT_ID, command);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_LOW);
	_delay_ms(1);
#else /*in case of 2lines 4bits mode*/
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command, PIN4_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command, PIN5_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command, PIN6_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command, PIN7_ID));
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command, PIN0_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command, PIN1_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command, PIN2_ID));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command, PIN3_ID));
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_LOW);
	_delay_ms(1);
#endif
	/************************************************************************/

}

/*
 * Description : Function to display a character on the LCD
 */
void LCD_displayCharacter (uint8 character)
{
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_HIGH); //RS=1 to display a character
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_HIGH);
	_delay_ms(1);

	/****************************write a character to the LCD****************************/
#if LCD_DATA_BITS_MODE==8
	GPIO_writePort(LCD_DATA_PORT_ID, character);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_LOW);
	_delay_ms(1);
#else /*in case of 2lines 4bits mode*/
		GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(character, PIN4_ID));
		GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(character, PIN5_ID));
		GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(character, PIN6_ID));
		GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(character, PIN7_ID));
		_delay_ms(1);
		GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_LOW);
		_delay_ms(1);
		GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_HIGH);
		_delay_ms(1);
		GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(character, PIN0_ID));
		GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(character, PIN1_ID));
		GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(character, PIN2_ID));
		GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(character, PIN3_ID));
		_delay_ms(1);
		GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIIN_ID, LOGIC_LOW);
		_delay_ms(1);
	#endif
	/************************************************************************/
}

/*
 * Description : Function to display string on the LCD
 */
void LCD_diplaystring (const char *str)
{
	uint8 i;
	for (i = 0 ; str[i] != '\0' ; i++)
	{
		LCD_displayCharacter(str[i]);
	}
}

/*
 * Description : Function to move the cursor to the required position
 */
void LCD_moveCursor (uint8 row , uint8 column)
{

	uint8 LCD_cursorAddress;
	switch (row)
	{
	case 0 :
		LCD_cursorAddress = column;/*move the cursor to position at row=0,column=0*/
		break;
	case 1 :
		LCD_cursorAddress = column + LCD_ROW1_START_ADDRESS;/*move the cursor to position at row=1,column=0*/
		break;
	case 2 :
		LCD_cursorAddress = column + LCD_ROW2_START_ADDRESS;/*move the cursor to position at row=2,column=0*/
		break;
	case 3 :
		LCD_cursorAddress = column + LCD_ROW3_START_ADDRESS;/*move the cursor to position at row=3,column=0*/
		break;
	}
	LCD_sendCommand(LCD_cursorAddress | LCD_SET_CURSOR_LOCATION);
}

/*
 * Description : Function to display a string on the required position
 */
void LCD_displayStringRowColumn (const char *str , uint8 row , uint8 column)
{
	LCD_moveCursor(row, column);
	LCD_diplaystring(str);
}

/*
 * Description : Function to clear the LCD
 */
void LCD_clearScreen (void)
{
	LCD_sendCommand(LCD_CLEAR_SCREEN);
}

/*
 * Description : Function to display an integer on the LCD
 */
void LCD_integerToString (int num)
{
	char buffer[16];
	itoa(num, buffer, 10);/*C function to convert integer to its corresponding ASCII value, 10 for decimal*/
	LCD_diplaystring(buffer);
}
/************************************************************************************************************/

