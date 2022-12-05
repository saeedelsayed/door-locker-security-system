/**************************************************************************************************************
 *
 * MODULE: LCD
 *
 * FILE NAME: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Created on: October 5, 2022
 *
 * Author: Saeed Elsayed
 *
 **************************************************************************************************************/


#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"

/**********************************************************************************
 *                                  Definitions                                   *
 **********************************************************************************/

#define LCD_DATA_BITS_MODE            8

#if (LCD_DATA_BITS_MODE !=4  && LCD_DATA_BITS_MODE !=8)

#error "number of data bits should be equal 4 or 8"

#endif


#define LCD_RS_PORT_ID                PORTD_ID
#define LCD_RS_PIN_ID                 PIN6_ID

#define LCD_E_PORT_ID                 PORTD_ID
#define LCD_E_PIN_ID                  PIN7_ID

#define LCD_DATA_PORT_ID              PORTA_ID

#if (LCD_DATA_BITS_MODE ==4)     // configuration for the 4 bits data mode

#define LCD_DB4_PIN_ID                PIN3_ID
#define LCD_DB5_PIN_ID                PIN4_ID
#define LCD_DB6_PIN_ID                PIN5_ID
#define LCD_DB7_PIN_ID                PIN6_ID

#endif

#define LCD_CLEAR_COMMAND                    0x01
#define LCD_GO_TO_HOME                       0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE        0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE         0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_CURSOR_OFF                       0x0C
#define LCD_CURSOR_ON                        0x0E
#define LCD_SET_CURSOR_LOCATION              0x80

/**********************************************************************************
 *                           Functions Prototypes                                 *
 **********************************************************************************/

/*
 * Description:
 * Initialize the LCD:
 * 1- setup the LCD pins direction by using the GPIO driver
 * 2- setup the LCD data mode
 */
void LCD_init(void);

/*
 * Description:
 * the function that sends the commands to the LCD
 * */
void LCD_sendCommand(uint8 command);

/*
 * Description:
 * display any character on the screen
 */
void LCD_displayCharacter(uint8 data);

/*
 * Description:
 * display any string on the screen
 */
void LCD_displayString(const char *Str);

/*
 * Description:
 * begin to write the first character from any place by moving the cursor to that place
 */
void LCD_moveCursor(uint8 row, uint8 col);

/*
 * Description:
 * write a string on a specific row and a specific column
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *Str);

/*
 * Description:
 * display an integer on the screen as a string
 */
void LCD_integerToString(int data);

/*
 * Description:
 * delete any thing from the screen
 */
void LCD_clearScrean(void);

#endif /* LCD_H_ */
