/*
 *    File name: twi.h
 *   Created on: Oct 25, 2022
 *       Author: Mohamed Badr
 *  Description: Header file of TWI/I2C ATmega32 driver
 */

#ifndef TWI_H_
#define TWI_H_
/************Common Header Files***************/
#include "std_types.h"
/**********************************************/

/*************Static Configurations***************/
#define TWI_WITHOUT_INTERRUPT

/*************************************************/

/******************************************Definitions************************************************/

/************data transfer rates *************/
#define TWI_NORMAL_MODE_100K 100000UL  /* 100Kb/s */
#define TWI_FAST_MODE_400K 400000UL   /* 400Kb/s */
#define TWI_FAST_MODE_PLUS_1000K 1000000UL     /* 1Mb/s */
#define TWI_HIGH_SPEED_MODE_3400K 3400000UL   /* 3.4Mb/s */
/**********************************************/

/************** I2C Status Bits in the TWSR Register *********************/
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */
/****************************************************************/

/*********************************************************************************************************/

/***********User defined Data Types***************/
typedef enum
{
	TWI_F_CPU_1, TWI_F_CPU_4, TWI_F_CPU_16, TWI_F_CPU_64
} TWI_prescaler; /*prescaler values*/

typedef enum{
	TWI_OFF,TWI_ON
}TWI_generalCallRecognitionState;

typedef struct
{
	TWI_prescaler TWI_prescalerValue;
	uint32 TWI_bitRateValue;
	uint8 TWI_deviceAddress;
	TWI_generalCallRecognitionState TWI_generalCallRecognitionStateValue;
} TWI_config;

/*************************************************/

/**********Extern Public Global Variable************/

/***************************************************/

/***************Function Prototypes For Public Functions**********************/
/*
 * Description : Function to initialize TWI with its dynamic configurations
 * 1- TWI device address
 * 2- Make General Call Recognition State ON or OFF
 * 3- Set TWI prescaler value
 * 4- Set TWI bit rate register
 * 5- Enable TWI
 */
void TWI_init (TWI_config *TWI_configurations);

/*
 * Description :Function to write the start bit on SDA(serial data line) in TWI bus
 */
void TWI_start(void);

/*
 * Description :Function to write the stop bit on SDA(serial data line) in TWI bus
 */
void TWI_stop(void);

/*
 * Description :Function to write a byte on SDA(serial data line) in TWI bus
 */
void TWI_writeByte (uint8 data);

/*
 * Description :Function to read a byte from SDA(serial data line) in TWI bus and write an Acknowledge on SDA
 */
uint8 TWI_readByteWithACK(void);

/*
 * Description :Function to read a byte from SDA(serial data line) in TWI bus only
 * and it does not send ACK bit indicating that the receiver does not want to receive more(like we send a NACK)
 */
uint8 TWI_readByteWithNACK(void);

/*
 * Description :Function to return the status of TWI logic and TWI serial bus
 */
uint8 TWI_getStatus (void);

/****************************************************************************/

/***********Macros***********/

/****************************/

#endif /* TWI_H_ */
