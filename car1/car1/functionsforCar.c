#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "basicFunctions.h"
#include <util/delay.h>

#define HIGH 1
#define LOW 0

//void millis_init(void) {
	//ms = 0;
	//TCCR2A |= (1 << WGM21); // CTC mode
	//TCCR2B |= (1 << CS22);  // Prescaler = 64
	//OCR2A = 249;            // Compare value for 1ms (assuming 16MHz clock)
//
	//TIMSK2 |= (1 << OCIE2A); // Enable Timer2 Compare Match A interrupt
	//sei();                   // Enable global interrupts
//}
//ISR(TIMER2_COMPA_vect) {
	//ms++;
//}
//
//inline uint32_t millis(void) {
	//cli();
	//uint32_t ret = ms;
	//sei();
//
	//return ret;
//}



void digitalWrite(uint8_t pin, uint8_t value) {
	if (pin >= 0 && pin < 8) {
		if (value == 0) {
			PORTD &= ~(1 << pin);
			} else {
			PORTD |= (1 << pin);
		}
		} else if (pin >= 8 && pin < 16) {
		pin -= 8;
		if (value == 0) {
			PORTB &= ~(1 << pin);
			} else {
			PORTB |= (1 << pin);
		}
		} else {
		return;
	}
}

void pinMode(uint8_t pin, uint8_t value) {
	if (pin >= 0 && pin < 8) {
		if (value == 0) {
			DDRD &= ~(1 << pin);
			} else if(value == 1) {
			DDRD |= (1 << pin);
			} else {
			DDRD &= ~(1<<pin);
			PORTD |= (1<<pin);
		}
		} else if (pin >= 8 && pin < 16) {
		pin -= 8;
		if (value == 0) {
			DDRB &= ~(1 << pin);
			} else if(value == 1) {
			DDRB |= (1 << pin);
			} else {
			DDRB &= ~(1<<pin);
			PORTB |= (1<<pin);
		}
		} else {
		return;
	}
}

int digitalRead(int pin) {
	if (pin >= 0 && pin < 8) {
		if (PIND & (1 << pin)) {
			return HIGH;
			} else {
			return LOW;
		}
	} else if (pin >= 8 && pin < 14) {
		pin -= 8;
		if (PINB & (1 << pin)) {
			return HIGH;
			} else {
			return LOW;
		}
	} else {
		pin = 7;
		if (PINB & (1 << pin)) {
			return HIGH;
		} else {
			return LOW;
		}
	}
	return LOW;
}


unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout) {
	unsigned long width = 0; // To store pulse width in microseconds
	unsigned long start_time = 0;

	// Convert timeout to clock cycles
	unsigned long timeout_ticks = timeout * (F_CPU / 1000000UL);

	// Wait for the pulse to start
	start_time = 0;
	if (pin < 8)
	{
		while (((PIND & (1 << pin)) ? 1 : 0) != state) {
			if (++start_time >= timeout_ticks) {
				return 0; // Timeout
			}
			_delay_us(1); // Add delay for accurate timing
		}

		// Measure the pulse width
		width = 0;
		while (((PIND & (1 << pin)) ? 1 : 0) == state) {
			if (++width >= timeout_ticks) {
				return 0; // Timeout
			}
			_delay_us(1); // Add delay for accurate timing
		}
	} else {
		pin -= 8;
		while (((PINB & (1 << pin)) ? 1 : 0) != state) {
			if (++start_time >= timeout_ticks) {
				return 0; // Timeout
			}
			_delay_us(1); // Add delay for accurate timing
		}

		// Measure the pulse width
		width = 0;
		while (((PINB & (1 << pin)) ? 1 : 0) == state) {
			if (++width >= timeout_ticks) {
				return 0; // Timeout
			}
			_delay_us(1); // Add delay for accurate timing
		}
	}
	return width; // Return width in microseconds
}


void pwm_init(void) {

	// Configure Timer1 for PWM (Pins 9, 10)
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); // Fast PWM, 10-bit
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);    // Prescaler = 8
	ICR1 = 255; // TOP value for 8-bit resolution

	// Configure Timer0 for PWM (Pins 5, 6)
	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // Fast PWM
	TCCR0B = (1 << CS01); // Prescaler = 8
}