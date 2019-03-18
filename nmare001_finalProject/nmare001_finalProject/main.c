/* Partner(s) Name & E-mail:Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Final Project
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
//periods 
const unsigned short tasksNum = 3;
const unsigned long taskPeriodGCD = 50;
const unsigned long periodLightSeq = 150;
const unsigned long periodSoundSeq = 50;
const unsigned long periodInstrumentSel = 50;

//////////////////////////////////
//variables for functions
static unsigned char lightPos = 0x80;
unsigned char sound0 = 0x00; //keeps track of sound0 sequence
unsigned char sound1 = 0x00; //keeps track of sound1 sequence
unsigned char sound2 = 0x00; //keeps track of sound2 sequence
unsigned char soundt = 0x00; 
unsigned char soundPicked = 0x00; 
unsigned char tmpC = 0x00;
unsigned char button1 = 0x00;
unsigned char SO_cnt = 0x00;
unsigned char instrument = 0x00;//which sound is being programmed
unsigned char sound1Check = 0x01;
unsigned char sound0Check = 0x00;
unsigned char lsP = 3; //which multiple of 150ms does each step last
unsigned char lsCnt = 0;
//////////////////////////////////

enum LIGHTSEQ_STATES {LS_Start, LS_Play}; //outputs the sequence of sounds
     int TickFct_LightSeq(int state){ 
	
	switch(state){
		case LS_Start:
		state = LS_Play;
		break;
		
		state = LS_Play;
		PORTC = 0xFF;
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
		if (lsCnt >= lsP) {
			lsCnt = 0;
		if (lightPos > 0x01) {
			lightPos = lightPos / 2;
			PORTB = lightPos;
		}
		else {
			lightPos = 0x80;
			PORTB = lightPos;
			//PORTD = 0xFF;
		}
		if (lightPos & sound0) PORTC = 0xFE;
		else if (lightPos & sound1) PORTC = 0xFD;
		else if (lightPos & sound2) PORTC = 0xFB;
		else PORTC = 0xFF;
		}
		else PORTC = 0xFF;
		++lsCnt;
		
		
		
		break;
		
		default:
		break;
	}			//state actions
	return state;
}

enum SOUNDSEQ_STATES {SS_Start, SS_Wait, SS_Press, SS_Release}; //user programs sequence
int TickFct_SoundSeq(int state){
	
	switch(state){
		case SS_Start:
		state = SS_Wait;
		break;
		
		case SS_Wait:
		if (button1) {
			state = SS_Press;
		}
		break;
		
		case SS_Press:
		if (instrument == 0) soundt = sound0;
		else if (instrument == 1) soundt = sound1;
		else if (instrument == 2) soundt = sound2;
		state = SS_Release;
		
		break;
		
		case SS_Release:
		if (!button1) {
			state = SS_Wait;
		}
		break;
		
		default:
		break;
	}
	
	switch(state){	//state actions
		case SS_Start:
		break;
		
		case SS_Wait:
		break;
		
		case SS_Press:
		if (instrument == 0) soundt = sound0;
		else if (instrument == 1) soundt = sound1;
		else if (instrument == 2) soundt = sound2;
		if ((button1 & 0x80) && (button1 & 0x40)) soundt = soundt;
		else if ((button1 & 0x01) && (button1 & 0x02)) soundt = soundt;
		else {
		if (button1 & 0x80) {
			if (soundt & 0x80) soundt = soundt - 0x80;
			else soundt = soundt + 0x80;
		}
		if (button1 & 0x40) {
			if (soundt & 0x40) soundt = soundt - 0x40;
			else soundt = soundt + 0x40;
		}
		if (button1 & 0x20) {
			if (soundt & 0x20) soundt = soundt - 0x20;
			else soundt = soundt + 0x20;
		}
		if (button1 & 0x10) {
			if (soundt & 0x10) soundt = soundt - 0x10;
			else soundt = soundt + 0x10;
		}
		if (button1 & 0x08) {
			if (soundt & 0x08) soundt = soundt - 0x08;
			else soundt = soundt + 0x08;
		}
		if (button1 & 0x04) {
			if (soundt & 0x04) soundt = soundt - 0x04;
			else soundt = soundt + 0x04;
		}
		if (button1 & 0x02) {
			if (soundt & 0x02) soundt = soundt - 0x02;
			else soundt = soundt + 0x02;
		}
		if (button1 & 0x01) {
			if (soundt & 0x01) soundt = soundt - 0x01;
			else soundt = soundt + 0x01;
		}
		}
		if (instrument == 0) sound0 = soundt;
		else if (instrument == 1) sound1 = soundt;
		else if (instrument == 2) sound2 = soundt;
		break;
		
		case SS_Release:
		break;
		
		
		default:
		break;
	}
	return state;
}

enum INSTRUMENT_SEL {IS_Start, IS_Wait, IS_Press, IS_Release, BPM_Inc, BPM_Dec }; //user changes sound to program or bpm of the sequence
int TickFct_InstrumentSel(int state){
	
	switch(state){
		case IS_Start:
		state = IS_Wait;
		break;
		
		case IS_Wait:
		if ((button1 & 0x80) && (button1 & 0x40)) state = IS_Press;
		else if ((button1 & 0x01) && (button1 & 0x02)) state = BPM_Inc;
		else if ((button1 & 0x01) && (button1 & 0x04)) state = BPM_Dec;
		break;
		
		case IS_Press:
		state = IS_Release;
		break;
		
		case BPM_Inc:
		state = IS_Release;
		break;
		
		case BPM_Dec:
		state = IS_Release;
		
		case IS_Release:
		if (!button1) state = IS_Wait;
		break;
		
	}
	
	switch(state){
		case IS_Start:
		break;
		
		case IS_Wait:
		if (instrument == 0) PORTD = sound0;
		else if (instrument == 1) PORTD = sound1;
		else if (instrument == 2) PORTD = sound2;
		break;
		
		case IS_Press:
		if (instrument < 2) ++instrument;
		else instrument = 0;
		break;
		
		case BPM_Inc:
		if (lsP > 1) --lsP;
		break;
		
		case BPM_Dec:
		if (lsP < 5) ++lsP;
		break;
		
		case IS_Release:
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








int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //set PORTA as input
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	DDRC = 0xFF; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	//PORTC = 0xFF;
	button1 = ~PINA;
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
	tasks[i].state = IS_Start;
	tasks[i].period = periodInstrumentSel;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_InstrumentSel;
	i++;
	
	
	
	TimerSet(taskPeriodGCD);
	TimerOn();
	
	while (1)
	{
		button1 = ~PINA;
	}
}


