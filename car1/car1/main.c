#define F_CPU 16000000UL
#define SafeDistance 23
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "CarFunctions.h"
#include "basicFunctions.h"




int workMode = 0;
int irSensor1 = 0;
int irsensor2 = 0;
//volatile uint8_t speed = 200;
int frontEchoPin = 8;  // ECHO pin
int rightEchoPin = 4;  // ECHO pin
int leftEchoPin = 7;   // ECHO pin
int leftTrigPin = 13;  // TRIG pin
int frontTrigPin = 13;
int rightTrigPin = 13;
unsigned long previousMillis = 0;
unsigned long previousLCDMillis = 0;
unsigned long previousButtonMillis = 0;
int interval = 1000;
char command;
float distanceFront, distanceRight, distanceLeft, duration1, duration2, duration3;

void UART_Init(unsigned long baudrate) {
	// Set the baud rate
	unsigned int ubrr = F_CPU / 16 / baudrate - 1;
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	// Enable transmitter
	UCSR0B = (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

int UART_Transmit(char data, FILE *stream) {
	if (data == '\n') {
		UART_Transmit('\r', stream);  // Add carriage return before newline
	}

	while (!(UCSR0A & (1 << UDRE0)));  // Wait for the transmit buffer to be empty
	UDR0 = data;  // Load the data into the buffer
	return 0;
}

// Create a FILE stream for UART
FILE uart_output = FDEV_SETUP_STREAM(UART_Transmit, NULL, _FDEV_SETUP_WRITE);




void pwm_init(void) {

	// Configure Timer1 for PWM (Pins 9, 10)
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); // Fast PWM, 10-bit
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);    // Prescaler = 8
	ICR1 = 255; // TOP value for 8-bit resolution

	// Configure Timer0 for PWM (Pins 5, 6)
	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // Fast PWM
	TCCR0B = (1 << CS01); // Prescaler = 8
}

void printDistances() {
	printf("distances: ");
	printf("right %.2f", distanceRight);
	printf("---");
	printf("front %.2f", distanceFront);
	printf("---");
	printf("left %.2f", distanceLeft);
	printf(" cm\n");
}


int main() {
	UART_Init(9600);  // Initialize UART at 9600 baud
	stdout = &uart_output;  // Redirect stdout to UART
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pwm_init();
	pinMode(20, INPUT_PULLUP);
	pinMode(frontTrigPin, OUTPUT);
	pinMode(frontEchoPin, INPUT);
	pinMode(rightTrigPin, OUTPUT);
	pinMode(rightEchoPin, INPUT);
	pinMode(leftTrigPin, OUTPUT);
	pinMode(leftEchoPin, INPUT);
	pinMode(12, INPUT);
	pinMode(11, INPUT);
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);
	pinMode(7, INPUT);
	pinMode(8, INPUT);
	pinMode(13, OUTPUT);
	while (1)
	{
		// 70 starts to run
		//unsigned long currentMillis = millis();
		if (workMode == 0) {
			//if (currentMillis - previousMillis >= 100) {
				distanceRight = sensorValue(rightEchoPin);
				_delay_ms(100);
				distanceFront = sensorValue(frontEchoPin);
				_delay_ms(100);
				distanceLeft = sensorValue(leftEchoPin);
				_delay_ms(100);
				printDistances();
				
				if (distanceFront < SafeDistance) {
					if (distanceLeft < SafeDistance && distanceRight < SafeDistance) {
						driveBack();
						} else {
						if (distanceLeft > distanceRight) {
							driveRight();
							} else if (distanceLeft < distanceRight) {
							driveLeft();
						}
					}
					} else if (distanceLeft < SafeDistance) {
					driveLeft();
					} else if (distanceRight < SafeDistance) {
					driveRight();
					} else {
					driveForward();
				}
				//showlcd();
			//}
		}
		//if (mode == 1) {
		//speed = 317;
		//if (digitalRead(2) == 1) {
		//driveRight();
		//_delay_ms(300);
		//} else if (digitalRead(3) == 1) {
		//driveLeft();
		//_delay_ms(300);
		//} else if (digitalRead(12) == 1) {
		//driveRight();
		//} else if (digitalRead(10) == 1) {
		//driveLeft();
		//} else {
		//driveForward();
		//}
		//// showlcd();
		//}
		//
		//if (mode == 2) {
		//// showlcd();
		//// Check if there's incoming data from Bluetooth (phone)
		//// if (BTSerial.available()) {
		////     command = BTSerial.read();  // Read the incoming command
		////     Serial.print("Received command: ");
		////     Serial.println(command);
		////     Serial.println(speed);
		////     showlcd();
		////
		////     // Process the command to control the robot car
		////     controlRobot(command);
		//// }
		//
		//// If data is entered in the Serial Monitor, send it to Bluetooth (useful for debugging)
		//// if (Serial.available()) {
		////     char outgoingData = Serial.read();
		////     BTSerial.write(outgoingData);  // Send to phone
		//// }
		//}
		//
		//if (digitalRead(20) == 0 /* && currentMillis - previousButtonMillis >= interval */) {
		//mode++;
		//// showlcd();
		//// Serial.println(mode);
		//// previousButtonMillis = currentMillis;
		//}
		//if (mode == 3) {
		//mode = 0;
		//}
	}
}


//void increaseSpeed() {
//	if (speed < 495) {
//		speed += 25;
//	}
//}

//void decreaseSpeed() {
//	if (speed > 0) {
//		speed -= 25;
//	}
//}

// Control robot based on command
//void controlRobot(char cmd) {
//switch (cmd) {
//case 'F':  // Move Forward
//driveForward();
//break;
//case 'B':  // Move Backward
//driveBack();
//break;
//case 'L':  // Turn Left
//driveRight();
//break;
//case 'R':  // Turn Right
//driveLeft();
//break;
//case 'S':  // Stop
//stop();
//break;
//case '+':  // Increase Speed
////increaseSpeed();
//break;
//case '-':  // Decrease Speed
////decreaseSpeed();
//break;
//default:
//printf("Invalid command\n");
//break;
//}
//}



