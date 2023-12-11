/*
 *    File name: usart.h
 *   Created on: Oct 11, 2022
 *       Author: Mohamed Badr
 *  Description: Header file for atmega32 UART driver
 */

#ifndef UART_H_
#define UART_H_
/************Common Header Files***************/
#include"std_types.h"
/**********************************************/

/*************Static Configurations***************/
#define UART_WITHOUT_UISNG_INTERRUPT
/*************************************************/

/************Definitions***************/
#define UART_U2X_BIT_ID (1)

#define UART_PARITY_START_BIT_ID (4)

#define UART_STOP_BITS_BIT_ID (3)

#define UART_N_BITS_MODE_START_BIT_ID (1)
/**************************************/

/***********User defined Data Types***************/
typedef enum
{
	UART_PARITY_OFF = 0, UART_PARITY_EVEN = 2, UART_PARITY_ODD = 3
} UART_parityType;

typedef enum
{
	UART_1_STOP_BIT, UART_2_STOP_BIT
} UART_stopBitsNum;

typedef enum
{
	UART_5_BIT_DATA_MODE = 0,
	UART_6_BIT_DATA_MODE = 1,
	UART_7_BIT_DATA_MODE = 2,
	UART_8_BIT_DATA_MODE = 3,
	UART_9_BIT_DATA_MODE = 7
} UART_nBitDataType;

typedef enum
{
	UART_singleSpeed, UART_doubleSpeed
} UART_transmissionSpeedType;

typedef struct
{
	UART_parityType UART_parity;
	UART_stopBitsNum UART_stopBits;
	UART_nBitDataType UART_nBitsDataMode;
	UART_transmissionSpeedType UART_transmissionSpeed;
	uint32 UART_baudRate;
} UART_config;
/*************************************************/

/**********Extern Public Global Variable************/

/***************************************************/

/***************Function Prototypes For Public Functions**********************/

/*
 * Description : Function to set the UART dynamic configurations and enable RX and TX
 * 1- UART operating mode (normal mode or double speed mode)
 * 2- parity bits type (disabled , even ,odd)
 * 3- the number of stop bits (1 bit or 2 bits)
 * 4- UART n-bits data mode (5-bits,6-bits,7-bits,8-bits,9-bits)
 * 5- set baud rate in UBRR register using an equation
 * 6- to enable RX and TX interrupts in case we use interrupts
 */
void UART_init (UART_config *UART_configurations);

/*
 * Description :Function that take a byte (data) and send it serially through UART (TX pin)
 */
void UART_sendByte (const uint8 data);

/*
 * Description :Function to send a string serially through the UART
 */
void UART_sendString (const uint8 *string);

/*
 * Description : Function that return a byte(data) received serially from UART (RX pin)
 */
uint8 UART_receiveByte (void);

/*
 * Description : Function to receive a string until '#' serially through the UART
 */
void UART_receiveString (uint8 *string);
/****************************************************************************/

/***********Macros***********/

/****************************/

#endif /* UART_H_ */
