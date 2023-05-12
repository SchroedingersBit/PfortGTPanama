# Eröffnungsrennen
Hier werden unsere Programme und Ideen für das Eröffnungsrennen erklärt und dargestellt. Besonders sollte hier auf das Ultraschall-Sensoren Programm eingegangen werden.
Für das Eröffnungsrennen wird kein Kamera Script benötigt. Dies liegt daran, dass allein die Spielfeldgröße im Mittelbereich angepasst wird. Daher werden nur die Ultraschall-Sensoren-Daten verarbeitet.

## RC_Control.ino
Gestartet wird das Programm über das RC_Control.ino, welches alle weiteren Klassen öffnet und managed.
```c++
#include "variables.h"  // all the vars, actuators, and sensors are defined and initialized here.
#include "ControlRC.h"  // all functions for control, read sensor and update variables are located here



void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  initializeHardware();
}


unsigned long stopTime = 0;  // keep track of when to stop
// looping over all important classes
void loop() {
  //drive();
  updateControlData();
  drivingDC.drive(45);
  //control_DC();
  control_servo();
  
  //print();




  //Abbruchbedingung nachdem 3 Runden gefahren wurden
  if (stop && stopTime == 0) {
    stopTime = millis() + 2000;  // keep driving for 3 more seconds
  }

  if (stopTime > 0 && millis() >= stopTime) {
    while (1) {
      steeringServo.drive(0);
      drivingDC.drive(0);
      delay(100);
    }
  }
}


// important informations for bugfixing
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
In dieser Header Datei sind alle Funktionen zur Steuerung desFahrzeugs basierend auf den Sensordaten enthalten.
Es werden Überprüfungen für die Sicherheitswinkel, Kurven und zur Aktualisierung der Referenzwinkeln initialisiert wodurch die Fahrzeugsteuerung garantiert wird.
```c++
#ifndef Control_h
#define Control_h
#import <math.h>
#include "variables.h"


void checkSafeAngle() {
  if (referenceAngle - 10 <= roll && roll <= referenceAngle + 10) {
    safeAngle = true;
  } else {
    safeAngle = false;
  }
}
void checkCurve() {
  if (abs(rightShift) >= MAX_DISTANCE - 340) {
    if (distances[0] < 90)
      if (safeAngle)
        if (rightShift > 0)
          referenceAngle -= 90;
        else referenceAngle += 90;
  }
}
void updateReference() {
  if (safeAngle)
    if ((abs(rightShift) - abs(lastShift)) >= 0)
      referenceAngle = roll;
}
void stopCheck() {
  int numberOfRounds = 3;
  if (abs(referenceAngle) >= numberOfRounds * 360-20)
    stop = true;
  else
    stop = false;
}

void updateSensorData() {
  manager.readDistances(distances);
  roll = orientation.getTotalRoll();
}

void updateChecks() {
  checkSafeAngle();
  checkCurve();
  stopCheck();
}

void updateControlData() {
  updateSensorData();
  updateChecks();
  int frontDistance = distances[0];
  int rightDistance = distances[1];
  int leftDistance = distances[2];

  //compute Target Velocity

  const float FACTOR = 1.0f;     // 2
  const float FACTOR2 = 0.005f;  //0.01
  float target_velocity = frontDistance * FACTOR * tanh(frontDistance * FACTOR2);

  //compute Steering Angle

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

void control_servo() {
  float target_car_angle = controlDataArr[0];
  float err = target_car_angle - roll;
  steeringServo.drive(err * 0.1);
}

void control_DC() {
  float target_velocity = controlDataArr[1];
  drivingDC.drive(target_velocity);
}

void drive() {
  updateControlData();
  control_servo();
  control_DC();
}

#endif
```
