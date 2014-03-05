#include <pololu/OrangutanDigital.h>
#include <pololu/OrangutanTime.h>

/*
 * led-run: for the Orangutan SVP.
 *
 * This program will run signals through pins A0 - A7 in a cycle.
 * LEDs placed in those port pins will make a nice show. 
 *
 */

int main() { 
	while(1) { 
		set_digital_output(IO_A0, HIGH); 
		delay_ms(100);
		set_digital_output(IO_A0, LOW);
		delay_ms(100);
		set_digital_output(IO_A1, HIGH); 
		delay_ms(100);
		set_digital_output(IO_A1, LOW);
		delay_ms(100);
		set_digital_output(IO_A2, HIGH); 
		delay_ms(100);
		set_digital_output(IO_A2, LOW);
		delay_ms(100);
		set_digital_output(IO_A3, HIGH); 
		delay_ms(100);
		set_digital_output(IO_A3, LOW);
		delay_ms(100);
		set_digital_output(IO_A4, HIGH); 
		delay_ms(100);
		set_digital_output(IO_A4, LOW);
		delay_ms(100);
		set_digital_output(IO_A5, HIGH); 
		delay_ms(100);
		set_digital_output(IO_A5, LOW);
		delay_ms(100);
		set_digital_output(IO_A6, HIGH); 
		delay_ms(100);
		set_digital_output(IO_A6, LOW);
		delay_ms(100);
		set_digital_output(IO_A7, HIGH); 
		delay_ms(100);
		set_digital_output(IO_A7, LOW);
		delay_ms(100);

	} 
}
 
