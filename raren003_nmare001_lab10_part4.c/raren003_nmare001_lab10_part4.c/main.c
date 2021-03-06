/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #10 Exercise #5
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

task tasks[4];
const unsigned short tasksNum = 4;
const unsigned long taskPeriodGCD = 2;
const unsigned long periodInputLED = 4;
const unsigned long periodAutoSlow = 1000;
const unsigned long periodAutoFast = 400;
const unsigned long periodCombineLED = 2;
const unsigned long periodSpeaker = 2;
static unsigned char SpeakerFreq = 0;


static unsigned char threeLEDs = 0x00;
static unsigned char bilnkingLED =  0x00;
static unsigned char speakerOutput = 0x00;

static unsigned char input_led = 0x00;
static unsigned char counterInc = 0x00;
static unsigned char counterDec = 0x00;

static unsigned char autoSlowVal = 0x00;
static unsigned char autoSlowCount = 0;

unsigned char button = 0x00; //used to check which button is being pressed based on the value of
enum INPUT_STATES {input_start, input_wait, input_inc, input_dec, input_reset};
int TickFct_INPUT_STATES(int state){
	
	switch(state){
		
		case input_start:
		state = input_wait;
		break;
		
		case  input_wait:
		if (button == 3) state = input_reset;
		else if (button == 1 && (input_led + autoSlowVal < 9)) {
			++input_led;
			state = input_inc;
		}
		else if (button == 2 && (input_led + autoSlowVal >= 1))  {
			--input_led;
			state = input_dec;
		}
		break;
		
		case input_inc:
		if (button == 3) state = input_reset;
		else if (button == 0) state = input_wait;
		break;
		
		case  input_dec:
		if (button == 3) state = input_reset;
		else if (button == 0) state = input_wait;
		break;
		
		case input_reset:
		state = input_wait;
		break;
		
		default:
		break;
	}
	
	switch(state){ // state actions
		case input_start:
		input_led = 0x00;
		break;
		
		case input_wait:
		//input_led = PORTB;
		counterInc = 0;
		counterDec = 0;
		break;
		
		case input_inc:
		++counterInc;
		break;
		
		case input_dec:
		++counterDec;
		break;
		
		case input_reset:
		input_led = 0x00;
		autoSlowVal = 0;
		break;
		
		default:
		break;
	}			//state actions
	return state;
}

enum AutoSlow {auto_start, auto_change};
int TickFct_AutoSlow(int state){
	
	switch(state){
		case auto_start:
		state = auto_change;
		break;
		
		case auto_change:
		break;
		
		default:
		break;
	}
	
	switch(state){	//state actions
		case auto_start:
		break;
		
		case auto_change:
		if (counterInc > 2 && ((autoSlowVal + input_led) < 9) && autoSlowCount < 4) ++autoSlowVal, ++autoSlowCount;
		else if (counterDec > 2 && ((autoSlowVal + input_led) >= 1) && autoSlowCount < 4) --autoSlowVal, ++autoSlowCount;
		else if (counterDec < 2 && counterInc < 2) autoSlowCount = 0;
		break;
		
		
		default:
		break;
	}
	return state;
}

enum AutoFast {autoF_start, autoF_change};
int TickFct_AutoFast(int state){
	
	switch(state){
		case autoF_start:
		state = autoF_change;
		break;
		
		case autoF_change:
		break;
		
		default:
		break;
	}
	
	switch(state){	//state actions
		case autoF_start:
		break;
		
		case autoF_change:
		if (counterInc > 2 && ((autoSlowVal + input_led) < 9) && autoSlowCount > 3) ++autoSlowVal;
		else if (counterDec > 2 && ((autoSlowVal + input_led) >= 1) && autoSlowCount > 3) --autoSlowVal;
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
		PORTB = input_led + autoSlowVal;
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


// 0.954 hz is the lowest frequency possible with this function,
// based on settings in PWM_on
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency){
		if (!frequency){ TCCR0B &= 0x08; } // stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A =  0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250){ OCR0A = 0x0000;}
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }
		
		TCNT0 = 0; //reset counter
		current_frequency = frequency; // Updates the current frequency
	}
	
}

void PWM_on() {
	TCCR0A = (1 << COM0A0) | (1 << WGM00);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //set PORTA as input
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	
	//PWM_on();
	
	unsigned char i = 0;
	tasks[i].state = input_start;
	tasks[i].period = periodInputLED;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_INPUT_STATES;
	i++;
	tasks[i].state = auto_start;
	tasks[i].period = periodAutoSlow;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_AutoSlow;
	i++;
	tasks[i].state = autoF_start;
	tasks[i].period = periodAutoFast;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_AutoFast;
	i++;
	tasks[i].state = CO_start;
	tasks[i].period = periodCombineLED;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_CombineLeds;
	i++;

	
	
	TimerSet(taskPeriodGCD);
	TimerOn();
	
	while (1)
	{
		button = ~PINA;
	}
}


