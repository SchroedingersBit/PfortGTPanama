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
const uint8_t frontTrigPin = 2;
const uint8_t frontEchoPin = 3;
const uint8_t rightTrigPin = 4;
const uint8_t rightEchoPin = 5;
const uint8_t leftTrigPin = 6;
const uint8_t leftEchoPin = 7;
UltrasonicManager manager(frontTrigPin, frontEchoPin, rightTrigPin, rightEchoPin, leftTrigPin, leftEchoPin);


//2.b) ServoMotor
byte servoPin = 9;
MyServo steeringServo(servoPin);

//2.c) DC motor
byte DC_PWMpin = 11;
MyDC drivingDC(DC_PWMpin);


//2.d) Serial IO
//SerialIO IO;

//2.e) CarOrientation
CarOrientation orientation;

//2.f) Camera
camera cam;

//2.f) Vars
float controlDataArr[] = { 0, 0 };  //[desired Car Angle, Speed]
float distances[] = { 0, 0, 0 };
float roll;
float referenceAngle = 0;
float rightShift;
bool stop;
bool safeAngle;
float lastShift = 10000000;
uint8_t color = 0;


void initializeHardware() {
  //IO.init();
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


#endif