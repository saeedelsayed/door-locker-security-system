/**************************************************************************************************************
 *
 * MODULE: GPIO
 *
 * FILE NAME: gpio.h
 *
 * Description: Header file for the AVR GPIO driver
 *
 * Created on: October 5, 2022
 *
 * Author: Saeed Elsayed
 *
 **************************************************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

#include "std_types.h"

/**************************************************************
 *                        definitions                         *
 **************************************************************/

#define NUM_OF_PORTS           4
#define NUM_OF_PINS_PER_PORT   8

#define PORTA_ID               0
#define PORTB_ID               1
#define PORTC_ID               2
#define PORTD_ID               3

#define PIN0_ID                0
#define PIN1_ID                1
#define PIN2_ID                2
#define PIN3_ID                3
#define PIN4_ID                4
#define PIN5_ID                5
#define PIN6_ID                6
#define PIN7_ID                7

/**************************************************************
 *                        Type Declaration                    *
 **************************************************************/

typedef enum {
	PIN_INPUT, PIN_OUTPUT
}GPIO_PinDirectionType;

typedef enum {
	PORT_INPUT, PORT_OUTPUT =0xFF
}GPIO_PortDirectionType;

/**************************************************************
 *                    Functions Prototypes                    *
 **************************************************************/

/*
 * Description:
 * Setup the direction of the required pin input/output
 */

void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num, GPIO_PinDirectionType direction);

/*
 * Description:
 * Write the value logic high or logic low on the required pin
 * if the pin is input, the function will enable/disable the internal pull-up resistor
 */

void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value);

/*
 * Description:
 * return the value logic high or logic low from the required pin
 * If the input port number or pin number are not correct, The function will return Logic Low.
 */

uint8 GPIO_readPin(uint8 port_num, uint8 pin_num);

/*
 * Description:
 * setup the direction of all pins of the port input/output
 * If the direction value is PORT_INPUT all pins in this port should be input pins.
 * If the direction value is PORT_OUTPUT all pins in this port should be output pins.
 */

void GPIO_setupPortDirection(uint8 port_num, GPIO_PortDirectionType direction);

/*
 * Description:
 * write the value to the pins of the port
 * if the pin is output, the value will be written
 * if the pin is input, this will enable/disable the internal pull-up resistor of that pin
 */

void GPIO_writePort(uint8 port_num, uint8 value);

/*
 * Description:
 * return the value of the required port
 * If the input port number or pin number are not correct, The function will return Logic Low.
 */

uint8 GPIO_readPort(uint8 port_num);

#endif /* GPIO_H_ */
