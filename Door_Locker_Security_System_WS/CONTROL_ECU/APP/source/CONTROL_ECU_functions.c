/*
 *    File name: CONTROL_ECU_functions.c
 *   Created on: Nov 6, 2022
 *       Author: Mohamed Badr
 *  Description: Source file for CONTROL_ECU functions and configurations
 */

/*******Module header file**********/
#include "CONTROL_ECU_functions.h"
/***********************************/

/**********Other Modules header files used by this module*************/
#include "twi.h"
#include "uart.h"
#include "timer1.h"
#include<avr/io.h>

#include <util/delay.h>

#include "../../HAL/include/buzzer.h"
#include "../../HAL/include/dc-motor.h"
#include "../../HAL/include/eeprom.h"
/*****************************************************************/

/*******Global variables***********/
uint8 g_passwordArray[PASSWORD_SIZE_PLUS_ENTER] = { 0 },
		g_repeatedPassword[PASSWORD_SIZE_PLUS_ENTER] = { 0 };
TIMER1_ConfigType g_TIMER1_configurations;
volatile uint8 g_tick = 0;
uint8 g_3trialsPasswordFlag = PASSWORDS_NOT_IDENTICAL;/*in case g_3trialsPasswordFlag=1 then the 2 passwords are identical */
/***********************************/

/*********Interrupt Service Routine**********/

/********************************************/

/***************Function Prototypes For private Functions**********************/

/******************************************************************************/

/*************************Functions definitions**************************/

/*
 * Description : Function to set UART dynamic configurations
 */
void UART_setDynamicConfig (UART_config *UART_configurations)
{
	/********UART dynamic configurations**********/
	UART_configurations->UART_baudRate = 9600;
	UART_configurations->UART_nBitsDataMode = UART_8_BIT_DATA_MODE;
	UART_configurations->UART_parity = UART_PARITY_OFF;
	UART_configurations->UART_stopBits = UART_1_STOP_BIT;
	UART_configurations->UART_transmissionSpeed = UART_doubleSpeed;
	/**********************************************/
}

/*
 * Description : Function to set TWI dynamic configurations
 */
void TWI_setDynamicConfig (TWI_config *TWI_configurations)
{
	TWI_configurations->TWI_bitRateValue = TWI_NORMAL_MODE_100K;
	TWI_configurations->TWI_prescalerValue = TWI_F_CPU_1;
	TWI_configurations->TWI_deviceAddress = 1;/*this MCU will have an address 0x01*/
	TWI_configurations->TWI_generalCallRecognitionStateValue = TWI_OFF;
}

/*
 * Description : Function to set TIMER1 dynamic configurations
 */
void TIMER1_setDynamicConfig (TIMER1_ConfigType *TIMER1_configurations)
{
	TIMER1_configurations->mode = TIMER1_CTC_MODE;
	TIMER1_configurations->initial_value = 0;
	TIMER1_configurations->compare_value = 7812;
	TIMER1_configurations->prescaler = TIMER1_F_CPU_1024;
	TIMER1_configurations->TIMER1_OutputPinsNumberValue = 0;
	TIMER1_configurations->TIMER1_ctcModeOutputValueOnOC1A =
			TIMER1_CTC_MODE_NO_OUTPUT;
	TIMER1_configurations->TIMER1_ctcModeOutputValueOnOC1B =
			TIMER1_CTC_MODE_NO_OUTPUT;
}

/*
 * Description : Function to receive the password from CONTROL_ECU
 */
void doorLock_receivePassword (uint8 *receivedPass)
{
	uint8 count;
	/* we loop until password size +1
	 * to receive enter button to make CONTROL_ECU know that the user finished entering the password*/
	for (count = 0 ; count < PASSWORD_SIZE_PLUS_ENTER ; count++)
	{
		receivedPass[count] = UART_receiveByte();/*receive the password from HMI_ECU byte by byte*/
	}
}

/*
 * Description : Function to check whether the 2 passwords are identical or not in case of creating a new password
 */
uint8 doorLock_passwordFirstCheck (void)
{
	uint8 count, passwordState = PASSWORDS_IDENTICAL;
	for (count = 0 ; count < PASSWORD_SIZE ; count++)
	{
		if (g_passwordArray[count] != g_repeatedPassword[count])/*in case the 2 passwords are not identical*/
		{
			passwordState = PASSWORDS_NOT_IDENTICAL;
			break;
		}
	}

	UART_sendByte(passwordState);/*send the state of comparing 2 passwords to HMI_ECU*/
	return passwordState;

}
/*
 * Description : Function to save the password in EEPROM
 */
