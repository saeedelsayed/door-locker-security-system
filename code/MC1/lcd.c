/**************************************************************************************************************
 *
 * MODULE: LCD
 *
 * FILE NAME: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Created on: October 5, 2022
 *
 * Author: Saeed Elsayed
 *
 **************************************************************************************************************/

#include "lcd.h"
#include "gpio.h"
#include "common_macros.h"
#include <util/delay.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Description:
 * Initialize the LCD:
 * 1- setup the LCD pins direction by using the GPIO driver
 * 2- setup the LCD data mode
 */
void LCD_init(void)
{
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID,LCD_E_PIN_ID, PIN_OUTPUT);


	_delay_ms(20);   // must make delay > 15ms for the LCD power on

// configurations if we work with the 4 bits data mode
#if (LCD_DATA_BITS_MODE ==4)

	/* Configure 4 pins in the data port as output pins */
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID, PIN_OUTPUT);

	/* Send for 4 bit initialization of LCD  */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);
	/* use 2-lines LCD + 4-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);



#elif (LCD_DATA_BITS_MODE == 8)
// configurations if we work with the 8 bits data mode

	/* Configure the data port as output port */
	GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);
	/* use 2-lines LCD + 8-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);

#endif
	LCD_sendCommand(LCD_CURSOR_OFF);     // make cursor off
	LCD_sendCommand(LCD_CLEAR_COMMAND);  // clear the screen at the beginning

}

/*
 * Description:
 * the function that sends the commands to the LCD
 * */
void LCD_sendCommand(uint8 command)
{
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_LOW); /* Instruction Mode RS=0 */
	_delay_ms(1);   /* delay for processing Tas = 50ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);  /* Enable LCD E=1 */
	_delay_ms(1);  /* delay for processing Tpw - Tdws = 190ns */

// configurations if we work with the 4 bits data mode
#if (LCD_DATA_BITS_MODE ==4)

	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command,4));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command,5));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command,6));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command,7));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH); /* Enable LCD E=0 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command,0));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command,1));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command,2));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command,3));

	_delay_ms(1);  /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */

#elif (LCD_DATA_BITS_MODE ==8)
// configurations if we work with the 8 bits data mode
	GPIO_writePort(LCD_DATA_PORT_ID, command);  /* out the required command to the data bus D0 --> D7 */
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif
}

/*
 * Description:
 * display any character on the screen
 */
void LCD_displayCharacter(uint8 data)
{
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_HIGH);  /* Data Mode RS=1 */
	_delay_ms(1); /* delay for processing Tas = 50ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH); /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

// configurations if we work with the 4 bits data mode
#if (LCD_DATA_BITS_MODE ==4)

	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(data,4));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(data,5));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(data,6));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(data,7));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH); /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(data,0));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(data,1));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(data,2));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(data,3));

	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */

#elif (LCD_DATA_BITS_MODE ==8)
// configurations if we work with the 8 bits data mode
	GPIO_writePort(LCD_DATA_PORT_ID, data); /* out the required command to the data bus D0 --> D7 */
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif
}

/*
 * Description:
 * display any string on the screen
 */
void LCD_displayString(const char *Str)
{
	uint8 i=0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
}

/*
 * Description:
 * begin to write the first character from any place by moving the cursor to that place
 */
void LCD_moveCursor(uint8 row, uint8 col)
{
	uint8 lcd_memmory_address;
	/* Calculate the required address in the LCD DDRAM */
	switch(row){
	case 0:
		lcd_memmory_address = col;
		break;
	case 1:
		lcd_memmory_address = 0x40 + col;
		break;
	case 2:
			lcd_memmory_address = 0x10 + col;
		break;
	case 3:
			lcd_memmory_address = 0x50 + col;
		break;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(LCD_SET_CURSOR_LOCATION | lcd_memmory_address);
}

/*
 * Description:
 * write a string on a specific row and a specific column
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *Str)
{
	/* first we move the cursor then we write the string */
	LCD_moveCursor(row, col);
	LCD_displayString(Str);
}

/*
 * Description:
 * display an integer on the screen as a string
 */
void LCD_integerToString(int data)
{
	char buff[16]; /* String to hold the ascii result */
	itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	LCD_displayString(buff); /* Display the string */
}

/*
 * Description:
 * delete any thing from the screen
 */
void LCD_clearScrean()
{
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}


