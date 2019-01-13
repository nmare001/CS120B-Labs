/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #2 Exercise #3
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF;	// Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF;	// Configure port C's 8 pins as inputs
	DDRD = 0x00; PORTD = 0x00;	// Configure port D's 8 pins as outputs, Initialize 0’s
	unsigned char tmpA = 0x00;  // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00;  // Temporary variable to hold the value of B
	unsigned char tmpC = 0x00;  // Temporary variable to hold the value of C
	
	unsigned char uc_dOut = 0x00; //value that will out put on PORTD
	signed long ul_acWeightDiff = 0x00; //variable holds the weight difference between A and C use a signed long because
	// the if c is greater than a the difference is negative in the line that calculates
	//difference
	unsigned long ul_weightActual; //value that holds the total weight in carts A, B, and C
	unsigned long ul_weightShifted; //value that holds the total weight in carts A, B, and C that has been shifted
	
	while (1)
	{
		// 1) Read input
		uc_dOut = 0x00;
		tmpC = PINC;
		tmpA = PINA;
		tmpB = PINB;
		
		
		//calculate total weight of the 3 carts, values have been cast to a short
		ul_weightActual = (short)tmpA + (short)tmpB;
		ul_weightActual = (ul_weightActual + (short)tmpC);
		//calculate the weight difference between carts A and C, values have been cast to a long
		ul_acWeightDiff = ((long)tmpA - (long)tmpC);
		
		if (ul_weightActual > 140)
		{
			uc_dOut = 0x01; //set PD0 to one if the cart's total passenger weight exceeds the maximum of 140 kg
		}
		
		if (ul_acWeightDiff > 80 || ul_acWeightDiff < -80)
		{
			uc_dOut = uc_dOut + 0x02;
		}
		//ul_weightShifted = ul_weightActual ;
		ul_weightShifted = ul_weightActual << 2;
		
		//write output
		PORTD = (uc_dOut + (short)ul_weightShifted);
		
	}
	
}