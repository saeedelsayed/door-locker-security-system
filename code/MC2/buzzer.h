/**************************************************************************************************************
 *
 * MODULE: BUZZER
 *
 * FILE NAME: buzzer.h
 *
 * Description: Header file for the buzzer driver
 *
 * Created on: November 5, 2022
 *
 * Author: Saeed Elsayed
 *
 **************************************************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

#define BUZZER_PORT_ID         PORTC_ID
#define BUZZER_PIN_ID          PIN5_ID

/****************************************************************************************
 *                                 functions definition                                 *
 ***************************************************************************************/

/*
 * Description:
 * function that initiate the buzzer
 */
void Buzzer_init();

/*
 * Description:
 * function that enables the buzzer
 */
void Buzzer_on(void);

/*
 * Description:
 * function that disables the buzzer
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
