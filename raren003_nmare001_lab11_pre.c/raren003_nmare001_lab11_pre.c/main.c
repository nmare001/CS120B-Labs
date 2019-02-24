/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #11 Exercise #5
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include "io.c"

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[5];
const unsigned short tasksNum = 5;
const unsigned long taskPeriodGCD = 50;
const unsigned long periodObstacle = 350;
const unsigned long periodObstacle2 = 350;
const unsigned long periodObstacleRemove = 750;
const unsigned long periodPlayerMove = 350;
const unsigned long periodCollider = 350;

static unsigned char obstaclePosition = 8;
static unsigned char obstaclePosition2 = 32;
unsigned char playerPosition = 5;
unsigned char button = 0x00; //used to check which button is being pressed based on the value of
unsigned char collided = 0x00;
unsigned char* LCD_Output = "Crash GAME OVER";

enum OBSTACLE_STATES {Obst_start, Obst_main};
int TickFct_Obstacle(int state){
	
	switch (state){			//transitions
		case Obst_start:
			state = Obst_main;
			break;
			
		case  Obst_main:
			state = Obst_main;
			break;
	}						//transitions
	
	switch (state){			//state actions
		case Obst_start:
		break;
		
		case  Obst_main:
			LCD_Cursor(obstaclePosition);
			LCD_WriteData('#');
			
			if (obstaclePosition > 1)
			{
				obstaclePosition = obstaclePosition - 1;
			}else {
				obstaclePosition = 16;
			}
		break;
	}						//state actions
	
	return state;
}

enum OBSTACLE2_STATES {Obst2_start, Obst2_main};
int TickFct_Obstacle2(int state){
	
	switch (state){			//transitions
		case Obst2_start:
		state = Obst2_main;
		break;
		
		case  Obst2_main:
		state = Obst2_main;
		break;
	}						//transitions
	
	switch (state){			//state actions
		case Obst2_start:
		break;
		
		case  Obst2_main:
		LCD_Cursor(obstaclePosition2);
		LCD_WriteData('#');
		
		if (obstaclePosition2 > 17)
		{
			obstaclePosition2 = obstaclePosition2 - 1;
		}else {
			obstaclePosition2 = 32;
		}
		break;
	}						//state actions
	
	return state;
}

enum OBSTACLEREMOVE_STATES {ObstRemove_start, ObstRemove_main};
int TickFct_ObstacleRemove(int state){
	
	switch (state){			//transitions
		case ObstRemove_start:
		state = ObstRemove_main;
		break;
		
		case  ObstRemove_main:
		state = ObstRemove_main;
		break;
	}						//transitions
	
	switch (state){			//state actions
		case ObstRemove_start:
		break;
		
		case  ObstRemove_main:
		LCD_ClearScreen();
		break;
	}						//state actions
	
	return state;
}

enum Player_Move {PM_Start, PM_Wait, PM_Up, PM_Down};
int TickFct_KeyPad_Run(int state){
	switch (state) {
		case PM_Start:
		state = PM_Wait;
		break;
		
		case PM_Wait:
		if (button == 0x01) state = PM_Up;
		else if (button == 0x02) state = PM_Down;
		break;
		
		case PM_Up:
		state = PM_Wait;
		break;
		
		case PM_Down:
		state = PM_Wait;
		break;
	}
	switch (state) {
		case PM_Start:
		playerPosition = 5;
		LCD_Cursor(playerPosition);
		break;
		
		case PM_Wait:
		LCD_Cursor(playerPosition);
		break;
		
		case PM_Up:
		playerPosition = 5;
		LCD_Cursor(playerPosition);
		
		break;
		
		case PM_Down:
		playerPosition = 21;
		LCD_Cursor(playerPosition);
		
		break;
	}
	return state;
}

enum Collider {CO_start, CO_wait};
int TickFct_Collider(int state){
	
	switch (state){			//transitions
		case CO_start:
		state = CO_wait;
		break;
	}						//transitions
	
	switch (state) {
		case CO_wait:
		if (playerPosition == obstaclePosition) { collided = 1; LCD_DisplayString(1, LCD_Output); }
		else if (playerPosition == obstaclePosition2){ collided = 1; LCD_DisplayString(1, LCD_Output); }
		break;
	}
	
	return state;
}

enum Pause {P_Start, P_Wait, P_GameOver, P_GameReset};		
int TickFct_Pause(int state) {
	switch(state) {
		case P_Start:
		state = P_Wait;
		break;
	
		case P_Wait:
		if (collided == 1) state = P_GameOver;
		break;
		
		case P_GameOver:
		if (button == 8) state = P_GameReset;
		break;
		
		case P_GameReset:
		state = P_Wait;
		break;
	}
	
	switch(state) {
		case P_GameOver:
		LCD_DisplayString(1, "Game Over");
		break;
		
		case P_GameReset:
		collided = 0;
		obstaclePosition = 8;
		obstaclePosition2 = 32;
		playerPosition = 5;
		break;
		
	}
		
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
    
    DDRC = 0xFF; PORTC = 0x00;	//LCD data lines
    DDRD = 0xFF; PORTD = 0x00;	//LCD control lines
	
	LCD_init();
	
	unsigned char i = 0;
	tasks[i].state = Obst_start;
	tasks[i].period = periodObstacle;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_Obstacle;
	i++;
	tasks[i].state = Obst2_start;
	tasks[i].period = periodObstacle2;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_Obstacle2;
	i++;
	tasks[i].state = ObstRemove_start;
	tasks[i].period = periodObstacleRemove;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_ObstacleRemove;
	i++;
	tasks[i].state = PM_Start;
	tasks[i].period = periodPlayerMove;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_KeyPad_Run;
	i++;
	tasks[i].state = CO_start;
	tasks[i].period = periodCollider;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_Collider;
	
	TimerSet(taskPeriodGCD);
	TimerOn();
	
    while (1) 
    {
		button = ~PINA;
    }
}