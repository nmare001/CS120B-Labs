/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #11 Exercise #4
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "io.c"


typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task; 

task tasks[3];
const unsigned short tasksNum = 3;
const unsigned long taskPeriodGCD = 100;
const unsigned long periodKeypad = 200;
const unsigned long periodLCDString = 500;
const unsigned long periodLCD = 200;


static unsigned char x; // used to get input from keypad
static unsigned char lcdOutput = ' '; // used to display output on LCD
static unsigned char lcdCursor = 0; // track location which is being written to on lcd
const unsigned char* lcdString = "Congratulations!";

enum KEYPAD_STATES {KEY_start,  KEY_Input};
int TickFct_Keypad(int state){
	
	switch(state){
		case KEY_start:
		state = KEY_Input;
		break;
		
		case KEY_Input:
		state = KEY_Input;
		break;
		
		default:
		break;
	}
	
	switch(state){
		case KEY_start:
		break;
		
		case KEY_Input:
			x = GetKeypadKey();
			
			//if (lcdCursor > 16){ lcdCursor = 1; } // reset cursor if end of lcdString has been reached
			
			switch(x) {
				case '\0': break;		//All 5 LEDs on
				case '1': lcdOutput = '1'; lcdCursor++; break;		//hex equivalent
				case '2': lcdOutput = '2'; lcdCursor++; break;
				
				
				//  . . . ?***** FINISH *****
				case '3': lcdOutput = '3'; lcdCursor++; break;
				case '4': lcdOutput = '4'; lcdCursor++; break;
				case '5': lcdOutput = '5'; lcdCursor++; break;
				case '6': lcdOutput = '6'; lcdCursor++; break;
				case '7': lcdOutput = '7'; lcdCursor++; break;
				case '8': lcdOutput = '8'; lcdCursor++; break;
				case '9': lcdOutput = '9'; lcdCursor++; break;
				case 'A': lcdOutput = 'A'; lcdCursor++; break;
				case 'B': lcdOutput = 'B'; lcdCursor++; break;
				case 'C': lcdOutput = 'C'; lcdCursor++; break;
				
				case 'D': lcdOutput = 'D'; lcdCursor++; break;
				case '*': lcdOutput = '*'; lcdCursor++; break;
				case '0': lcdOutput = '0'; lcdCursor++; break;
				case '#': lcdOutput = '#'; lcdCursor++; break;
				default: lcdOutput = 'R'; break;	// Should never occur.
			}
		break;
		
		default:
		break;
	}
	
	return state;
}

enum LCDSTRING_STATES {LCDSTRING_start,  LCDSTRING_Display, LCDSTRING_WAIT};
int TickFct_LCDSTRING(int state){
	
	switch(state){		//transitions
		case LCDSTRING_start:
		state = LCDSTRING_Display;
		break;
		
		case LCDSTRING_Display:
		state = LCDSTRING_WAIT;
		break;
		
		case LCDSTRING_WAIT:
		state = LCDSTRING_WAIT;
		break;
		
		default:
		break;
	}					//transitions
	
	
	switch(state){		//state actions
		case LCDSTRING_start:
		break;
		
		case LCDSTRING_Display:
	    LCD_DisplayString(1, lcdString);
		break;
		
		case LCDSTRING_WAIT:
		break;
		
		default:
		break;
	}					//state actions
	return state;
}

enum LCD_STATES {LCD_start,  LCD_Display};
int TickFct_LCD(int state){
	
	switch(state){		//transitions
		case LCD_start:
			state = LCD_Display;
		break;
		
		case LCD_Display:
			state = LCD_Display;
		break;
		
		default:
		break;
	}					//transitions
	
	
	switch(state){		//state actions
		case LCD_start:
		break;
		
		case LCD_Display:
		if (lcdCursor > 0)
		{
			LCD_Cursor(lcdCursor);
			LCD_WriteData(lcdOutput);
			
			if (lcdCursor > 15){ lcdCursor = 0; } // reset cursor if end of lcdString has been reached
		}	
		break;
		
		default:
		break;
	}					//state actions
	return state;
}



///////////////////////////////////////////////////////////////
////////////FUNCTIONS FOR THE TIMER////////////////////////////
///////////////////////////////////////////////////////////////

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B 	= 0x0B;	// bit3 = 1: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: prescaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A 	= 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register

	TIMSK1 	= 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1 = 0;

	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;

	//Enable global interrupts
	SREG |= 0x80;	// 0x80: 1000000
}

void TimerOff() {
	TCCR1B 	= 0x00; // bit3bit2bit1bit0=0000: timer off
}

/*void TimerISR() {
	TimerFlag = 1;
}*/

void TimerISR(){
	unsigned char i;
	for (i = 0; i <tasksNum; ++i){
		if ( tasks[i].elapsedTime >= tasks[i].period){
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += taskPeriodGCD;
	}
	//TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect)
{
	// CPU automatically calls when TCNT0 == OCR0 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; 			// Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { 	// results in a more efficient compare
		TimerISR(); 				// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}


int main(void)
{
    DDRA = 0xF0; PORTA = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	
	DDRC = 0xFF; PORTC = 0x00;	//LCD data lines
	DDRD = 0xFF; PORTD = 0x00;	//LCD control lines
	
	LCD_init();
	
	unsigned char i = 0;
	tasks[i].state = KEY_start;
	tasks[i].period = periodKeypad;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_Keypad;
	i++;
	tasks[i].state = LCDSTRING_start;
	tasks[i].period = periodLCDString;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_LCDSTRING;
	i++;
	tasks[i].state = LCD_start;
	tasks[i].period = periodLCD;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_LCD;
	
	TimerSet(taskPeriodGCD);
	TimerOn();
	
    while (1) 
    {
    }
}