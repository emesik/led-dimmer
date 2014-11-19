#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "trans.h"

/*****************************************************************************
 *
 * LED driver
 *
 *	PB1 (OC0B) - PWM output
 *	PB4 (ADC2) - potentiometer input
 *
 *	The PWM output is reverse of the input voltage from potentiometer.
 *	For 0V the duty cycle will be 100%, for Vcc it will be 0%.
 *	That allows for full force work when no potentiometer is present.
 *
 ****************************************************************************/


uint8_t read_potentiometer() {
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADCH;
}

int main() {
	DDRB |= (1 << PB1);		// set PB1 as output
	DDRB &= ~(1 << PB4);	// set PB4 as input
	PORTB &= ~(1 << PB1);	// set PB1 low

	// phase correct PWM, inverting, no prescaler
	TCCR0A = (1 << COM0B1) | (1 << COM0B0) | (1 << WGM00);
	TCCR0B = (1 << CS00);
	OCR0B = 0xff;	// 0% duty cycle

	ADCSRA = (1 << ADEN)	// enable ADC
		| (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// set 128 prescaler
	ADMUX = (1 << ADLAR) | (1 << MUX1);	// ADC2 (PB4) as input, 8bit precision

#ifdef DEBUG
	for (uint8_t i=0xff; i>0; i--) {
		OCR0B = i;
		_delay_ms(100);
	}
#endif

	while (1) {
		OCR0B = 0xff - pot2pwm[read_potentiometer()];
	}
	return 0;
}
