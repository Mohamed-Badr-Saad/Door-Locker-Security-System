/*
 *    File name: buzzer.h
 *   Created on: Nov 3, 2022
 *       Author: Mohamed Badr
 *  Description: Header file for BUZZER driver
 */

#ifndef BUZZER_H_
#define BUZZER_H_
/************Common Header Files***************/

/**********************************************/

/*************Static Configurations***************/
#define BUZZER_PORT_ID PORTD_ID
#define BUZZER_PIN_ID PIN7_ID
#define BUZZER_ON LOGIC_LOW
#define BUZZER_OFF LOGIC_HIGH
/*************************************************/

/************Definitions***************/

/**************************************/

/***********User defined Data Types***************/

/*************************************************/

/**********Extern Public Global Variable************/

/***************************************************/

/***************Function Prototypes For Public Functions**********************/
/*
 * Description : Function to set the pin direction that is connected to the BUZZER
 */
void BUZZER_init (void);

/*
 * Description : Function to turn on the BUZZER
 */
void BUZZER_on(void);

/*
 * Description : Function to turn off the BUZZER
 */
void BUZZER_off(void);
/****************************************************************************/

/***********Macros***********/

/****************************/


#endif /* BUZZER_H_ */
