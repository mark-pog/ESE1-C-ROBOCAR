#define F_CPU 16000000UL
#define SafeDistance 23
#define FrontSafeDistance 40
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "lcd.h"
extern void lcd_backlight(char on);    //not in lcd.h
#include "CarFunctions.h"
#include "basicFunctions.h"



int i = 0;
int workMode = 2;
int irSensor1 = 0;
int irsensor2 = 0;
volatile char command = 0;
volatile uint8_t speed = 200;
int frontEchoPin = 8;  // ECHO pin
int rightEchoPin = 4;  // ECHO pin
int leftEchoPin = 7;   // ECHO pin
// TRIG pin
int frontTrigPin = 13;
unsigned long previousMillis = 0;
unsigned long previousLCDMillis = 0;
unsigned long previousButtonMillis = 0;
int interval = 1000;
float distanceFront, distanceRight, distanceLeft, duration1, duration2, duration3;


// Initialize UART for Bluetooth communication and debugging

// UART RX Interrupt Service Routine


void UART_Init(unsigned long baudrate) {
	// Set the baud rate
	unsigned int ubrr = F_CPU / 16 / baudrate - 1;
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	// Enable transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); 

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

ISR(USART_RX_vect) {
	command = UDR0; // Read received data
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





void printDistances() {
	printf("distances: ");
	printf("right %.2f", distanceRight);
	printf("---");
	printf("front %.2f", distanceFront);
	printf("---");
	printf("left %.2f", distanceLeft);
	printf(" cm\n");
}

volatile uint32_t ms = 0;

void millis_init(void) {
	ms = 0;
	TCCR2A |= (1 << WGM21); // CTC mode
	TCCR2B |= (1 << CS22);  // Prescaler = 64
	OCR2A = 249;            // Compare value for 1ms (assuming 16MHz clock)

	TIMSK2 |= (1 << OCIE2A); // Enable Timer2 Compare Match A interrupt
	sei();                   // Enable global interrupts
}

ISR(TIMER2_COMPA_vect) {
	ms++;
}

inline uint32_t millis(void) {
	cli();
	uint32_t ret = ms;
	sei();

	return ret;
}

void checkCommand() {
	if (command) {
		printf("Received Command: %c\n", command);
		switch (command) {
			case 'F': driveForward(speed); break;
			case 'B': driveBack(speed); break;
			case 'L': driveLeft(speed); break;
			case 'R': driveRight(speed); break;
			case 'S': stop(); break;
			//case '+': increaseSpeed(); break;
			//case '-': decreaseSpeed(); break;
			default: printf("Invalid Command\n"); break;
		}
		command = 0; // Reset command after execution
	}
}



int main() {
	lcd_init(LCD_ON_DISPLAY);
	millis_init();
	UART_Init(9600);  // Initialize UART at 9600 baud
	stdout = &uart_output;  // Redirect stdout to UART
	pinMode(5, OUTPUT);    //motor pins
	pinMode(6, OUTPUT);    //
	pinMode(9, OUTPUT);    //
	pinMode(10, OUTPUT);   //
	pwm_init();
	pinMode(20, INPUT_PULLUP); // button pin
	pinMode(frontTrigPin, OUTPUT);    //   Ultrasonic sensors pins
	pinMode(frontEchoPin, INPUT);    //
	pinMode(rightEchoPin, INPUT);    //
	pinMode(leftEchoPin, INPUT);    //
	pinMode(12, INPUT);     // IR sensors pin
	pinMode(11, INPUT);    //
	pinMode(2, INPUT);    //
	pinMode(3, INPUT);    //
	DDRC |= (1<<0);
	lcd_clrscr();
	lcd_gotoxy(0, 0);
	lcd_puts_P("DADAMS CO.");
	lcd_gotoxy(1, 1);
	lcd_puts("PRESENTS");
	while (1)
	{
		// 70 starts to run
	
		unsigned long currentMillis = millis();
		if (workMode == 0) {
			int speedUS = 0;
			if (currentMillis - previousMillis >= 100) {
				distanceRight = sensorValue(rightEchoPin);
				_delay_ms(10);
				distanceFront = sensorValue(frontEchoPin);
				_delay_ms(10);
				distanceLeft = sensorValue(leftEchoPin);
				_delay_ms(10);
				printDistances();
				
				if (distanceFront < FrontSafeDistance) {
					if (distanceLeft < SafeDistance && distanceRight < SafeDistance) {
						driveBack(speedUS);
						} else {
						if (distanceLeft > distanceRight) {
							driveLeft(speedUS);
							} else if (distanceLeft < distanceRight) {
							driveRight(speedUS);
						}
					}
					} else if (distanceLeft < SafeDistance) {
					driveRight(speedUS);
					} else if (distanceRight < SafeDistance) {
					driveLeft(speedUS);
					} else {
					driveForward(speedUS);
				}
				//lcd_clrscr();
				//lcd_gotoxy(0, 0);
				//lcd_puts("Ultrasonic");
				previousMillis = currentMillis;
			}
		}
		
		
		
		
		if (workMode == 1) {
			if (digitalRead(2) == 1) {
				driveLeft(100);
				_delay_ms(300);
			} else if (digitalRead(3) == 1) {
				driveRight(100);
				_delay_ms(300);
			} else if (digitalRead(12) == 1) {
				driveLeft(100);
			} else if (digitalRead(10) == 1) {
				driveRight(100);
			} else {
				driveForward(100);
			}
			i = 0;
			//lcd_clrscr();
			//lcd_gotoxy(0, 0);
			//lcd_puts("IR");
		}
		
		
		
		
		
		if (workMode == 2) {
			if (i == 0)
			{
				stop();
				i++;
			}
			//lcd_clrscr();
			//lcd_gotoxy(0, 0);
			//lcd_puts("BLuetooth");
			checkCommand();
		// showlcd();
		// Check if there's incoming data from Bluetooth (phone)
		// if (BTSerial.available()) {
		//     command = BTSerial.read();  // Read the incoming command
		//     Serial.print("Received command: ");
		//     Serial.println(command);
		//     Serial.println(speed);
		//     showlcd();
		//
		//     // Process the command to control the robot car
		//     controlRobot(command);
		// }
		
		// If data is entered in the Serial Monitor, send it to Bluetooth (useful for debugging)
		// if (Serial.available()) {
		//     char outgoingData = Serial.read();
		//     BTSerial.write(outgoingData);  // Send to phone
		// }
		}
		
		if (digitalRead(20) == 0  && currentMillis - previousButtonMillis >= interval) {
			workMode++;
		// showlcd();
		// Serial.println(mode);
			previousButtonMillis = currentMillis;
			printf("%d", workMode);
			lcd_clrscr();
			lcd_gotoxy(0,1);
			if (workMode == 0 || workMode == 3){
				lcd_puts(" Ultrasonic");
			} else if (workMode == 1){
				lcd_puts(" IR");
			} else {
				lcd_puts(" Bluetooth");
			}
		}
		if (workMode == 3) {
			workMode = 0;
		}
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



