/**************************************************************************************************************
 *
 * MODULE: DC motor
 *
 * FILE NAME: DC_motor.h
 *
 * Description: Header file for the DC motor driver
 *
 * Created on: October 9, 2022
 *
 * Author: Saeed Elsayed
 *
 **************************************************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

#define DC_MOTOR_PORT_ID             PORTB_ID  // define the port we connect the pins of the motor to its pins

#define DC_MOTOR_INPUT_PIN1_ID       PIN0_ID   // define the first pin of the motor
#define DC_MOTOR_INPUT_PIN2_ID       PIN1_ID   // define the second pin of the motor

#define DC_MOTOR_ENABLE_PORT_ID      PORTB_ID  // define the port of the enable pin of the bridge
#define DC_MOTOR_ENABLE_PIN_ID       PIN3_ID   // define the pin of the enable pin of the bridge


typedef enum       // determine the direction of rotation of the DC motor
{
   STOP, CLOCK_WISE, ANTI_CLOCK_WISE
}DcMotor_State;

typedef enum{      // determine the mode of speed of the DC motor
	OFF, SPEED_25, SPEED_50, SPEED_75, SPEED_100
}DC_speed;

/*
 * Description:
 * function that initialize the DC motor
 */
void DcMotor_Init(void);


/*
 * Description:
 * function which determine the direction of rotation and speed
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
