#include <avr/io.h>
#include <avr/interrupt.h>
#include "elapsed.h"

/* globals */

volatile unsigned long millis;
volatile unsigned int ticks;


/* init timer and interrupts for ms elapsed counter */

void timer1init(void) {
  OCR1B = 100;                    // this is for timing, with 8mhz cpu, /8 prescale, and OCR1B = 100, interrupt will trigger 10k times a second
  TIFR |= (1 << OCF1B);           // clear interrupt flags
  TIMSK |= (1 << OCIE1B);         // enable output compare interrupts
  TCCR1B = (0 << CS13) | (1 << CS12) | (0 << CS11) | (0 << CS10);           // start timer, /8 prescale
  sei(); 			  //enable interrupts
}


/* return how many milliseconds have elapsed since timer1init */  
long elapsed(void) {
	return millis;
}


/* interrupts */

ISR(TIMER1_CMPB_vect)
{
	ticks++;
	if(ticks % 10 == 0 ) { // 10000 ticks a second / 10 = 1 ms
  		millis++;
  		ticks = 0; // reset tick count
	}
}