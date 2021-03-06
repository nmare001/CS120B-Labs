/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #10 Exercise #3 W/struct task
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[3];
const unsigned short tasksNum = 4;
const unsigned long taskPeriodGCD = 2;
const unsigned long periodLightSeq = 400;
const unsigned long periodSoundSeq = 50;
const unsigned long periodCombineLED = 2;
const unsigned long periodSpeaker = 2;


static unsigned char threeLEDs = 0x00;
static unsigned char bilnkingLED =  0x00;
static unsigned char speakerOutput = 0x00;
//////////////////////////////////
static unsigned char lightPos = 0x80;
unsigned char sound1 = 0x00;
unsigned char sound2 = 0x00;
unsigned char sound3 = 0x00;
unsigned char soundPicked = 0x00;
//////////////////////////////////

enum LIGHTSEQ_STATES {LS_Start, LS_Play}E;
int TickFct_LightSeq(int state){
	
	switch(state){
		case LS_Start:
		state = LS_Play;
		break;
		
		case LS_Play:
		break;
		
		default:
		break;
	}
	
	switch(state){ // state actions
		case LS_Start:
		lightPos = 0x80;
		PORTB = lightPos;
		break;
		
		case LS_Play:
		if (lightPos > 0x01) {
			lightPos = lightPos / 2;
			PORTB = lightPos;
		}
		else {
			lightPos = 0x80;
			PORTB = lightPos;
			PORTC = 0x01;
		}
		
		break;
		
		default:
		break;
	}			//state actions
	return state;
}

enum SOUNDSEQ_STATES {SS_Start, SS_Run};
int TickFct_SoundSeq(int state){
	
	switch(state){
		case SS_Start:
		state = SS_Run;
		break;
		
		default:
		break;
	}
	
	switch(state){	//state actions
		case SS_Start:
		break;
		
		case SS_Run:
		//PORTD = 0xFF;
		break;
		
		default:
		break;
	}
	return state;
}

enum COMBINELED_STATES {CO_start, CO_combine };
int TickFct_CombineLeds(int state){
	
	switch(state){
		case CO_start:
		state =  CO_combine;
		break;
		
		case CO_combine:
		state = CO_combine;
		break;
		
	}
	
	switch(state){
		case CO_start:
		break;
		
		case CO_combine:
		//PORTB = threeLEDs | bilnkingLED;
		break;
		
	}
	
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






enum SPEAKER_STATES {SP_start, SP_wait, SP_on};
int TickFct_Speaker(int state){
	switch(state){
		case SP_start:
		state =  SP_start;
		break;
		
		case SP_wait:
		break;
		
		case SP_on:
		break;
		
		default:
		break;
	}
	
	switch(state){
		case SP_start:
		break;
		
		case SP_wait:
		break;
		
		case  SP_on:
		default:
		break;
		
	}
	
	return state;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //set PORTA as input
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char i = 0;
	tasks[i].state = LS_Start;
	tasks[i].period = periodLightSeq;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_LightSeq;
	i++;
	tasks[i].state = SS_Start;
	tasks[i].period = periodSoundSeq;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_SoundSeq;
	i++;
	tasks[i].state = CO_start;
	tasks[i].period = periodCombineLED;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_CombineLeds;
	i++;
	tasks[i].state = SP_start;
	tasks[i].period = periodSpeaker;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_Speaker;
	
	
	TimerSet(taskPeriodGCD);
	TimerOn();
	
	while (1)
	{
	}
}

