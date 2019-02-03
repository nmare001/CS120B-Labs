/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #7 Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "timer.h"
#include "io.c"

enum States {START, INIT, WAIT, INCREMENT, DECREMENT} state;

unsigned char count = 0x00; //counter variable for setting portB

void Tick(){
	
	
	switch(state) {				//transitions
		case START:
		state = INIT;
		break;
		
		case INIT:
		state = WAIT;
		break;
		
		case WAIT:
		if(!(~PINA & 0x01) && !(~PINA & 0x02)){ //!A0 && !A1
			state = WAIT;
			}else if((~PINA & 0x01) && (~PINA & 0x02)){ //A0 && A1
			state = INIT;
			}else if((~PINA & 0x01) && !(~PINA & 0x02)){ //A0 && !A1
			state = INCREMENT;
			}else if(!(~PINA & 0x01) && (~PINA & 0x02)){ //!A0 && A1
			state = DECREMENT;
		}
		break;
		
		case INCREMENT:
		if ((~PINA & 0x01) && !(~PINA & 0x02)){ //A0 && !A1
			state = INCREMENT;
			}else if ((~PINA & 0x01) && (~PINA & 0x02)){ //A0 && A1
			state = INIT;
		}else if (!(~PINA & 0x01))	//!A0
		{
			state = WAIT;
		}
		break;
		
		case DECREMENT:
		if (!(~PINA & 0x01) && (~PINA & 0x02)){ //!A0 && A1
			state = DECREMENT;
			}else if ((~PINA & 0x01) && (~PINA & 0x02)){ //A0 && A1
			state = INIT;
		}else if (!(~PINA & 0x02))	//!A1
		{
			state = WAIT;
		}
		break;
		
		default:
		break;
	}							//transitions
	
	switch(state){				//state actions
		case START:
		break;
		
		case INIT:
		count = 0;
		LCD_Cursor(1);					//position cursor on the LCD display
		LCD_WriteData(count + '0');		//display count
		break;
		
		case WAIT:
		break;
		
		case INCREMENT:
		if (count < 0x09)
		{
			count = count + 1;
		}
		LCD_Cursor(1);					//position cursor on the LCD display
		LCD_WriteData(count + '0');		//display count
		break;
		
		case DECREMENT:
		if (count > 0x00)
		{
			count = count - 1;
		}
		LCD_Cursor(1);					//position cursor on the LCD display
		LCD_WriteData(count + '0');		//display count
		break;
		
		default:
		break;
	}								//state actions
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00;	//LCD data lines
	DDRD = 0xFF; PORTD = 0x00;	//LCD control lines
	
	//initialize the LCD display
	LCD_init();
	
	
	TimerSet(1000);
	TimerOn();
	
	state = START;
	
	while (1)
	{
		Tick();
		while (!TimerFlag){} //wait 1 sec
		TimerFlag = 0;
	}
}

