/* Partner(s) Name & E-mail:Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #2 Exercise #2
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as outputs, Initialize 0’s
	unsigned char tmpA = 0x00;  // Temporary variable to hold the value of A
	unsigned char cntavail = 0x00; //hold the value that will be out put on portc
	while (1)
	{
		// 1) Read input
		tmpA = PINA;
		if(tmpA == 0x0F){
			cntavail = (cntavail & 0x00) | 0x00; //sets output to binary 0 if 4 of the parking spaces are taken
		}
		else if(tmpA == 0x07 || tmpA == 0x0B || tmpA == 0x0D || tmpA == 0x0E){
			cntavail = (cntavail & 0x00) | 0x01; //sets output to binary 1 if 3 of the parking spaces are taken
		}
		else if(tmpA == 0x03 || tmpA == 0x05 || tmpA == 0x06 || tmpA == 0x09 || tmpA == 0x0A || tmpA == 0x0C){
			cntavail = (cntavail & 0x00) | 0x02; //sets output to binary 2 if 2 of the parking spaces are taken
		}
		else if(tmpA == 0x01 || tmpA == 0x02 || tmpA == 0x04 || tmpA == 0x08){
			cntavail = (cntavail & 0x00) | 0x03; //sets output to binary 3 if 1 of the parking spaces are taken
		}
		else if(tmpA == 0x00){
			cntavail = (cntavail & 0x00) | 0x04; //sets output to binary 4 if 0 of the parking spaces are taken
		}
		else{
			cntavail = (cntavail & 0x00) | 0xFF; //sets outputs as 1111 1111 if there is an error
		}
		
		//write output
		PORTC = cntavail;
	}
}

