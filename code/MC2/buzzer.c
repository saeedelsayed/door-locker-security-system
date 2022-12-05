/**************************************************************************************************************
 *
 * MODULE: BUZZER
 *
 * FILE NAME: buzzer.c
 *
 * Description: Source file for the buzzer driver
 *
 * Created on: November 5, 2022
 *
 * Author: Saeed Elsayed
 *
 **************************************************************************************************************/

#include "buzzer.h"
#include <avr/io.h>


/*
 * Description:
 * function that initiate the buzzer
 */
void Buzzer_init(){

	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT); /* Setup the direction for the buzzer pin as output pin */
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW); /* initially turn off the buzzer */

}

/*
 * Description:
 * function that enables the buzzer
 */
void Buzzer_on(void){

	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH); /* turn on the buzzer */

}

/*
 * Description:
 * function that disables the buzzer
 */
void Buzzer_off(void){

	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW); /* turn off the buzzer */

}

