/*
 *    File name: CONTROL_ECU_functions.h
 *   Created on: Nov 6, 2022
 *       Author: Mohamed Badr
 *  Description: Header file for CONTROL_ECU functions and configurations
 */

#ifndef CONTROL_ECU_FUNCTIONS_H_
#define CONTROL_ECU_FUNCTIONS_H_
/************Common Header Files***************/
#include "std_types.h"
#include "timer1.h"
#include "twi.h"
#include "uart.h"
/**********************************************/

/*************Static Configurations***************/

/*************************************************/

/************Definitions***************/
#define PASSWORD_SIZE 5
#define PASSWORD_SIZE_PLUS_ENTER 1+PASSWORD_SIZE
#define PASSWORDS_IDENTICAL 1
#define PASSWORDS_NOT_IDENTICAL 0
#define CONTROL_ECU_READY 0xFF
#define OPEN_DOOR '+'
#define CHANGE_PASSWORD '-'
#define NUMBER_OF_PASSWORD_TRIALS 3
#define DUMMY_VALUE 0xF0
/**************************************/

/***********User defined Data Types***************/

/*************************************************/

/**********Extern Public Global Variable************/
extern uint8 g_passwordArray[PASSWORD_SIZE_PLUS_ENTER] ,
		g_repeatedPassword[PASSWORD_SIZE_PLUS_ENTER];
extern TIMER1_ConfigType g_TIMER1_configurations;
extern volatile uint8 g_tick ;
extern uint8 g_3trialsPasswordFlag;
/***************************************************/

/***************Function Prototypes For Public Functions**********************/

/*
 * Description : Function to set UART dynamic configurations
 */
void UART_setDynamicConfig (UART_config *UART_configurations);

/*
 * Description : Function to set TWI dynamic configurations
 */
void TWI_setDynamicConfig (TWI_config *TWI_configurations);

/*
 * Description : Function to set TIMER1 dynamic configurations
 */
void TIMER1_setDynamicConfig (TIMER1_ConfigType *TIMER1_configurations);

/*
 * Description : Function to receive newly created password and store it in EEPROM
 */
void doorLock_receiveAndStoreNewPassword (void);

/*
 * Description : Function to receive the password from CONTROL_ECU
 */
void doorLock_receivePassword (uint8 *receivedPass);

/*
 * Description : Function to check whether the 2 passwords are identical or not in case of creating a new password
 */
uint8 doorLock_passwordFirstCheck (void);

/*
 * Description : Function to save the password in EEPROM
 */
void doorLock_savePassword (void);

/*
 * Description : Function to receive the required option from HMI_ECU
 */
uint8 doorLock_receiveOption (void);

/*
 * Description : Function to check whether the saved password in EEPROM is identical to the received password or not
 */
uint8 doorLock_passwordSecondCheck (void);

/*
 * Description : Function to increment g_tick every compare match [callback function]
 */
void tickIncrement (void);

/*
 * Description : Function to make a delay for specific time in seconds
 */
void delayInSec (uint16 timeInSec);

/*
 * Description : Function to open the door
 */
void doorLock_openDoor (void);

/*
 * Description : Function to check the entered password(for the third time) in order to do specific option
 * in case the user failed to enter the password correctly in the 3 trials then we have to return to the main options
 */
uint8 doorLock_3trialsPasswordCheck (void);
/****************************************************************************/

/***********Macros***********/

/****************************/


#endif /* CONTROL_ECU_FUNCTIONS_H_ */
