/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #6 Exercise #2
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "timer.h"

enum States {START, INIT, NEXTLED, LIT, WAIT} state;

unsigned char tmpB = 0x00; //variable for setting portB
unsigned char button = 0x00;

void Tick(){
	switch(state){			//transitions
		case START:
		state = INIT;
		break;
		
		case INIT:
		if (!button) state = NEXTLED;
		break;
		
		case NEXTLED:
		state = NEXTLED;
		if (button) state = LIT;
		else state = NEXTLED;
		break;
		
		case LIT:
		if (!button) state = WAIT;
		else state = LIT;
		break;
		
		case WAIT:
		if (button) state = INIT;
		else state = WAIT;
		break;
		
		default:
		break;
	}						//transitions
	
	switch(state){			//state actions
		case START:
		break;
		
		case INIT:
		tmpB = 0x01;
		PORTB = tmpB;
		break;
		
		case NEXTLED:
		if (tmpB >= 0x04) //go back to led 1 after 3 is lit
		{
			tmpB = 0x01;
		}
		else //shift to move to next led
		{
			tmpB = tmpB * 2;
		}
		PORTB = tmpB;
		break;
		
		default:
		break;
	}						//state actions
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	DDRA = 0x00; PORTA = 0xFF;
	
	state = START;
			TimerSet(300);
			TimerOn();
	button = ~PINA & 0x01;
	while (1)
	{

		Tick();
		button = ~PINA & 0x01;
		while (!TimerFlag){} //wait 1 sec
		TimerFlag = 0;
	}
}

