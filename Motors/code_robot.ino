// Motor control pins for the H-Bridge
const int motor1A = 5;  // H-Bridge input 1 (forward)
const int motor1B = 6;  // H-Bridge input 2 (backward)
const int motor2A = 7;  // H-Bridge input 3 (forward)
const int motor2B = 8;  // H-Bridge input 4 (backward)

// Speed constants
const int FULL_SPEED = 255;  // Max PWM value for full speed
const int HALF_SPEED = 127;  // Half speed
const int MEDIUM_SPEED = 191; // Medium speed

void setup() {
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);

  stopMotors();  // Initialize with motors stopped
}

void loop() {
  moveForward(FULL_SPEED);  // Full speed forward
  delay(2000);              // Move forward 2 seconds

  moveBackward(HALF_SPEED); // Move backward at half speed
  delay(2000);              // Move backward 2 seconds

  turnLeft(MEDIUM_SPEED);   // Turn left at medium speed
  delay(1000);              // Turn left 1 second

  turnRight(MEDIUM_SPEED);  // Turn right at medium speed
  delay(1000);              // Turn right 1 second
  
  stopMotors();             // Stop the motors
  delay(2000);              // Pause for 2 seconds
}

void moveForward(int speed) {
  analogWrite(motor1A, speed);  // Motor 1 forward
  analogWrite(motor1B, 0);      // Motor 1 stop backward
  analogWrite(motor2A, speed);  // Motor 2 forward
  analogWrite(motor2B, 0);      // Motor 2 stop backward
}

void moveBackward(int speed) {
  analogWrite(motor1A, 0);      // Motor 1 stop forward
  analogWrite(motor1B, speed);  // Motor 1 backward
  analogWrite(motor2A, 0);      // Motor 2 stop forward
  analogWrite(motor2B, speed);  // Motor 2 backward
}

void turnLeft(int speed) {
  analogWrite(motor1A, 0);      // Stop Motor 1
  analogWrite(motor1B, speed);  // Motor 1 backward
  analogWrite(motor2A, speed);  // Motor 2 forward
  analogWrite(motor2B, 0);      // Stop Motor 2 backward
}

void turnRight(int speed) {
  analogWrite(motor1A, speed);  // Motor 1 forward
  analogWrite(motor1B, 0);      // Stop Motor 1 backward
  analogWrite(motor2A, 0);      // Stop Motor 2 forward
  analogWrite(motor2B, speed);  // Motor 2 backward
}

void stopMotors() {
  analogWrite(motor1A, 0);      // Stop Motor 1
  analogWrite(motor1B, 0);      // Stop Motor 1 backward
  analogWrite(motor2A, 0);      // Stop Motor 2
  analogWrite(motor2B, 0);      // Stop Motor 2 backward
}
