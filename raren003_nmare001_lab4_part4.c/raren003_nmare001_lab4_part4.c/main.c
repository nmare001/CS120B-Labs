/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #4 Exercise #4
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

enum States {Start, Init, One, Two, Unlocked} state;

unsigned char tmpC = 0x07;

void Tick(){
	
	
	switch(state) {				//transitions
		case Start:
		state = Init;
		break;
		
		case Init:
		if (PINA == 0x04) state = One;
		break;
		
		case One:
		if ((PINA & 0x02) || (PINA & 0x80) || (PINA == 0x03)) state = Init;
		else if ((PINA == 0x00)) state = Two;
		break;
		
		case Two:
		if ((PINA & 0x04) || (PINA & 0x80) || (PINA == 0x03) || ((PINA & 0x02) && (PORTB == 1))) state = Init;
		else if ((PINA == 0x02) && (PORTB == 0)) state = Unlocked;
		break;
		
		case Unlocked:
		if (PINA & 0x80 || (PINA == 0x03)) state = Init;
		else if (PINA == 0x04) state = One;
		break;
		
	}
	
	switch(state){				//state actions
		case Init:
		PORTB = 0;
		PORTC = 1;
		break;
		
		case One:
		PORTC = 2;
		break;
		
		case Two:
		PORTC = 3;
		break;
		
		case Unlocked:
		PORTB = 1;
		PORTC = 4;
		break;
	}								//state actions
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0x00;	// Configure port B's 8 pins as outputs, Initialize 0’s
	DDRC = 0x00; PORTC = 0x00;
	
	PORTB = 0;
	PORTC = 0;
	state = Start;	//initial call
	
	while(1) {Tick();}
}

