#ifndef variables_H
#define variables_H
#include <Arduino.h>
#include "CarOrientation.h"
//1) Include necessary libraries for your sensors and actuators here
#include "UltrasonicManager.h"
#include "MyServo.h"
#include "MyDC.h"
#include "SerialIO.h"
#include "camera.h"


//2) Declare your variables, sensors, and actuators

//2.a) UltrasonicManager
const uint8_t leftTrigPin = 2;
const uint8_t leftEchoPin = 3;
const uint8_t frontTrigPin = 4;
const uint8_t frontEchoPin = 5;
const uint8_t rightTrigPin = 6;
const uint8_t rightEchoPin = 7;
UltrasonicManager manager(frontTrigPin, frontEchoPin, rightTrigPin, rightEchoPin, leftTrigPin, leftEchoPin);


//2.b) ServoMotor
byte servoPin = 9;
byte servo_PWMpin = 13;  // goes in the driver to control Voltage
MyServo steeringServo(servoPin, servo_PWMpin);

//2.c) DC motor
byte DC_speedPin = 11;  //PWM PIN
byte pinIN1 = 22;       //rechts
byte pinIN2 = 24;       //links
MyDC drivingDC(DC_speedPin, pinIN1, pinIN2);


//2.d) Serial IO
//SerialIO IO;

//2.e) CarOrientation
CarOrientation orientation;



//2.g) Vars
float controlDataArr[] = { 0, 0 };  //[desired Car Angle, Speed]
float distances[] = { 0, 0, 0 };
float roll;
float referenceAngle = 0;
float theoreticalAngle = 0;
int wallDistance = 20;
int wallcheck;
float wallShift;
float rightShift;
float camShift;
float abc;
int turns = 0;
bool safeAngle;
bool Sumcheck = true;
float lastShift = 10000000;
int velocity = 40;
int direction = 0;
int sign;
int color;
float x_pos;
int8_t obstacle;
float frameWidth;
int Blockcheck;
int area;
int redturn;
bool redcheck = true;
int Run = 1;  //0 for open Challenge and 1 for obstacle Challenge

//2.f) Camera
camera cam;


void initializeHardware() {
  steeringServo.init();
  drivingDC.init();
  cam.init();
  if (!orientation.init()) {
    Serial.println("Failed to initialize CarOrientation");
    while (1) {
      delay(10);
    }
  }
}


//ToDelete

float hh=0;
#endif
