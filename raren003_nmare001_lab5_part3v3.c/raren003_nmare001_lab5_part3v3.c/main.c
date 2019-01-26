/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #5 Exercise #3
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

enum States {Start, Init, Wait, One, Two, Three, Four} state;

unsigned int check = 0x00;
void Tick(){
	
	
	switch(state) {				//transitions
		case Start:
		state = Init;
		break;
		
		case Init:
		state = Wait;
		break;
		
		case Wait:
		if ((~PINA & 0x01) && PORTB == 0x00) state = One;
		else if ((~PINA & 0x01) && PORTB == 0x21) state = Two;
		else if ((~PINA & 0x01) && (PORTB == 0x12) && (check == 0x00)) state = Three;
		else if ((~PINA & 0x01) && PORTB == 0x0C) state = Four;
		else if ((~PINA & 0x01) && (PORTB == 0x12) && (check == 0x01)) state = One;
		break;
		
		case One:
		if (!(~PINA & 0x01)) state = Wait;
		break;
		
		case Two:
		if (!(~PINA & 0x01)) state = Wait;
		break;
		
		case Three:
		if (!(~PINA & 0x01)) state = Wait;
		break;
		
		case Four:
		if (!(~PINA & 0x01)) state = Wait;
		break;
		
		
	}
	
	switch(state){				//state actions
		
		case Start:
		PORTB = 0;
		//PORTC = PORTB;
		break;
		
		case Init:
		PORTB = 0;
		//PORTC = PORTB;
		break;
		
		case One:
		PORTB = 0x21;
		//PORTC = PORTB;
		break;
		
		case Two:
		PORTB = 0x12;
		//PORTC = PORTB;
		check = 0x00;
		break;
		
		case Three:
		PORTB = 0x0C;
		//PORTC = PORTB;
		break;
		
		case Four:
		PORTB = 0x12;
		//PORTC = PORTB;
		check = 0x01;
		break;
	}								//state actions
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00;	// Configure port B's 8 pins as outputs, Initialize 0’s
	DDRC = 0xFF; PORTC = 0x00;
	
	PORTB = 0;
	state = Start;	//initial call
	
	while(1) {Tick();}
}

