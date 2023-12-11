/****************************************************
 *    File name: dc-motor.c
 *   Created on: Oct 6, 2022
 *       Author: Mohamed Badr
 *  Description: Source file for DC-Motor driver
 ****************************************************/

/*******Module header file**********/
#include "dc-motor.h"
/***********************************/

/**********Other Modules header files used by this module*************/
#include "gpio.h"
#include "timer0.h"
/*****************************************************************/

/**********header files used by this module*************/
#include <avr/io.h>
/*****************************************************************/

/*******Global variables***********/

/***********************************/

/*********Interrupt Service Routine**********/

/********************************************/

/***************Function Prototypes For private Functions**********************/

/******************************************************************************/

/*************************Functions definitions**************************/
/*
 * Description : Function to initialize pins direction and value of the DC-Motor
 */
void DcMotor_init (void)
{
	/*make PB1,PB2 output pins to determine the state of the motor*/
	GPIO_setupPinDirection(DC_MOTOR_INPUT1_PORT_ID, DC_MOTOR_INPUT1_PIN_ID,
			PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_INPUT2_PORT_ID, DC_MOTOR_INPUT2_PIN_ID,
			PIN_OUTPUT);
	/*********************************************************************/

	/*make the motor stops at the start*/
	GPIO_writePin(DC_MOTOR_INPUT1_PORT_ID, DC_MOTOR_INPUT1_PIN_ID, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_INPUT2_PORT_ID, DC_MOTOR_INPUT2_PIN_ID, LOGIC_LOW);
	/*********************************/
}

/*
 * Description : Function to set the motor rotation state and the to set the speed of the motor
 */
void DcMotor_Rotate (DcMotor_state state , uint8 speed)
{

	/***********************to set the rotation state of the motor*********************************/
	if (state == DC_MOTOR_STOP)
	{
		DC_MOTOR_INPUT1_PORT = (DC_MOTOR_INPUT1_PORT
				& ~ (1 << DC_MOTOR_INPUT1_PIN_ID));/*set input1 = 0*/
		DC_MOTOR_INPUT2_PORT = (DC_MOTOR_INPUT2_PORT
				& ~ (1 << DC_MOTOR_INPUT2_PIN_ID));/*set input2 = 0*/
	}
	else if (state == DC_MOTOR_CLOCKWISE)
	{
		DC_MOTOR_INPUT1_PORT = (DC_MOTOR_INPUT1_PORT
				& ~ (1 << DC_MOTOR_INPUT1_PIN_ID));/*set input1 = 0 */
		DC_MOTOR_INPUT2_PORT = (DC_MOTOR_INPUT2_PORT
				& ~ (1 << DC_MOTOR_INPUT2_PIN_ID))
				| ( LOGIC_HIGH << DC_MOTOR_INPUT2_PIN_ID);/*set input2 = 1 */
	}
	else
	{
		DC_MOTOR_INPUT1_PORT = (DC_MOTOR_INPUT1_PORT
				& ~ (1 << DC_MOTOR_INPUT1_PIN_ID))
				| ( LOGIC_HIGH << DC_MOTOR_INPUT1_PIN_ID);/*set input1 = 1 */
		DC_MOTOR_INPUT2_PORT = (DC_MOTOR_INPUT2_PORT
				& ~ (1 << DC_MOTOR_INPUT2_PIN_ID));/*set input2 = 0 */
	}
	/****************************************************************************************************/
	PWM_Timer0_Start(speed);/*to set the speed of the motor*/
}

/*****************************************************************/
