/*
 *    File name: eeprom.h
 *   Created on: Oct 26, 2022
 *       Author: Mohamed Badr
 *  Description: Header file for external EEPROM(24C16) driver
 */

#ifndef EEPROM_H_
#define EEPROM_H_
/************Common Header Files***************/
#include "std_types.h"
/**********************************************/

/*************Static Configurations***************/

/*************************************************/

/************Definitions***************/
#define EEPROM_READ_BIT 1 /*in case we wand to read from EEPROM*/
#define EEPROM_WRITE1_BIT 0 /*in case we wand to write in EEPROM*/
#define EEPROM_FIXED_HIGH_BITS_ID (0b10100000) /*the fixed 4-bits ID in each EEPROM device in 24Cxx family*/
#define ERROR 0x00
#define SUCCESS 0xFF
/**************************************/

/***********User defined Data Types***************/

/*************************************************/

/**********Extern Public Global Variable************/

/***************************************************/

/***************Function Prototypes For Public Functions**********************/
/*
 * Description : Function to write a byte in specific location in the external EEPROM
 * it takes a byte(the required data to be written in EEPROM) and the byte location(11-bits) in the EEPROM
 * it return the state of operation whether it is succeeded or failed
 */
uint8 EEPROM_writeByte (uint8 data , uint16 address);

/*
 * Description : Function to read a byte from specific location in the external EEPROM
 * it takes a variable address where the received data will be stored and the byte location(11-bits) in the EEPROM
 * it return the state of operation whether it is succeeded or failed
 */
uint8 EEPROM_readByte (uint8 *data , uint16 address);

/****************************************************************************/

/***********Macros***********/

/****************************/


#endif /* EEPROM_H_ */