void doorLock_savePassword (void)
{
	uint8 count, eepromAdress = 0;

	for (count = 0 ; count < PASSWORD_SIZE ; count++)
	{
		EEPROM_writeByte(g_passwordArray[count], eepromAdress);/*save the password to EEPROM*/
		_delay_ms(10);/*between every frame on TWI bus we have to wait 10ms*/
		eepromAdress++;
	}
}

/*
 * Description : Function to receive the required option from HMI_ECU
 */
uint8 doorLock_receiveOption (void)
{
	uint8 option;
	option = UART_receiveByte();
	return option;
}
/*
 * Description : Function to check whether the saved password in EEPROM is identical to the received password or not
 */
uint8 doorLock_passwordSecondCheck (void)
{
	uint8 count, eepromAdress = 0, eepromData, passwordState =
	PASSWORDS_IDENTICAL;
	for (count = 0 ; count < PASSWORD_SIZE ; count++)
	{
		EEPROM_readByte(&eepromData, eepromAdress);
		if (g_passwordArray[count] != eepromData)
		{
			passwordState = PASSWORDS_NOT_IDENTICAL;
			break;
		}
		eepromAdress++;
		_delay_ms(10);
	}
	UART_sendByte(passwordState);/*sends the state of the password to HMI_ECU*/
	return passwordState;
}

/*
 * Description : Function to increment g_tick every compare match [callback function]
 */
void tickIncrement (void)
{
	g_tick++;
}

/*
 * Description : Function to make a delay for specific time in seconds
 */
void delayInSec (uint16 timeInSec)
{
	/*************wait for timeInSec***********/
	while (g_tick < timeInSec)
	{
	}
	g_tick = 0;
	/*****************************************/
}

/*
 * Description : Function to open the door
 */
void doorLock_openDoor (void)
{
	TIMER1_init(&g_TIMER1_configurations);/*it will call the callback function every 1sec*/
	DcMotor_Rotate(DC_MOTOR_CLOCKWISE, DC_MOTOR_FULL_SPEED);
	delayInSec(15);/*wait for 15sec*/
	DcMotor_Rotate(DC_MOTOR_STOP, DC_MOTOR_STOP);
	delayInSec(3);/*hold for 3sec*/
	DcMotor_Rotate(DC_MOTOR_ANTI_CLOCKWISE, DC_MOTOR_FULL_SPEED);
	delayInSec(15);/*wait for 15sec*/
	DcMotor_Rotate(DC_MOTOR_STOP, DC_MOTOR_STOP);

	TIMER1_deInit();
}

/*
 * Description : Function to receive newly created password and store it in EEPROM
 */
void doorLock_receiveAndStoreNewPassword (void)
{
	/****************************repeat this loop in case the entered 2 passwords are not identical****************************/
	do
	{
		doorLock_receivePassword(g_passwordArray);/*receive the password from HMI_ECU for the first time*/
		doorLock_receivePassword(g_repeatedPassword);/*receive the password from HMI_ECU for the second time*/
	} while (doorLock_passwordFirstCheck() == PASSWORDS_NOT_IDENTICAL);
	/************************************************************************************************************************/
	/******In case the 2 passwords are identical CONTROL_ECU will save the password in EEPROM******/
	doorLock_savePassword();
	/*************************************************/
	UART_sendByte(CONTROL_ECU_READY);/*to inform HMI_ECU that CONTROL_ECU is ready to receive data*/
}

/*
 * Description : Function to check the entered password(for the third time) in order to do specific option
 * in case the user failed to enter the password correctly in the 3 trials then we have to return to the main options
 */
uint8 doorLock_3trialsPasswordCheck (void)
{
	uint8 count, passwordState = DUMMY_VALUE;

	/* we make the fourth loop
	 * in order to check the password that it is entered for the third time*/
	for (count = 0 ; count <= NUMBER_OF_PASSWORD_TRIALS ; count++)
	{
		if (passwordState == PASSWORDS_IDENTICAL)
		{
			g_3trialsPasswordFlag = PASSWORDS_IDENTICAL;
			break;
		}
		else if (count < 3)/*in case the 2 passwords are not identical and count<3*/
		{
			doorLock_receivePassword(g_passwordArray);/*receive the password from HMI_ECU*/
			passwordState = doorLock_passwordSecondCheck();/*check if the saved password in EEPROM is identical to received password*/
		}
	}
	return g_3trialsPasswordFlag;
}
/*****************************************************************/
