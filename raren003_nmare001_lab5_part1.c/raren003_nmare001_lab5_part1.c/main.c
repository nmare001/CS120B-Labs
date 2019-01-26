/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #5 Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}



int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as inputs
	
	unsigned char button = 0x00;
	unsigned char led = 0x00;
	
	while (1)
	{
		led = 0;
		button = ~PINA & 0x0F;
		//PORTA = 0xFF;
		
		
		if (button >= 1) led = SetBit(led, 5, 1);
		if (button >= 3) led = SetBit(led, 4, 1);
		if (button >= 5) led = SetBit(led, 3, 1);
		if (button >= 7) led = SetBit(led, 2, 1);
		if (button >= 10) led = SetBit(led, 1, 1);
		if (button >= 13) led = SetBit(led, 0, 1);
		if (button <= 4) led = SetBit(led, 6, 1);
		PORTC = led;
	}
}
