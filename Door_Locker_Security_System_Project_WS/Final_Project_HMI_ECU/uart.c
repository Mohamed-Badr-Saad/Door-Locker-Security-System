/*
 *    File name: usart.c
 *   Created on: Oct 11, 2022
 *       Author: Mohamed Badr
 *  Description: Source file for atmega32 UART driver
 */

/*******Module header file**********/
#include "uart.h"
/***********************************/

/**********Other Modules header files used by this module*************/

/*****************************************************************/

/**********Other header files used by this module*************/
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/*****************************************************************/

/*******Global variables***********/
#ifdef UART_UISNG_INTERRUPT
volatile static uint8 g_dataTx, g_dataRx, g_flagRx = 0, g_flagTx = 0;
#endif
/***********************************/

/*********Interrupt Service Routine**********/
#ifdef UART_UISNG_INTERRUPT
ISR(USART_UDRE_vect)/*when the data register is empty*/
{
	if (g_flagTx == 1)
	{
		UDR = g_dataTx;
		g_flagTx = 0;
	}
}

ISR(USART_RXC_vect)
{
	g_dataRx = UDR;
	g_flagRx = 1;
}
#endif
/********************************************/

/***************Function Prototypes For private Functions**********************/

/******************************************************************************/

/*************************Functions definitions**************************/
/*
 * Description : Function to set the UART dynamic configurations and enable RX and TX
 * 1- UART operating mode (normal mode or double speed mode)
 * 2- parity bits type (disabled , even ,odd)
 * 3- the number of stop bits (1 bit or 2 bits)
 * 4- UART n-bits data mode (5-bits,6-bits,7-bits,8-bits,9-bits)
 * 5- set baud rate in UBRR register using an equation
 * 6- to enable RX and TX interrupts in case we use interrupts
 */
void UART_init (UART_config *UART_configurations)
{
	uint16 UBRR_value = 0;
	/**************************************Set UART dynamic configurations**************************************/

	/*********set whether the UART operates in U2X(double transmission speed) or not********/
	UCSRA =
			(UCSRA & ~ (1 << UART_U2X_BIT_ID))
					| ( (UART_configurations->UART_transmissionSpeed)
							<< UART_U2X_BIT_ID);
	/********************************************************************/

	/*************Receiver enable and transmitter enable************/
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	/**************************************************************/

	/*****to use UCSRC *****/
	UCSRC |= (1 << URSEL);
	/************************/
	/****************Set the UART parity type***************/
	UCSRC = (UCSRC & ~ (0x03 << UART_PARITY_START_BIT_ID))
			| ( (UART_configurations->UART_parity) << UART_PARITY_START_BIT_ID);
	/*************************************************/

	/*************Set the UART stop bits number************/
	UCSRC = (UCSRC & ~ (1 << UART_STOP_BITS_BIT_ID))
			| ( (UART_configurations->UART_stopBits) << UART_STOP_BITS_BIT_ID);
	/*************************************************/

	/************Set the UART parity type**************/
	UCSRC = (UCSRC & ~ (0x03 << UART_N_BITS_MODE_START_BIT_ID))
			| ( (UART_configurations->UART_nBitsDataMode)
					<< UART_N_BITS_MODE_START_BIT_ID);
	/*************************************************/

	/***********in case of 9-bits data mode**********/
	if (UART_configurations->UART_nBitsDataMode == 9)
	{
		UCSRB |= (1 << UCSZ2);
	}
	/*************************************************/

	/********calculating UBBR in term of the baud rate************/
	if (UART_configurations->UART_transmissionSpeed == UART_singleSpeed)/*in case the UART operates in normal mode*/
	{
		UBRR_value = (uint16) ( (F_CPU
				/ (16 * UART_configurations->UART_baudRate)) - 1);
	}
	else if (UART_configurations->UART_transmissionSpeed == UART_doubleSpeed)/*in case the UART operates in U2X(double transmission speed)*/
	{
		UBRR_value = (uint16) ( (F_CPU
				/ (8 * UART_configurations->UART_baudRate)) - 1);
	}
	/*****************************************************************/

	/***********setting UBBR*********/
	UBRRL = UBRR_value;
	UBRRH = UBRR_value >> 8;
	/***********************************/
	/**************************************************************************************************************************/
#ifdef UART_UISNG_INTERRUPT
	UCSRB |= (1 << RXCIE) | (1 << UDRIE);/* Rx complete interrupt enable & data register empty interrupt enable*/
#endif
}

/*
 * Description :Function that take a byte (data) and send it serially through UART (TX pin)
 */
void UART_sendByte (const uint8 data)
{
#ifndef UART_UISNG_INTERRUPT
	while (BIT_IS_CLEAR(UCSRA, UDRE))
	{
	}
	UDR = data;
#endif

#ifdef UART_UISNG_INTERRUPT
	g_dataTx = data;
	g_flagTx = 1;
#endif

}

/*
 * Description :Function to send a string serially through the UART
 */
void UART_sendString (const uint8 *string)
{
	uint8 count;
	for (count = 0 ; string[count] != '\0' ; count++)
	{
		UART_sendByte(string[count]);
#ifdef UART_UISNG_INTERRUPT
		while (g_flagTx == 1)/*wait until the byte is sent*/
		{
		}
#endif
	}
}

/*
 * Description : Function that return a byte(data) received serially from UART (RX pin)
 */
uint8 UART_receiveByte (void)
{
#ifndef UART_UISNG_INTERRUPT
	while (BIT_IS_CLEAR(UCSRA, RXC))
	{
	}
	return UDR;
#endif

#ifdef UART_UISNG_INTERRUPT
	while (g_flagRx == 0)/*wait until the byte is received*/
	{
	}
	g_flagRx = 0;
	return g_dataRx;
#endif
}

/*
 * Description : Function to receive a string until '#' serially through the UART
 */
void UART_receiveString (uint8 *string)
{
	uint8 count = 0;
	string[count] = UART_receiveByte();
	while (string[count] != '#')
	{
		count++;
		string[count] = UART_receiveByte();
	}

	string[count] = '\0';

}
/*****************************************************************/
