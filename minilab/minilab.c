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

#define RED_IO		IO_B3
#define YELLOW_IO	IO_A3
#define GREEN_IO	IO_A7
#define OCR_TOP		2500
#define RED_TOP 	500
#define YELLOW_TOP	250

// need global variables to communicate between the ISR and the main program
static volatile short redRelease_ms;
static volatile short yellowRelease_ms;
static volatile short releaseRedToggle;
static volatile short releaseYellowPoll;

int main() { 
	
	// Set control register bits for 'mode 4' to use OCR3A with CTC timer/counter mode
	TCCR3B |= 1 << WGM32; 
	
	// set the clock tick prescaler bits to use clkio/8
	TCCR3B |= 1 << CS31;  
	
	/* OCIE3A: Timer/Counter3, Output Compare A Match Interrupt Enable
	 * When this bit is written to one, the Timer/Counter3 Output Compare A 
	 * Match interrupt is enabled. */
	TIMSK3 |= 1 << OCIE3A;
	
	// set the timer3 counter compare register to fire when it reaches this value
	OCR3A = OCR_TOP;
	
	// enable interrupts on all PCINT23:16 I/O pins
	PCICR |= 1 << PCIE2;
	
	// mask the interrupts on PCINT23:16 so we only have PCINT21 (TOP_BUTTON)
	PCMSK2 |= 1 << PCINT21;
	
	// set the I-flag in the Status Register (interrupts globally enabled), 
	sei();
	
	while (1) {
		if (releaseRedToggle) {
			set_digital_output(RED_IO, TOGGLE);
			releaseRedToggle = 0;
		}
		if (releaseYellowPoll) {
			// see if the bottom button is depressed as we are polling it
			if (get_single_debounced_button_press(BOTTOM_BUTTON))
			set_digital_output(YELLOW_IO, TOGGLE);
			releaseYellowPoll = 0;
		}
	}
}	
	
// 'ISR' special construct to create an interrupt service routines

// 'TIMER3_COMPA_vect' is what ties this function to the Timer3 OC3A interrupt
ISR(TIMER3_COMPA_vect) {
	
	redRelease_ms++;
	if (redRelease_ms >= RED_TOP) {
		releaseRedToggle = 1;
		// reset the red timer
		redRelease_ms = 0;
	}
	
	yellowRelease_ms++;
	if (yellowRelease_ms >= YELLOW_TOP) { 
		releaseYellowPoll = 1; 
		// reset the yellow polling timer
		yellowRelease_ms = 0;
	}
}

// Interrupt service routine for any PCINT23:16
ISR(PCINT2_vect) {
	// The top button must have been pushed, so toggle the portpin
	if (button_is_pressed(TOP_BUTTON)) {
		set_digital_output(GREEN_IO, TOGGLE);
	}
}
