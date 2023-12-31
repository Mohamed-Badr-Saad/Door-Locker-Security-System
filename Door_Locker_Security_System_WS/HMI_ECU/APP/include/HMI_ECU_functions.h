/*
 *    File name: HMI_ECU_functions.h
 *   Created on: Nov 6, 2022
 *       Author: Mohamed Badr
 *  Description: Heaser file for HMI_ECU functions and configurations
 */

#ifndef HMI_ECU_FUNCTIONS_H_
#define HMI_ECU_FUNCTIONS_H_
/************Common Header Files***************/
#include "std_types.h"
#include "timer1.h"
#include "uart.h"
/**********************************************/

/*************Static Configurations***************/

/*************************************************/

/************Definitions***************/
#define PASSWORD_SIZE 5
#define PASSWORDS_IDENTICAL 1
#define PASSWORDS_NOT_IDENTICAL 0
#define CONTROL_ECU_READY 0xFF
#define CLICK_PERIOD_IN_SEC 500
#define OPEN_DOOR '+'
#define CHANGE_PASSWORD '-'
#define DUMMY_VALUE 0xF0
#define NUMBER_OF_PASSWORD_TRIALS 3
/**************************************/

/***********User defined Data Types***************/

/*************************************************/

/**********Extern Public Global Variable************/
extern TIMER1_ConfigType g_TIMER1_configurations;
extern uint8 g_3trialsPasswordFlag;
extern volatile uint8 g_tick;
/***************************************************/

/***************Function Prototypes For Public Functions**********************/
/*
 * Description : Function to
 * 1) get the password from keypad(user)
 * 2) send the password to CONTROL_ECU
 */
void doorLock_sendPassword (void);

/*
 * Description : Function to wait until the user enters the required option and send this option to CONTROL_ECU
 */
uint8 doorLock_chooseOption (void);

/*
 * Description : Function to increment g_tick every compare match [callback function]
 */
void tickIncrement (void);

/*
 * Description : Function to make a delay for specific time in seconds
 */
void delayInSec (uint16 timeInSec);

/*
 * Description : Function to display the door state
 */
void doorLock_openDoor (void);

/*
 * Description : Function to create new password
 */
void doorLock_createNewPassword (void);

/*
 * Description : Function to check the entered password(for the third time) in order to do specific option
 * in case the user failed to enter the password correctly in the 3 trials then we have to return to the main options
 */
uint8 doorLock_3trialsPasswordCheck (void);

/*
 * Description : Function to set UART dynamic configurations
 */
void UART_setDynamicConfig (UART_config *UART_configurations);

/*
 * Description : Function to set TIMER1 dynamic configurations
 */
void TIMER1_setDynamicConfig (TIMER1_ConfigType *TIMER1_configurations);
/****************************************************************************/

/***********Macros***********/

/****************************/

#endif /* HMI_ECU_FUNCTIONS_H_ */
