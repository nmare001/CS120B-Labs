/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #4 Exercise #2
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

enum States {Start, INIT, WAIT, INCREMENT, DECREMENT, RESET} state;

unsigned char tmpC = 0x07;

void Tick(){
	
	
	switch(state) {				//transitions
		case Start:
		state = INIT;
		break;
		
		case INIT:
		state = WAIT;
		break;
		
		case WAIT:
		if(!(PINA & 0x01) && !(PINA & 0x02)){ //!A0 && !A1
			state = WAIT;
			}else if((PINA & 0x01) && (PINA & 0x02)){ //A0 && A1
			state = RESET;
			}else if((PINA & 0x01) && !(PINA & 0x02)){ //A0 && !A1
			state = INCREMENT;
			}else if(!(PINA & 0x01) && (PINA & 0x02)){ //!A0 && A1
			state = DECREMENT;
		}
		break;
		
		case INCREMENT:
		state = WAIT;
		break;
		
		case DECREMENT:
		state = WAIT;
		break;
		
		case RESET:
		state = WAIT;
		break;
		
		default:
		break;
	}							//transitions
	
	switch(state){				//state actions
		case INIT:
		PORTC = tmpC;
		break;
		
		case WAIT:
		break;
		
		case INCREMENT:
		if (tmpC < 0x09)
		{
			tmpC = tmpC + 1;
		}
		PORTC = tmpC;
		break;
		
		case DECREMENT:
		if (tmpC > 0x00)
		{
			tmpC = tmpC -1;
		}
		PORTC = tmpC;
		break;
		
		case RESET:
		tmpC = 0x00;
		PORTC = tmpC;
		break;
		
		default:
		break;
	}								//state actions
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRC = 0x00; PORTC = 0x00;	// Configure port B's 8 pins as outputs, Initialize 0’s
	
	state = Start;	//initial call
	
	while(1) {Tick();}
}

