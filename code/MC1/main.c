/**************************************************************************************************************
 *
 * MODULE: MC1
 *
 * FILE NAME: main.c
 *
 * Description: main file for the micro controller 1
 *
 * Created on: November 6, 2022
 *
 * Author: Saeed Elsayed
 *
 **************************************************************************************************************/

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include <util/delay.h>
#include "timer1.h"


uint8 volatile end_of_callBack_function = 0;   /* it will tell us if the callback function has ended or not */

void clearLCD(){     /* the callback function to clear the lcd */
	LCD_clearScrean(); /* clear the LCD */
	end_of_callBack_function =1;   /* set it to 1 so we know that the callback function has ended */
}


int main(void){

	uint8 password_is_set_first = 0;   /* variable tell us that the password has been entered */
	uint8 password_is_set_second = 0;  /* variable tell us that the password has been re-entered */
	uint8 key;  /* store the value of the pressed button from the keypad */
	uint8 number_of_entered_keys = 0; /* make sure that the user has entered five numbers */
	uint8 password_entered = 0;  /* make sure that the user has pressed enter */
	uint8 pass_is_correct;    /* it will come from the other UART from the other micro-controller to tell
	                                us if the two passwords are matched or not */
	uint8 phase_of_pass = 1;   /* will clear it if the two passwords are matched and then phase of
	                              password has ended and the system has a password*/
	uint8 pass_to_unlock_door_is_correct; /* will come from the other UART to tell us if the password
	                                         entered to open the door is correct or not */
	LCD_init(); /* initiate the LCD*/
	UART_ConfigType uart_config = {bits_8, disabled, one_bit, 9600}; /* set the configurations of the UART */
	UART_init(&uart_config);    /* initiate the UART with the required configurations */
	Timer1_setCallBack(clearLCD);  /* set the callback of timer1 with compare mode to the function clearLCD */



	while(1)
	{
		if(phase_of_pass){    /* if phase of password has not ended then enter a new password*/
			if(password_is_set_first == 0)  /* check if the password is entered for the first time or not */
			{
				LCD_clearScrean();       /* clear the LCD */
				LCD_moveCursor(0,0);     /* move cursor of LCD to row 0 and column 0 */
				LCD_displayString("plz enter pass:");  /* ask the user to enter a password */
				LCD_moveCursor(1,0);     /* move cursor of LCD to row 1 and column 0 */
				while(!password_is_set_first) /* do not exit until the five numbers is entered */
				{
					key = KEYPAD_getPressedKey(); /* take the numbers of password from the user from the keypad */
					if(key == '=')  /* if user pressed enter before giving us the five numbers then ignore that */
						continue;
					LCD_displayCharacter('*');  /* display '*' when the user enter a number */
					UART_sendByte(key);         /* send the entered number to the other UART */
					_delay_ms(300);  /* make delay to prevent the keypad from entering many numbers
					                    while the user is entering only one number */
					number_of_entered_keys++;  /* increment the counter of the entered numbers */
					if(number_of_entered_keys == 5){   /* if the user has entered five numbers then
					                                      wait for him to press the enter button */
						while(!password_entered) /* do not exit until the enter is pressed */
						{
							key = KEYPAD_getPressedKey(); /* take input from the user */
							if(key == '='){
								password_entered = 1;     /* set it to 1 if user pressed enter to exit form the while loop */
							}
						}
						password_is_set_first = 1; /* we have taken the password for the first time */
					}

				}
				number_of_entered_keys = 0;     /* reset counter of entered numbers to reuse it in the re-entering */
				password_entered =0;    /* reset checker of the enter button to reuse it in the re-entering */
			}
			if(password_is_set_second == 0) /* check if the password is re-entered or not */
			{
				LCD_moveCursor(0,0);    /* move cursor of LCD to row 0 and column 0 */
				LCD_displayString("plz re-enter the");   /* ask the user to re-enter the password */
				LCD_moveCursor(1,0);      /* move cursor of LCD to row 1 and column 0 */
				LCD_displayString("same pass:");
				while(!password_is_set_second)    /* do not exit until the five numbers is entered */
				{
					key = KEYPAD_getPressedKey();  /* take the numbers of password from the user from the keypad */
					if(key == '=')   /* if user pressed enter before giving us the five numbers then ignore that */
						continue;
					LCD_displayCharacter('*'); /* display '*' if a number is entered  */
					UART_sendByte(key);  /* send the entered number to the other UART to compare it with the first password */
					_delay_ms(300);      /* make delay to prevent the keypad from entering many numbers
					                        while the user is entering only one number */
					number_of_entered_keys++;  /* increment the counter of the entered numbers */
					if(number_of_entered_keys == 5){      /* if the user has entered five numbers then
					                                      wait for him to press the enter button */
						while(!password_entered)   /* do not exit until the enter is pressed */
						{
							key = KEYPAD_getPressedKey();  /* take input from the user */
							if(key == '='){
								password_entered = 1;  /* set it to 1 if user pressed enter to exit form the while loop */
							}
						}
						password_is_set_second = 1;   /* we have taken the password for the second time */
					}

				}
				number_of_entered_keys = 0; /* reset counter of entered numbers to reuse it if the user asked to change the pass */
				password_entered =0;        /* reset checker of the enter button to reuse it if the user asked to change the pass */
			}

			pass_is_correct = UART_recieveByte(); /* UART tell us if the two passwords are matched or not */
			if(pass_is_correct == 1){  /* if they are matched then we save the password and end the phase of the password */
				phase_of_pass = 0;
			}
			password_is_set_first = 0;   /* reset to reuse it if the user asked to change the pass */
			password_is_set_second = 0;  /* reset to reuse it if the user asked to change the pass */
		}

		if(!phase_of_pass){       /* now the phase of pass has ended and we are showing the main options */
			LCD_clearScrean();    /* clear the LCD */
			LCD_moveCursor(0,0);  /* move cursor of LCD to row 0 and column 0 */
			LCD_displayString("+ : Open Door"); /* show the user the two options */
			LCD_moveCursor(1,0);
			LCD_displayString("- : Change Pass");
			key = KEYPAD_getPressedKey();
			while((key != '+') && (key != '-')){  /* wait until he chooses any of them */
				key = KEYPAD_getPressedKey();
				_delay_ms(300);
			}
			if(key == '+'){   /* if he chooses to open the door we will ask him to enter the pass and will give him 3 tries */
				uint8 tries = 3;   /* number of tries to enter the correct password */
				uint8 success = 0; /* state of entering the password */
				while(tries>0){    /* still taking the pass form the user until the tries ends */
					LCD_clearScrean(); /* clear the LCD */
					LCD_moveCursor(0,0);  /* move cursor of LCD to row 0 and column 0 */
					LCD_displayString("plz enter pass:"); /* ask the user to enter the password */
					LCD_moveCursor(1,0); /* move cursor of LCD to row 1 and column 0 */
					UART_sendByte(2);    /* 2 is a number from my choice that tell the UART that we need to
					                        check the entered pass if correct or not */
					uint8 digits_of_password = 0; /* number of entered numbers */
					while(digits_of_password<5)   /* do not exit until the five numbers is entered */
					{
						key = KEYPAD_getPressedKey();  /* take the numbers of password from the user from the keypad */
						if(key == '=')   /* if user pressed enter before giving us the five numbers then ignore that */
							continue;
						LCD_displayCharacter('*'); /* display '*' if a number is entered  */
						UART_sendByte(key);        /* send the entered number to the other UART */
						_delay_ms(300);    /* make delay to prevent the keypad from entering many numbers
					                        while the user is entering only one number */
						digits_of_password++;  /* increment the counter of the entered numbers */
					}
					while(!password_entered)     /* do not exit until the enter is pressed */
					{
						key = KEYPAD_getPressedKey();  /* take input from the user */
						if(key == '='){
							password_entered = 1;  /* set it to 1 if user pressed enter to exit form the while loop */
						}
					}
					password_entered = 0; /* reset checker of the enter button to reuse it if the user asked to change the pass */
					pass_to_unlock_door_is_correct = UART_recieveByte(); /* UART tell us if the pass is correct or not */
					UART_sendByte(10);   /* byte that I send  to prevent the motor form rotation before it receive this byte*/
					if(pass_to_unlock_door_is_correct == 5 )  /* all the five numbers are correct */
					{
						LCD_clearScrean(); /* clear the LCD */
						LCD_moveCursor(0,4); /* move cursor of LCD to row 0 and column 4 */
						LCD_displayString("Door is"); /* show this statement */
						LCD_moveCursor(1,3);  /* move cursor of LCD to row 1 and column 3 */
						LCD_displayString("unlocking"); /* show this statement */
						/* wait for 15 seconds and then call the callback function to clear the LCD */
						Timer1_ConfigType timer1_config= {0, 15000, F_CPU_1024, COMPARE_MODE};
						Timer1_init(&timer1_config); /* initiate the timer1 with the required configurations */
						while(!end_of_callBack_function); /* do not exit until the timer finishes and callback function ends  */
						end_of_callBack_function = 0;  /* reset to reuse it again */
						/* wait for 3 seconds after clearing the LCD while the motor is fixed */
						Timer1_ConfigType timer2_config= {0, 3000, F_CPU_1024, COMPARE_MODE};
						Timer1_init(&timer2_config);
						while(!end_of_callBack_function);
						end_of_callBack_function = 0;
						LCD_moveCursor(0,0); /* move cursor of LCD to row 0 and column 0 */
						LCD_displayString("Door is locking");  /* show this statement */
						/* wait for 15 seconds and then call the callback function to clear the LCD */
						Timer1_ConfigType timer3_config= {0, 15000, F_CPU_1024, COMPARE_MODE};
						Timer1_init(&timer3_config);
						while(!end_of_callBack_function);
						end_of_callBack_function = 0;
						Timer1_deInit(); /* terminate timer1 */
						tries = 0;     /* make tries = 0 to exit form the will loop */
						success = 1;   /* the entered password is correct */
					}

					else{
						tries--;  /* decrement the available tries */
					}

				}
				if(success == 0){     /* if the user has entered the pass 3 times wrong */
					UART_sendByte(4); /* 4 is a number from my choice that tell the UART that the user has entered
					                  the pass 3 times wrong and we need to turn on the buzzer */
					LCD_clearScrean(); /* clear the LCD */
					LCD_moveCursor(0,5); /* move cursor of LCD to row 0 and column 5 */
					LCD_displayString("ERROR"); /* display error massage */
					/* wait for 1 minute using ticks and then call the callback function to clear the LCD */
					Timer1_ConfigType timer1_config= {0, 60000, F_CPU_1024, COMPARE_MODE};
					Timer1_init(&timer1_config);
					while(!end_of_callBack_function); /* do not exit until the timer finishes and callback function ends  */
					end_of_callBack_function = 0;  /* reset to reuse it again */
					Timer1_deInit(); /* terminate timer1 */
				}
			}
			else if(key == '-'){  /* if he chooses to change password we will ask him to enter the pass and will give him 3 tries */
				uint8 tries = 3;   /* number of tries to enter the correct password */
				uint8 success = 0; /* state of entering the password */
				while(tries>0){    /* still taking the pass form the user until the tries ends */
					LCD_clearScrean(); /* clear the LCD */
					LCD_moveCursor(0,0);  /* move cursor of LCD to row 0 and column 0 */
					LCD_displayString("plz enter pass:"); /* ask the user to enter the password */
					LCD_moveCursor(1,0); /* move cursor of LCD to row 1 and column 0 */
					UART_sendByte(3);    /* 3 is a number from my choice that tell the UART that we need to
									                        check the entered pass if correct or not */
					uint8 digits_of_password = 0; /* number of entered numbers */
					while(digits_of_password<5)   /* do not exit until the five numbers is entered */
					{
						key = KEYPAD_getPressedKey();  /* take the numbers of password from the user from the keypad */
						if(key == '=')   /* if user pressed enter before giving us the five numbers then ignore that */
							continue;
						LCD_displayCharacter('*'); /* display '*' if a number is entered  */
						UART_sendByte(key);        /* send the entered number to the other UART */
						_delay_ms(300);    /* make delay to prevent the keypad from entering many numbers
									                        while the user is entering only one number */
						digits_of_password++;  /* increment the counter of the entered numbers */
					}
					while(!password_entered)     /* do not exit until the enter is pressed */
					{
						key = KEYPAD_getPressedKey();  /* take input from the user */
						if(key == '='){
							password_entered = 1;  /* set it to 1 if user pressed enter to exit form the while loop */
						}
					}
					password_entered = 0; /* reset checker of the enter button to reuse it if the user asked to change the pass */
					uint8 state_of_change_password = UART_recieveByte();  /* UART tell us if the pass is correct or not */
					UART_sendByte(17); /* byte form my choice I send to make sure the other UART is synchronized with me*/
					if(state_of_change_password == 5 ){   /* all the five numbers are correct */
						phase_of_pass = 1;     /* re-open the phase of the pass again to allow the user to change it */
						password_is_set_first = 0;
						password_is_set_second = 0;
						tries = 0;     /* make tries = 0 to exit form the will loop */
						success = 1;   /* the entered password is correct */
					}
					else
					{
						tries--; /* decrement the available tries */
					}
				}
				if(success == 0){     /* if the user has entered the pass 3 times wrong */
					UART_sendByte(4); /* 4 is a number from my choice that tell the UART that the user has entered
									                  the pass 3 times wrong and we need to turn on the buzzer */
					LCD_clearScrean(); /* clear the LCD */
					LCD_moveCursor(0,5); /* move cursor of LCD to row 0 and column 5 */
					LCD_displayString("ERROR"); /* display error massage */
					/* wait for 1 minute using ticks and then call the callback function to clear the LCD */
					Timer1_ConfigType timer1_config= {0, 60000, F_CPU_1024, COMPARE_MODE};
					Timer1_init(&timer1_config);
					while(!end_of_callBack_function); /* do not exit until the timer finishes and callback function ends  */
					end_of_callBack_function = 0;  /* reset to reuse it again */
					Timer1_deInit(); /* terminate timer1 */
				}
			}
		}




	}
}
