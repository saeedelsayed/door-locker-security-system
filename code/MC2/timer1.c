/**************************************************************************************************************
 *
 * MODULE: TIMER1
 *
 * FILE NAME: timer1.c
 *
 * Description: Source file for the TIMER1 driver
 *
 * Created on: November 5, 2022
 *
 * Author: Saeed Elsayed
 *
 **************************************************************************************************************/

#include "timer1.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

uint8 ticks =0;
uint8 buzzer_ticks =0;

/* Global variable to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_callBackPtr2)(void) = NULL_PTR;

/*
 * Description:
 * Function to initialize the Timer driver
 * */
void Timer1_init(const Timer1_ConfigType * Config_Ptr){

	TCNT1 = Config_Ptr->initial_value;  /* TCNT1 will carry the initial value */
	TCCR1A |= (1<<FOC1A) | (1<<FOC1B); /* set for non pwm_mode */
	if(Config_Ptr->mode == COMPARE_MODE){

		TCCR1B |= (1<<WGM12);  /* choose the compare mode */
		OCR1A = Config_Ptr->compare_value; /* store the compare value in the compare register A */
		TIMSK |=  (1<<OCIE1A);         /* enable the interrupt for compare register A */

	}

	if(Config_Ptr->mode == NORMAL_MODE){

		/* by default WGM10, WGM11, WGM12 and WGM13 will be zeros */
		TIMSK |=  (1<<TOIE1);  /* enable the interrupt for the overflow  */

	}

	TCCR1B |= Config_Ptr->prescaler;  /*choosing the pre-scaler wanted */
	SREG |= (1<<7);                /* enable interruption */

}

/*
 * Description:
 * Function to disable the Timer1
 * */
void Timer1_deInit(void){

	/* Clear All Timer1 Registers */
	TCNT1 = 0;
	TCCR1A = 0;
	TCCR1B = 0;
	OCR1A = 0;
	/* disable the interrupt for compare register A */
	TIMSK &= ~(1<<OCIE1A);
	/* disable the interrupt for the overflow  */
	TIMSK &= ~(1<<TOIE1);

}

/*
 * Description:
 * Function to set the Call Back function address
 * */
void Timer1_setCallBack(void(*a_ptr)(void)){

	/* Save the address of the Call back function in a global variable */
		g_callBackPtr = a_ptr;

}
/*
 * Description:
 * Function to set the Call Back function address
 * */
void Timer1_setCallBack2(void(*a_ptr)(void)){

	/* Save the address of the Call back function in a global variable */
		g_callBackPtr2 = a_ptr;

}
/*
 * Description:
 * function the time1 will execute if the timer value is equal the compare value
 * */
ISR(TIMER1_COMPA_vect){
        ticks++;  /* we use ticks because we need the timer to count 2 times before we execute the callback function */
        if(ticks == 2){
        	ticks =0;  /* reset the ticks when executing the callback function */
		if(g_callBackPtr != NULL_PTR){
			(*g_callBackPtr)(); /* execute the callback function */
		}
        }

}
/*
 * Description:
 * function the time1 will execute when the overflow occur
 * */
ISR(TIMER1_OVF_vect){
	buzzer_ticks++; /* we use buzzer_ticks because we need the timer to count 7 times before we execute the callback function */
        if(buzzer_ticks == 7){
        	buzzer_ticks =0;    /* reset the buzzer_ticks when executing the callback function */
		if(g_callBackPtr2 != NULL_PTR){
			(*g_callBackPtr2)(); /* execute the callback function */
		}
        }

}
