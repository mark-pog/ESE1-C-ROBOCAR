
int trigPin = 7;    // TRIG pin
int echoPin = 6;    // ECHO pin
unsigned long previousSensorMillis = 0;
unsigned long previousSensorInterval = 0;
int interval = 5000;
float duration_us, distance_cm;



void setup() {
  // pinMode(4, OUTPUT);
  // pinMode(5, OUTPUT);
  // pinMode(6, OUTPUT);
  // pinMode(7, OUTPUT);
  // pinMode(9, OUTPUT);
  // pinMode(11, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}


int driveForward(int speed){
    /*digitalWrite(6, HIGH);
    digitalWrite(7, LOW); 
    analogWrite(11, speed);
    digitalWrite (4, HIGH); 
    digitalWrite(5, LOW); 
    analogWrite(9, speed);*/
    Serial.println("Driving Forward");
}

int driveRight(int speed){
   /* digitalWrite(6, HIGH); 
    digitalWrite(7, LOW); 
    analogWrite(11, speed);
    digitalWrite(4, HIGH); 
    digitalWrite(5, LOW); 
    analogWrite(9, speed - 100);*/
    Serial.println("Driving Right");
}

void sensorsValues(){
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
}


void loop() {
  sensorsValues();
  if(distance_cm < 50){
    driveRight(200);
  } else {
    driveForward(200);
  }
  delay(500);
}

