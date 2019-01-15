/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #3 Exercise #5
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
	DDRD = 0x00; PORTD = 0xFF;	// Configure port D's 8 pins as inputs
	unsigned char tmpD = 0x00;  // Temporary variable to hold the value of D
	unsigned char tmpB = 0x00;  // Temporary variable to hold the value of B
	
	unsigned long ul_totalWeight;
	unsigned long ul_bitCalc = 1;
	unsigned char uc_i;
	while (1)
	{
		
		ul_totalWeight = 0;
		ul_bitCalc = 1;
		
		tmpD = PIND;
		tmpB = PINB;
		if (GetBit(tmpB, 0)) ++ul_totalWeight;
		ul_bitCalc = ul_bitCalc * 2;
		
		for (uc_i = 0; uc_i < 8; ++uc_i) {
			if (GetBit(tmpD, uc_i)) ul_totalWeight = ul_totalWeight + ul_bitCalc;
			ul_bitCalc = ul_bitCalc * 2;
		}
		
		if (ul_totalWeight >= 70) tmpB = SetBit(tmpB, 1, 1); //SetBit returns a copy
		if (ul_totalWeight >= 5 && ul_totalWeight < 70) tmpB = SetBit(tmpB, 2, 1);
		PORTB = tmpB;
		
	}
}

