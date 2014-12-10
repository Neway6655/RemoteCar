// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

// DC motor on M1
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);

int inByte;
// initial speed
int speed = 200;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps  
   
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
}

void accelerate() {  
//  if (speed < 0){
//    speed = 0;
//  } 
//  if (speed < 255){
//    speed = speed + 10;
//  }
//  Serial.println("Speed: " + speed);
//  motor1.setSpeed(speed);
//  motor2.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void decelerate() {
//  speed = speed - 50;
//  motor1.run(FORWARD);
//  motor2.run(FORWARD);
//  if (speed > 0) {    
//    motor1.run(FORWARD);
//    motor2.run(FORWARD);
//    Serial.println("Speed: " + speed);
//    motor1.setSpeed(speed);
//    motor2.setSpeed(speed);
//  } else if (speed == 0) {
//    // stopped
//    speed = 0;
//    motor1.run(RELEASE);
//    motor2.run(RELEASE);
//  } else {
//    motor1.run(BACKWARD);
//    motor2.run(BACKWARD);
//    int backwardSpeed = 0 - speed;    
//    if (backwardSpeed <= 255){
//      Serial.println("Speed: " + speed);
//      motor1.setSpeed(backwardSpeed);
//      motor2.setSpeed(backwardSpeed);
//    }else {
//      speed = -255;
//      Serial.println("Speed: " + speed);
//      motor1.setSpeed(255);
//      motor2.setSpeed(255);
//    }
//  }
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void turnLeft() {
  motor1.run(FORWARD);
  motor2.run(RELEASE);
}

void turnRight(){
  motor1.run(RELEASE);
  motor2.run(FORWARD);
}

// Test the DC motor, stepper and servo ALL AT ONCE!
void loop() {
  if (Serial.available() > 0){
    inByte = Serial.read();    // read serial byte
    Serial.println(inByte);     // print serial byte
    
    switch(inByte){
    case 49:  // w, forward
      Serial.println("Moving forward.");
      accelerate();
      break;
    case 50:  // s, backward
      Serial.println("Moving backward.");
      decelerate();
      break;
    case 51:   // a, left
      Serial.println("Turning left.");
      turnLeft();
      break;
    case 52:  // d, right
      Serial.println("Turning right.");
      turnRight();
      break;    
    }
  }
}
