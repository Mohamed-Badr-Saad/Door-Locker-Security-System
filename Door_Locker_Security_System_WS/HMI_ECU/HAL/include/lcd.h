/****************************************************
 * File name: LCD.h
 * Created on: Oct 3, 2022
 *     Author: Mohamed Badr
 *Description: Header file for LCD driver
 ****************************************************/

#ifndef LCD_H_
#define LCD_H_
/************Common Header Files***************/
#include"std_types.h"
#include <stdlib.h>
/**********************************************/

/*************Static Configurations***************/
#define LCD_DATA_BITS_MODE 8
#if ((LCD_DATA_BITS_MODE!=8) &&(LCD_DATA_BITS_MODE!=4))
#error "data bits mode must be equal to 4 or 8"
#endif

#define LCD_DATA_PORT_ID PORTB_ID
#if LCD_DATA_BITS_MODE ==4
#define LCD_DB4_PIN_ID PIN0_ID
#define LCD_DB5_PIN_ID PIN1_ID
#define LCD_DB6_PIN_ID PIN2_ID
#define LCD_DB7_PIN_ID PIN3_ID
#endif

#define LCD_RS_PORT_ID PORTC_ID
#define LCD_RS_PIN_ID PIN0_ID

#define LCD_RW_PORT_ID PORTC_ID
#define LCD_RW_PIN_ID PIN1_ID

#define LCD_E_PORT_ID PORTC_ID
#define LCD_E_PIIN_ID PIN2_ID

/***********************************************/
/************Definitions***************/
#define LCD_2LINES_8_BITS_MODE 0x38
#define LCD_2LINES_4_BITS_MODE 0x28
#define LCD_2LINES_4BITS_MODE_INIT1 0x33
#define LCD_2LINES_4BITS_MODE_INIT2 0x32
#define LCD_TURN_CURSOR_OFF 0x0C
#define LCD_CLEAR_SCREEN 0x01
#define LCD_SET_CURSOR_LOCATION 0x80
#define LCD_ROW1_START_ADDRESS 0x40
#define LCD_ROW2_START_ADDRESS 0x10
#define LCD_ROW3_START_ADDRESS 0x50
/**********************************************/
/***********User defined Data Types***************/

/*************************************************/

/**********Extern Public Global Variable************/

/*****************************************************/

/***************Function Prototypes For Public Functions**********************/
/*
 * Description : Function to initialize the LCD
 * 1-set the pin direction connected to RS,E and Data pins
 * 2-set the LCD data mode 4bits or 8bits
 */
void LCD_init (void);

/*
 * Description : Function to send a command to the LCD
 */
void LCD_sendCommand (uint8 command);

/*
 * Description : Function to display a character on the LCD
 */
void LCD_displayCharacter (uint8 character);

/*
 * Description : Function to display string on the LCD
 */
void LCD_diplaystring (const char *str);

/*
 * Description : Function to move the cursor to the required position
 */
void LCD_moveCursor (uint8 row , uint8 column);

/*
 * Description : Function to display a string on the required position
 */
void LCD_displayStringRowColumn (const char *str , uint8 row , uint8 column);

/*
 * Description : Function to clear the LCD
 */
void LCD_clearScreen (void);

/*
 * Description : Function to display an integer on the LCD
 */
void LCD_integerToString (int num);
/**********************************************************/

/***********Macros*************/

/****************************/
#endif /* LCD_H_ */
