/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #3 Exercise #5
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/


#include <avr/io.h>


enum States {Start, FIRST_ON, SECOND_ON} state;

void Tick() {
	switch(Start) {
		state = FIRST_ON;
	}
	switch(FIRST_ON) {
		if (PINA == 1) state = SECOND_ON;
	}
	switch(SECOND_ON) {
		if (PINA == 0) state == FIRST_ON;
	}
	switch(FIRST_ON) {
		PORTB = 1;
	}
	switch (SECOND_ON) {
		PORTB = 2;
	}
}

void main() {
	PORTB = 0x00;
	state = Start;
	while(1) {Tick();}
}


