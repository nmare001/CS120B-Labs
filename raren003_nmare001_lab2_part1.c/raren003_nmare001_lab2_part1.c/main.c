/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #2 Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00;	// Configure port B's 8 pins as outputs, Initialize 0’s
	unsigned char tmpB = 0x00;  // Temporary variable to hold the value of B
	unsigned char tmpA = 0x00;  // Temporary variable to hold the value of A
	while (1)
	{
		// 1) Read input
		tmpA = PINA;
		if(tmpA == 0x01 || tmpA == 0x02){
			tmpB = (tmpB & 0x00) | 0x01;
		}
		else{
			tmpB = (tmpB & 0x00) | 0x00;
		}
		
		//write output
		PORTB = tmpB;
	}
}

