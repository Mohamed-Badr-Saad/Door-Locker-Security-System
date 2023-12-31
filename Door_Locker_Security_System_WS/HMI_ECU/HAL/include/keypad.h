/*
 *    File name: keypad.h
 *   Created on: Oct 10, 2022
 *       Author: Mohamed Badr
 *  Description: Header file of the keypad driver
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_
/************Common Header Files***************/
#include "std_types.h"
/**********************************************/

/*************Static Configurations***************/
#define KEYPAD_STANDARD 0

#define KEYPAD_COLUMNS_NUMBER 4
#define KEYPAD_ROWS_NUMBER 4

#define KEYPAD_ROW_PORT_ID PORTA_ID
#define KEYPAD_ROW_START_PIN_ID PIN0_ID

#define KEYPAD_COLUMN_PORT_ID PORTA_ID
#define KEYPAD_COLUMN_START_PIN_ID PIN4_ID

#define KEYPAD_BUTTON_PRESSED LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED LOGIC_HIGH

/*************************************************/

/************Definitions***************/

/**************************************/

/***********User defined Data Types***************/

/*************************************************/

/**********Extern Public Global Variable************/

/***************************************************/

/***************Function Prototypes For Public Functions**********************/
/*
 * Description : Function to scan the keypad and return value of the pressed button
 */
uint8 KEYPAD_getPressedKey (void);
/****************************************************************************/

/***********Macros***********/

/****************************/

#endif /* KEYPAD_H_ */
