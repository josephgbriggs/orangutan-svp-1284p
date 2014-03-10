#include <pololu/orangutan.h>

/*
 * minilab: for the Orangutan SVP.
 *
 * Set up Timer3 to generate interrupts at 1000Hz (i.e. one every 1ms). Inside
 * the ISR, check for the release of tasks. In this case, we will blink the 
 * red LED light on the board.
 *
 * Processor runs at 20MHz -> 20,000,000 ticks / sec
 * 
 *      MCU CLOCK  * PRESCALER *   1/TOP    =   1000 Hz
 * 
 * 20,000,000 ticks   1 count      1 ISR      1000 ISRs
 * ---------------- * ------- * ----------- = --------- = 1000 Hz
 *       1 sec        8 ticks   2500 counts     1 sec
 */

// need global variables to communicate between the ISR and the main program
static volatile int iterationCounter;
static volatile short ledState;
static volatile short readyFlag;

int main() { 
	
	// Set control register bits for 'mode 4' to use OCR3A with CTC timer/counter mode
	TCCR3B |= 1 << WGM32; 
	
	// set the clock tick prescaler bits to use clkio/8
	TCCR3B |= 1 << CS31;  
	
	/* OCIE3A: Timer/Counter3, Output Compare A Match Interrupt Enable
	 * When this bit is written to one, the Timer/Counter3 Output Compare A 
	 * Match interrupt is enabled. */
	TIMSK3 |= 1 << OCIE3A;
	
	// set the timer3 counter compare register to fire the interrupt every '2500' counts
	OCR3A = 2500;
	
	// set the I-flag in the Status Register (interrupts globally enabled), 
	sei();
	
	while (1) {
		if (readyFlag) {
			ledState = (ledState == 1) ? 0 : 1;
			red_led(ledState);
			readyFlag = 0;
		}
	}
}	
	
// 'ISR' special construct to create an interrupt service routine
// 'TIMER3_COMPA_vect' is what ties this function to the Timer3 OC3A interrupt
ISR(TIMER3_COMPA_vect) {
	if (iterationCounter >= 1000) {
		readyFlag = 1;
		iterationCounter = 0;
	}
	iterationCounter++;
}

 
