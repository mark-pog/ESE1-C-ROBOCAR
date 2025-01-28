#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include "basicFunctions.h"
#include "CarFunctions.h"
#include <util/delay.h>
#define HIGH 1
#define LOW 0

// OCR0A - 6, OCR0B - 5, OCR1A - 9, OCR1B - 10

void driveForward(int speed) {
	OCR0A = speed;
	OCR0B = 0;
	OCR1B = 0;
	//digitalWrite(10, 0); // LOW
	//digitalWrite(5, 0);  // LOW
	OCR1A = speed;
	//PORTC0 &= ~(1<<0);
}

void driveRight(int speed) {
		//digitalWrite(11, 0);      // LOW
		OCR1B = 0;
		OCR1A = 0;
		OCR0A = speed;
		//analogWrite(6, 900);
		//digitalWrite(9, 0);       // LOW
		OCR0B = speed;
		//analogWrite(5, 930);
		//PORTC0 &= ~(1<<0);
}

void driveLeft(int speed) {
		OCR0A = 0;
		OCR0B = 0;
		OCR1A = speed;
		// analogWrite(11, 930);
		OCR1B = speed;
		// analogWrite(9, 900);
		//PORTC0 &= ~(1<<0);
}

void driveBack(int speed) {
	OCR0A = 0;
	//digitalWrite(6, 0); // LOW
	OCR0B = speed;
	OCR1B = speed;
	// analogWrite(11, speed);
	// analogWrite(5, speed);
	OCR1A = 0;
	//digitalWrite(9, 0); // LOW
	//PORTC0 |= (1<<0);
}

void stop() {
	OCR0A = 0;
	OCR0B = 0;
	OCR1A = 0;
	OCR1B = 0;
	//PORTC0 &= ~(1<<0);
}


float sensorValue(int pin) {
	// generate 10-microsecond pulse to TRIG pin
	digitalWrite(13, HIGH); // Assume rightTrigPin = PB4
	_delay_us(10); // Simulate delayMicroseconds(10)
	digitalWrite(13, LOW);

	// measure duration of pulse from ECHO pin
	float duration1 = pulseIn(pin, HIGH, 400); // Assume rightEchoPin = PB5

	// calculate the distance
	float distance = 0.087 * duration1;

	if (distance == 0) {
		distance = 700;
	}
	return distance;
}

//void sensorFrontValue() {
	//// generate 10-microsecond pulse to TRIG pin
	//digitalWrite(6, HIGH); // Assume frontTrigPin = PB6
	//_delay_us(10);
	//digitalWrite(6, LOW);
//
	//// measure duration of pulse from ECHO pin
	//duration2 = pulseIn(7, HIGH, 10000); // Assume frontEchoPin = PB7
//
	//// calculate the distance
	//distanceFront = 0.087 * duration2;
 //
	//if (distanceFront == 0) {
		//distanceFront = 700;
	//}
//
	//_delay_ms(30);
//}

//void sensorLeftValue(float *duration3,float *distanceLeft) {
	//// generate 10-microsecond pulse to TRIG pin
	//digitalWrite(3, HIGH); // Assume leftTrigPin = PB3
	//_delay_us(10);
	//digitalWrite(3, LOW);
//
	//// measure duration of pulse from ECHO pin
	//duration3 = pulseIn(2, HIGH, 10000); // Assume leftEchoPin = PB2
//
	//// calculate the distance
	//distanceLeft = 0.087 * duration3;
//
	//if (distanceLeft == 0) {
		//distanceLeft = 700;
	//}
//
	//_delay_ms(30);
//}