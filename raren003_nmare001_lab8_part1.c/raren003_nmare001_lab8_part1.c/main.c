/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #8 Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//		  in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	DDRD = 0xFF; PORTD = 0x00; //set PORTD as output initialize to 0
	
	unsigned short us_ADC; //variable to read 10-bit ADC result
	unsigned char tmpb; //hold out put to be displayed on PORTB
	unsigned char tmpd; //hold out put to be displayed on PORTD
	
	ADC_init(); //initialize A/D converter
	
	
	/* Replace with your application code */
	while (1)
	{
		us_ADC = ADC;
		tmpb = (char)us_ADC; //tmpb = 0xCD
		tmpd = (char)us_ADC >> 4; //tmpd = 0xBC
		
		PORTB = tmpb;
		PORTD = tmpd;
	}
}

