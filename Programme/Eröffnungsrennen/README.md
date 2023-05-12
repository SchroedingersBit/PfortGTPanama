# Eröffnungsrennen
Hier werden unsere Programme und Ideen für das Eröffnungsrennen erklärt und dargestellt. Besonders sollte hier auf das Ultraschall-Sensoren Programm eingegangen werden.
Für das Eröffnungsrennen wird kein Kamera Script benötigt. Dies liegt daran, dass allein die Spielfeldgröße im Mittelbereich angepasst wird. Daher werden nur die Ultraschall-Sensoren-Daten verarbeitet.

## RC_Control.ino
Gestartet wird das Programm über das RC_Control.ino, welches alle weiteren Klassen öffnet und managed.
```c++
//Inkludieren der erforderlichen Bibliotheken und Verwendung der Header Datein
#include "variables.h"  // all the vars, actuators, and sensors are defined and initialized here.
#include "ControlRC.h"  // all functions for control, read sonsor and update variables are located here


//serielle Verbindung erstellen und die Ports des Arduino aus dem Variable Header initialisieren
void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  initializeHardware();
}


unsigned long stopTime = 0;  // um zu wisen, wann gestoppt werden muss
//  Anweisungen für das Auto
void loop() {
  //drive();
  updateControlData();
  drivingDC.drive(45);
  //control_DC();
  control_servo();
  
  //print();





  //Abbruchbedingung nachdem 3 Runden gefahren wurden
  if (stop && stopTime == 0) {
    stopTime = millis() + 2000;  // weiterfahren für 3 weitere Sekunden
  }

  if (stopTime > 0 && millis() >= stopTime) {
    while (1) {
      steeringServo.drive(0);
      drivingDC.drive(0);
      delay(100);
    }
  }
}


// verschiedenen Sensordaten im Seriellen Monitor für Bugfixing und Verständnis von dem, was der Roboter tut
void print() {
  //manager.printDistances();
  Serial.print("referenceAngle ");
  Serial.print(referenceAngle);
  Serial.print("       rightShift ");
  Serial.print(rightShift);
  Serial.print("       roll ");
  Serial.print(roll);
  Serial.print("       targetAngle ");
  Serial.println(controlDataArr[0]);
}

```

Zuerst werden in `#include` alle wichtigen Header Dateien abgefragt um diese später zu verwenden. Danach wird in `void setup()` die serielle Verbindung erstellt und die Ports des Arduino aus dem Variable Header initialisiert. Nun loopen wir in `void loop` über die Anweisungen für das Auto. Zuerst wird die UpdateControlData in der ControlRC Header Datei ausgeführt. Danach lassen wir das Auto über den drivingDC Port mit einer Geschwindigkeit fahren und lenken am Ende gegebenenfalls. Sollte das Auto die drei Runden gefahren sein,
wird über die Abbruchbedingung das Programm beendet.
Es wird am Ende `void print()` verwendet um die verschiedenen Sensordaten im Seriellen Monitor auszugeben und somit das Bugfixing und Verständnis von dem, was der Roboter tut, zu verbessern.

## ControlRC.h
In dieser Header Datei sind alle Funktionen zur Steuerung des Fahrzeugs basierend auf den Sensordaten enthalten.
Es werden Überprüfungen für die Sicherheitswinkel, Kurven und zur Aktualisierung der Referenzwinkeln initialisiert wodurch die Fahrzeugsteuerung garantiert wird.

