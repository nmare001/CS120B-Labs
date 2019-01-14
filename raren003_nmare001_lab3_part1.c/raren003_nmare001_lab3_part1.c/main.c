/* Partner 1 Name & E-mail: Robert Arenas, raren003@ucr.edu
* Partner 2 Name & E-mail: Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #3 Exercise #1
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
	DDRB = 0x00; PORTB = 0xFF;	// Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF;	// Configure port C's 8 pins as inputs
	DDRD = 0x00; PORTD = 0x00;	// Configure port D's 8 pins as outputs, Initialize 0’s
	unsigned char tmpA = 0x00;  // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00;  // Temporary variable to hold the value of B
	
    unsigned char uc_numOnes;
	unsigned char uc_i;
    while (1) 
    {
		uc_numOnes = 0;
		tmpA = PINA;
		tmpB = PINB;
		
		for (uc_i = 0; uc_i < 8; ++uc_i) {
			if (GetBit(tmpA, uc_i)) ++uc_numOnes;
			if (GetBit(tmpB, uc_i)) ++uc_numOnes;
		}
		
		PORTC = uc_numOnes;
    }
}

