/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #10 Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "timer.h"

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
}task;

task tasks[3];
const unsigned short tasksNum = 3;
volatile unsigned char threeLEDs = 0;
volatile unsigned char blinkLEDs = 0;

enum THREELED_STATES {TH_start,TH_ZeroOn, TH_OneOn, TH_TwoOn} THREELED_STATE;
int TickFct_ThreeLeds(int state) {
	switch (THREELED_STATE) {
	case TH_start:
		return TH_ZeroOn;
	break;
	
	case TH_ZeroOn:
		return TH_OneOn;
	break;
	
	case TH_OneOn:
		return TH_TwoOn;
	break;
	
	case TH_TwoOn:
		return TH_ZeroOn;
	break;
	}
	
	switch(THREELED_STATE) {
		case TH_ZeroOn:
			threeLEDs = 0x01;
		break;
		
		case TH_OneOn:
			threeLEDs = 0x02;
		break;
		
		case TH_TwoOn:
			threeLEDs = 0x04;
		break;
	}
};

enum BLINKINGLED_STATES {BL_start,  BL_ThreeOn, BL_ZeroOn} BLINKINGLED_STATE;
int TickFct_BlinkingLeds(int state) {
		switch (BLINKINGLED_STATE) {
			case BL_start:
			return BL_ThreeOn;
			break;
			
			case BL_ThreeOn:
			return BL_ZeroOn;
			break;
			
			case BL_ZeroOn:
			return BL_ThreeOn;
			break;
		}
		
		switch(BLINKINGLED_STATE) {
			case BL_ThreeOn:
			blinkLEDs = 0x08;
			break;
			
			case BL_ZeroOn:
			blinkLEDs = 0x01;
			break;
		}
};

enum COMBINELED_STATES {CO_start, CO_combine } COMBINELED_STATE;
int TickFct_Combineleds(int state) {
	switch (COMBINELED_STATE) {
		case CO_start:
		return CO_combine;
		break;
	}
	
	switch (COMBINELED_STATE) {
		case CO_combine:
		PORTB = threeLEDs | blinkLEDs;
		break;
	}
};
	


int main(void)
{
	/* Replace with your application code */
	while (1)
	{
	}
}

