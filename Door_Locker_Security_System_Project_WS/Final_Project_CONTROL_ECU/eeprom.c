/*
 *    File name: eeprom.c
 *   Created on: Oct 26, 2022
 *       Author: Mohamed Badr
 *  Description: Source file for external EEPROM(24C16) driver
 */

/*******Module header file**********/
#include "eeprom.h"
/***********************************/

/**********Other Modules header files used by this module*************/
#include "twi.h"
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
 * Description : Function to write a byte in specific location in the external EEPROM
 * it takes a byte(the required data to be written in EEPROM) and the byte location(11-bits) in the EEPROM
 * it return the state of operation whether it is succeeded or failed
 */
uint8 EEPROM_writeByte (uint8 data , uint16 address)
{
	uint8 eepromDeviceAddress;

	/*****************************************************************************************************/
	TWI_start();/*send the start bit on TWI bus*/
	if (TWI_getStatus() != TWI_START)/*In case the start bit is not sent we return an error*/
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	/* Set the EEPROM device address fixed ID 0b1010
	 * Insert the highest 3 bits of EEPROM byte address in EEPROM device address
	 * Set the lowest bit = 0 in EEPROM device address to make the TWI activates write operation
	 * */
	eepromDeviceAddress = EEPROM_FIXED_HIGH_BITS_ID
			| (uint8) ( (address & 0x0700) >> 7);
	TWI_writeByte(eepromDeviceAddress);/*Send EEPROM device address on TWI bus*/

	/*in case master did not send the slave address or write bit or did not receive the ACK from the slave we return an error  */
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	TWI_writeByte((uint8) address);/*send the lowest 8-bits of EEPROM byte address*/
	if (TWI_getStatus() != TWI_MT_DATA_ACK)/*In case master did not send the byte or receive the ACK we return an error*/
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	TWI_writeByte(data);/*sends the required byte (to be written in EEPROM) on TWI bus */
	if (TWI_getStatus() != TWI_MT_DATA_ACK)/*In case master did not send the byte or receive the ACK we return an error*/
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	TWI_stop();/*send the stop bit on TWI bus*/
	/*****************************************************************************************************/

	return SUCCESS;/*Return SUCCESS in case every bit is sent correctly*/
}

/*
 * Description : Function to read a byte from specific location in the external EEPROM
 * it takes a variable address where the received data will be stored and the byte location(11-bits) in the EEPROM
 * it return the state of operation whether it is succeeded or failed
 */
uint8 EEPROM_readByte (uint8 *data , uint16 address)
{
	uint8 eepromDeviceAddress;

	/*****************************************************************************************************/
	TWI_start();/*send the start bit on TWI bus*/
	if (TWI_getStatus() != TWI_START)/*In case the start bit is not sent we return an error*/
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	/* Set the EEPROM device address fixed ID 0b1010
	 * Insert the highest 3 bits of EEPROM byte address in EEPROM device address
	 * Set the lowest bit = 0 in EEPROM device address to make the TWI activates write operation
	 * */
	eepromDeviceAddress = EEPROM_FIXED_HIGH_BITS_ID
			| (uint8) ( (address & 0x0700) >> 7);
	TWI_writeByte(eepromDeviceAddress);/*Send EEPROM device address on TWI bus*/

	/*in case master did not send the slave address or write bit or did not receive the ACK from the slave we return an error  */
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	TWI_writeByte((uint8) address);/*send the lowest 8-bits of EEPROM byte address*/
	if (TWI_getStatus() != TWI_MT_DATA_ACK)/*In case master did not send the byte or receive the ACK we return an error*/
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	TWI_start();/*send a repeated start bit on TWI bus*/
	if (TWI_getStatus() != TWI_REP_START)/*In case the repeated start bit is not sent we return an error*/
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	/* Set the EEPROM device address fixed ID 0b1010
	 * Insert the highest 3 bits of EEPROM byte address in EEPROM device address
	 * Set the lowest bit = 1 in EEPROM device address to make the TWI activates read operation
	 * */
	eepromDeviceAddress = EEPROM_FIXED_HIGH_BITS_ID
			| (uint8) ( (address & 0x0700) >> 7) | EEPROM_READ_BIT;
	TWI_writeByte(eepromDeviceAddress);/*Send EEPROM device address on TWI bus*/

	/*in case master did not send the slave address or read bit or did not receive the ACK from the slave we return an error  */
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	/* to receive only one byte from EEPROM
	 * we used NACK to prevent EEPROM(slave) from sending any more bytes */
	*data = TWI_readByteWithNACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)/*In case master did not receive the byte or send the NACK to the slave we return an error*/
		return ERROR;
	/*****************************************************************************************************/

	/*****************************************************************************************************/
	TWI_stop();/*send the stop bit on TWI bus*/
	/*****************************************************************************************************/

	return SUCCESS;

}
/*****************************************************************/
