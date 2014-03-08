#include <avr/io.h>
#include <pololu/OrangutanTime.h>

/*
 * led-run-reg: for the Orangutan SVP.
 *
 * This program will run signals through pins A0 - A7 in a cycle.
 * LEDs placed in those port pins will make a nice show.
 *
 * This program is the same as led-run, but it is implemented
 * using just the AVR libraries without the Pololu libraries.
 * We will set values in the port A register and need to set
 * the data direction register for output.
 *
 * _SFR_IO8 is a macro defined in sfr_defs.h to make special function
 * register definitions look like C variables or simple constants.
 * 
 * These values are defined in iom1284p.h:
 *  PINA 	_SFR_IO8(0x00)	the pin input register for A
 *  DDRA 	_SFR_IO8(0x01)	the data direction register for A
 *  PORTA 	_SFR_IO8(0x02) 	the port data register for A
 *
 */

int main() { 

	int i;
	
	// set data direction to 'out' for all bits in this register
	DDRA |= 0xFF;
	
	while(1) { 
		// set the first bit to '1' to light the first
		PORTA = 1;
		
		// shift the bit along the register to light each of the pins in turn
		for(i = 0; i < 8; ++i) {
			delay_ms(100);
			PORTA <<= 1;
		}
	} 
}
 
