/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #4 Exercise #5
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

enum States {Start, Init, One, Two, Unlocked} state;
unsigned char inp[3] = {0x04, 0x01, 0x02};

void Tick(){
	
	
	switch(state) {				//transitions
		case Start:
		state = Init;
		break;
		
		case Init:
		if (PINA == inp[PORTC - 1]) ++PORTC, state = One;
		break;
		
		case One:
		if ((PINA & 0x80) || (PINA == 0x03)) state = Init;
		else if ((PINA == 0x00)) state = Two;
		break;
		
		case Two:
		if ((PINA & 0x80) || (PORTC > 0x05) || (PINA == 0x03)) state = Init;
		else if ((PINA == 0x01) && (PORTC == 0x05)) state = Unlocked;
		else if ((PINA == inp[PORTC - 1])) ++PORTC, state = One;
		break;
		
		case Unlocked:
		if (PINA & 0x80) state = Init;
		break;
		
	}
	
	switch(state){				//state actions
		case Init:
		PORTB = 0;
		PORTC = 1;
		break;
		
		case One:
		break;
		
		case Two:
		break;
		
		case Unlocked:
		PORTB = 1;
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


