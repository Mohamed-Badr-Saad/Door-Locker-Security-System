/*
 *    File name: twi.c
 *   Created on: Oct 25, 2022
 *       Author: Mohamed Badr
 *  Description: Source file of TWI/I2C ATmega32 driver
 */

/*******Module header file**********/
#include "twi.h"
/***********************************/

/**********Other Modules header files used by this module*************/
/*****************************************************************/

/**********Other header files used by this module*************/
#include <avr/io.h>
#include "common_macros.h"

#ifdef TWI_WITH_INTERRUPT
#include <avr/interrupt.h>
#endif
/*****************************************************************/

/*******Global variables***********/
uint8 g_TWI_jobCompleted=0;
/***********************************/

/*********Interrupt Service Routine**********/
#ifdef TWI_WITH_INTERRUPT
ISR(TWI_vect)
{
	g_TWI_jobCompleted=1;
}
#endif
/********************************************/

/***************Function Prototypes For private Functions**********************/
static uint32 power (uint16 base , uint16 exponent);
/******************************************************************************/

/*************************Functions definitions**************************/
/*
 * Description : Function to initialize TWI with its dynamic configurations
 * 1- TWI device address
 * 2- Make General Call Recognition State ON or OFF
 * 3- Set TWI prescaler value
 * 4- Set TWI bit rate register
 * 5- Enable TWI
 */
void TWI_init (TWI_config *TWI_configurations)
{
#ifdef TWI_WITH_INTERRUPT
	SET_BIT(TWCR, TWIE);/*enable TWI interrupt*/
#endif

	/***********to give the device an address (7-bits only)***********/
	TWAR = (TWAR & 0x00) | ( (TWI_configurations->TWI_deviceAddress) << 1);
	/***********************************************************************/

	/******************to set General Call Recognition State************************/
	TWAR = (TWAR & 0xFE)
			| ( (TWI_configurations->TWI_generalCallRecognitionStateValue)
					& 0x01);
	/*********************************************************************************/

	/**********Set TWI prescaler value*************/
	TWSR = TWI_configurations->TWI_prescalerValue;
	/******************************************/

	/******************Set TWI bit rate register**************************************/
	TWBR = ( (F_CPU / TWI_configurations->TWI_bitRateValue) - 16)
			/ (float) (2 * power(4, TWI_configurations->TWI_prescalerValue));
	/*********************************************************************************/

	/*******enable TWI*******/
	SET_BIT(TWCR, TWEN);
	/**********************/
}

/*
 * Description :Function to write the start bit on SDA(serial data line) in TWI bus
 */
void TWI_start (void)
{
	/**********************************************************************/
	/* To clear TWI interrupt flag (to start the operations of the TWI)
	 * To set the TWI start condition bit
	 * To enable TWI
	 * */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	/*********************************************************************/
#ifdef TWI_WITH_INTERRUPT
	while(g_TWI_jobCompleted==0)/*wait until start bit is transmitted*/
	{
	}
	g_TWI_jobCompleted=0;
#else
	while (BIT_IS_CLEAR(TWCR, TWINT))/*wait until start bit is transmitted*/
	{
	}
#endif

	/* we do not need to clear the TWI interrupt flag
	 * because we clear it at the beginning of every operation(start,stop,transmit,receive,etc)
	 */
}

/*
 * Description :Function to write the stop bit on SDA(serial data line) in TWI bus
 */
void TWI_stop (void)
{
	/**********************************************************************/
	/* To clear TWI interrupt flag (to start the operations of the TWI)
	 * To set the TWI stop condition bit
	 * To enable TWI
	 * */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	/*********************************************************************/

	/***************************************************************************************************/
	/*it is not necessary to so this step because I(the device using TWI bus) do not need the bus any more when I send the stop bit*/
/*#ifdef TWI_WITH_INTERRUPT
	while(g_TWI_jobCompleted==0)//wait until stop bit is transmitted
	{
	}
	g_TWI_jobCompleted=0;
#else
	while (BIT_IS_CLEAR(TWCR, TWINT))   //wait until stop bit is transmitted
	{
	}
#endif*/
	/***************************************************************************************************/

	/* we do not need to clear the TWI interrupt flag
	 * because we clear it at the beginning of every operation(start,stop,transmit,receive,etc)
	 */
}

/*
 * Description :Function to write a byte on SDA(serial data line) in TWI bus
 */
void TWI_writeByte (uint8 data)
{
	/*******put the data in TWI data register*********/
	TWDR = data;
	/********************************************/

	/**********************************************************************/
	/* To clear TWI interrupt flag (to start the operations of the TWI)
	 * To enable TWI
	 * */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/*********************************************************************/
#ifdef TWI_WITH_INTERRUPT
	while(g_TWI_jobCompleted==0)/*wait until the byte is transmitted*/
	{
	}
	g_TWI_jobCompleted=0;
#else
	while (BIT_IS_CLEAR(TWCR, TWINT))/*wait until the byte is transmitted*/
	{
	}
#endif

}

/*
 * Description :Function to read a byte from SDA(serial data line) in TWI bus and write an Acknowledge on SDA
 */
uint8 TWI_readByteWithACK (void)
{
	/**********************************************************************/
	/* To clear TWI interrupt flag (to start the operations of the TWI)
	 * To set the TWI Acknowledge enable bit (when TWEA=1 then it sends ACK bit when the data is received)
	 * To enable TWI
	 * */
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
	/*********************************************************************/
#ifdef TWI_WITH_INTERRUPT
	while(g_TWI_jobCompleted==0)/*wait until a byte is received*/
	{
	}
	g_TWI_jobCompleted=0;
#else
	while (BIT_IS_CLEAR(TWCR, TWINT))/*wait until a byte is received*/
	{
	}
#endif
	/* we do not need to clear the TWI interrupt flag
	 * because we clear it at the beginning of every operation(start,stop,transmit,receive,etc)
	 */

	return TWDR;/*return the received byte*/
}

/*
 * Description :Function to read a byte from SDA(serial data line) in TWI bus only
 * and it does not send ACK bit indicating that the receiver does not want to receive more(like we send a NACK)
 */
uint8 TWI_readByteWithNACK (void)
{
	/**********************************************************************/
	/* To clear TWI interrupt flag (to start the operations of the TWI)
	 * To enable TWI
	 * */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/*********************************************************************/
#ifdef TWI_WITH_INTERRUPT
	while(g_TWI_jobCompleted==0)/*wait until a byte is received*/
	{
	}
	g_TWI_jobCompleted=0;
#else
	while (BIT_IS_CLEAR(TWCR, TWINT))/*wait until a byte is received*/
	{
	}
#endif
	/* we do not need to clear the TWI interrupt flag
	 * because we clear it at the beginning of every operation(start,stop,transmit,receive,etc)
	 */

	return TWDR;/*return the received byte*/
}

/*
 * Description :Function to return the status of TWI logic and TWI serial bus
 */
uint8 TWI_getStatus (void)
{
	uint8 status;
	status = TWSR & 0b11111000;/*to mask prescaler bits*/
	return status;
}

/*
 * Description :Function calculate the power value and it takes the base value and the exponent value
 */
static uint32 power (uint16 base , uint16 exponent)
{
	uint32 result = 1;
	uint8 i;
	for (i = 0 ; i < exponent ; i++)
	{
		result *= base;
	}
	return result;
}
/*****************************************************************/
