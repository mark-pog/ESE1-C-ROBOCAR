#define SafeDistance 15



int frontTrigPin = 3;  // TRIG pin
int frontEchoPin = 2;  // ECHO pin
int rightTrigPin = 5;  // TRIG pin
int rightEchoPin = 4;  // ECHO pin
int leftTrigPin = 7;   // TRIG pin
int leftEchoPin = 6;   // ECHO pin
unsigned long previousMillis = 0;
int interval = 5000;
float distanceFront, distanceRight, distanceLeft, duration_us;

void printDistances() {
  Serial.print("distances: ");
  Serial.print(distanceFront);
  Serial.print("---");
  Serial.print(distanceRight);
  Serial.print("---");
  Serial.print(distanceLeft);
  Serial.println(" cm");
}

void setup() {
  // pinMode(4, OUTPUT);
  // pinMode(5, OUTPUT);
  // pinMode(6, OUTPUT);
  // pinMode(7, OUTPUT);
  // pinMode(9, OUTPUT);
  // pinMode(11, OUTPUT);
  Serial.begin(9600);
  pinMode(frontTrigPin, OUTPUT);
  pinMode(frontEchoPin, INPUT);
  pinMode(rightTrigPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);
  pinMode(leftTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);
}


void driveForward() {
  /*digitalWrite(6, HIGH);
    digitalWrite(7, LOW); 
    analogWrite(11, speed);
    digitalWrite (4, HIGH); 
    digitalWrite(5, LOW); 
    analogWrite(9, speed);*/
  Serial.println("Driving Forward");
}

void driveRight() {
  /* digitalWrite(6, HIGH); 
    digitalWrite(7, LOW); 
    analogWrite(11, speed);
    digitalWrite(4, HIGH); 
    digitalWrite(5, LOW); 
    analogWrite(9, speed - 100);*/
  Serial.println("Driving Right");
}

void driveLeft() {
  /* digitalWrite(6, HIGH); 
    digitalWrite(7, LOW); 
    analogWrite(11, speed);
    digitalWrite(4, HIGH); 
    digitalWrite(5, LOW); 
    analogWrite(9, speed - 100);*/
  Serial.println("Driving Left");
}

void driveBack() {
  /* digitalWrite(6, HIGH); 
    digitalWrite(7, LOW); 
    analogWrite(11, speed);
    digitalWrite(4, HIGH); 
    digitalWrite(5, LOW); 
    analogWrite(9, speed - 100);*/
  Serial.println("Driving Back");
}

void sensorRightValue() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(rightTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(rightTrigPin, LOW);

  // measure duration of pulse from ECHO pin
  float duration_us = pulseIn(rightEchoPin, HIGH);

  // calculate the distance
  distanceRight = 0.017 * duration_us;

  // print the value to Serial Monitor
}
void sensorFrontValue() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(frontTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(frontTrigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(frontEchoPin, HIGH);

  // calculate the distance
  distanceFront = 0.017 * duration_us;
}
void sensorLeftValue() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(leftTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(leftTrigPin, LOW);

  // measure duration of pulse from ECHO pin
  float duration_us = pulseIn(leftEchoPin, HIGH);

  // calculate the distance
  distanceLeft = 0.017 * duration_us;
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 500) {
    printDistances();
    sensorFrontValue();
    sensorRightValue();
    sensorLeftValue();
    if (distanceFront < SafeDistance) {
      if (distanceLeft < SafeDistance && distanceRight < SafeDistance) {
        driveBack();
      } else {
        if (distanceLeft > distanceRight) {
          driveLeft();
        } else if (distanceLeft < distanceRight) {
          driveRight();
        }
      }
    } else {
      driveForward();
    }
    previousMillis = currentMillis;
  }
}