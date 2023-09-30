//Inkludieren der erforderlichen Bibliotheken für die Sensoren 
#ifndef variables_H
#define variables_H
#include <Arduino.h>
#include "CarOrientation.h"
#include "UltrasonicManager.h"
#include "MyServo.h"
#include "MyDC.h"
#include "camera.h"


// Deklarieren der Variablen, Sensoren

// UltrasonicManager für die Arduino Ports
const uint8_t frontTrigPin = 2;
const uint8_t frontEchoPin = 3;
const uint8_t rightTrigPin = 4;
const uint8_t rightEchoPin = 5;
const uint8_t leftTrigPin = 6;
const uint8_t leftEchoPin = 7;
UltrasonicManager manager(frontTrigPin, frontEchoPin, rightTrigPin, rightEchoPin, leftTrigPin, leftEchoPin);


//ServoMotor Port Deklarierung
byte servoPin = 9;
byte servo_PWMpin = 38;

MyServo steeringServo(servoPin, servo_PWMpin);

//DC Motor Port Deklarierung
byte DC_PWMpin = 11;
MyDC drivingDC(DC_PWMpin);

//CarOrientation Deklarierung
CarOrientation orientation;

//Camera Deklarierung
camera cam;

//weitere nützliche Variablen
float controlDataArr[] = { 0, 0 };  //[desired Car Angle, Speed]
float distances[] = { 0, 0, 0 };
float roll;
float referenceAngle = 0;
float rightShift;
float lastShift = 10000000;
bool stop;
bool safeAngle;
uint8_t color = 0;
int secs = 3.5;
int velocity = 90;//Grundgeschwindigkeit
int Run = 1;// Unterscheidung Hindernisrennen, Eröffnungsrennen

// Pins reservieren und initialisieren
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


#endif