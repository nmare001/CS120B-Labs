/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #3 Exercise #4
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0x00;	// Configure port B's 8 pins as outputs, Initialize 0’s
	DDRC = 0x00; PORTC = 0x00;	// Configure port C's 8 pins as outputs, Initialize 0’s
	unsigned char tmpA = 0x00;  // Temporary variable to hold the value of A
	
	unsigned char uc_upNib;  //variable used to get the upper nibble of PINA
	unsigned char uc_lowNib; //variable used to get the lower nibble of PINA
	
	while (1)
	{
		//Read input
		tmpA = PINA;
		
		//isolate and shift the upper nibble of PINA to the lower nibble of the variable uc_upNib
		uc_upNib = (tmpA & 0xF0) >> 4;
		//isolate and shift the lower nibble of PINA to the upper nibble of the variable uc_lowNib
		uc_lowNib = (tmpA & 0x0F) << 4;
		
		//assign the upper nibble of PINA to the lower nibble of PORTB
		PORTB = uc_upNib;
		//assign the lower nibble of PINAA to the upper nibble of PORTC
		PORTC = uc_lowNib;
		
	}
}