```c++
//Inkludieren der erforderlichen Bibliotheken.
#ifndef Control_h
#define Control_h
#import <math.h>
#include "variables.h"

// Überprüfung, ob der Referenzwinkel in einem sicheren Berreich liegt, sodass dabei die Ultraschall-Sensoren sichere Werte verwenden
void checkSafeAngle() {
  if (referenceAngle - 10 <= roll && roll <= referenceAngle + 10) {
    safeAngle = true;
  } else {
    safeAngle = false;
  }
}

//  Untersuchung durch die Ultraschall-Sensoren, ob eine Kurve detektiert wird
void checkCurve() {
  if (abs(rightShift) >= MAX_DISTANCE - 340) {
    if (distances[0] < 90)
      if (safeAngle)
        if (rightShift > 0)
          referenceAngle -= 90;
        else referenceAngle += 90;
  }
}
// Anpassung des Referenz Winkels, um ein gerades Fahren zu garantieren
void updateReference() {
  if (safeAngle)
    if ((abs(rightShift) - abs(lastShift)) >= 0)
      referenceAngle = roll;
}
// Untersuchung durch die Gyrosensoren, ob der Auto sich bereits 3*360° gedreht hat, dann Auto stoppen
void stopCheck() {
  int numberOfRounds = 3;
  if (abs(referenceAngle) >= numberOfRounds * 360-20)
    stop = true;
  else
    stop = false;
}
// neue Sensordaten
void updateSensorData() {
  manager.readDistances(distances);
  roll = orientation.getTotalRoll();
}
// Führt vorherige Funktionen aus
void updateChecks() {
  checkSafeAngle();
  checkCurve();
  stopCheck();
}
// wird durch RC_Control.ino gestartet und startet andere Funktionen, berechnet die verschiedenen WInkel und Anpassungen
void updateControlData() {
  updateSensorData();
  updateChecks();
  int frontDistance = distances[0];
  int rightDistance = distances[1];
  int leftDistance = distances[2];

  //berechnet Richtgeschwindigkeit

  const float FACTOR = 1.0f;     // 2
  const float FACTOR2 = 0.005f;  //0.01
  float target_velocity = frontDistance * FACTOR * tanh(frontDistance * FACTOR2);

  //berechnet Steuerwinkel 

  const float FACTOR_Steering = 5.0f;  //5
  // Calcualte relative lateral position on the road
  // (how far towards right border is the cart located)
  rightShift = leftDistance - rightDistance;
  // The closer the cart is to the right border, the more it should steer to the left (and vice versa).
  // (Steering to the right = positive steering angle)
  // To achieve this in a smooth way, a parameter-tuned tanh function can be used.
  float target_car_angle = -1 * (FACTOR_Steering / min(frontDistance, 40)) * rightShift * tanh(abs(rightShift)) + referenceAngle;
  updateReference();

  controlDataArr[0] = target_car_angle;
  controlDataArr[1] = target_velocity;
  lastShift = rightShift;
}
// Weiterleiten der berechneten Daten zum Servo, passt damit die Lenkung an
void control_servo() {
  float target_car_angle = controlDataArr[0];
  float err = target_car_angle - roll;
  steeringServo.drive(err * 0.1);
}
// passt den DC`s und damit die Geschwindigkeit an
void control_DC() {
  float target_velocity = controlDataArr[1];
  drivingDC.drive(target_velocity);
}
// führt vorherige Funktionen aus
void drive() {
  updateControlData();
  control_servo();
  control_DC();
}

#endif
```
Zuerst werden in der include section, alle weiteren Bibliotheken und Header Datein importiert, die weiterhin benötigt werden. In `void checkSafeAngle()` wird überprüft, ob die Abweichungen der Ultraschall-Sensoren in einem sicheren Bereich liegen, sodass es zu keiner Änderung im Lenkverhalten kommt. Durch `void checkCurve()` wird der Referenzwinkel anhand der vorliegenden Sensordaten aktualisiert.
## variables.h

```c++
//Inkludieren der erforderlichen Bibliotheken für die Sensoren 
#ifndef variables_H
#define variables_H
#include <Arduino.h>
#include "CarOrientation.h"

#include "UltrasonicManager.h"
#include "MyServo.h"
#include "MyDC.h"
#include "SerialIO.h"


// Deklarieren der Variablen, Sensoren

// UltrasonicManager für die Arduino Ports?
const uint8_t frontTrigPin = 2;
const uint8_t frontEchoPin = 3;
const uint8_t rightTrigPin = 4;
const uint8_t rightEchoPin = 5;
const uint8_t leftTrigPin = 6;
const uint8_t leftEchoPin = 7;
UltrasonicManager manager(frontTrigPin, frontEchoPin, rightTrigPin, rightEchoPin, leftTrigPin, leftEchoPin);


//ServoMotor Ports
byte servoPin = 9;
MyServo steeringServo(servoPin);

//DC motor Ports
byte DC_PWMpin = 11;
MyDC drivingDC(DC_PWMpin);


//Serial IO
//SerialIO IO;

//CarOrientation
CarOrientation orientation;

//weitere Vars
float controlDataArr[] = { 0, 0 };  //[desired Car Angle, Speed]
float distances[] = { 0, 0, 0 };
float roll;
float referenceAngle = 0;
float rightShift;
bool stop;
bool safeAngle;
float lastShift=10000000;

// Pins reservieren und initialisieren
void initializeHardware() {
  //IO.init();
  steeringServo.init();
  drivingDC.init();
  if (!orientation.init()) {
    Serial.println("Failed to initialize CarOrientation");
    while (1) {
      delay(10);
    }
  }
}


#endif
```
