# Eröffnungsrennen
Hier werden unsere Programme und Ideen für das Eröffnungsrennen erklärt und dargestellt. Besonders sollte hier auf das Ultraschall-Sensoren Programm eingegangen werden.
Für das Eröffnungsrennen wird kein Kamera Script benötigt. Dies liegt daran, dass allein die Spielfeldgröße im Mittelbereich angepasst wird. Daher werden nur die Ultraschall-Sensoren-Daten verarbeitet.

Gestartet wird das Programm über das RC_Control.ino, welches alle weiteren Klassen öffnet und managed.
## RC_Control.ino
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




  //Abbruchbedingung
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

Zuerst werden in `#include` alle wichtigen Header Dateien abgefragt um diese später zu verwenden. Danach wird in `void setup()` die serielle Verbindung erstellt und die Ports des Arduino aus dem Variable Header initialisiert.

Dabei wird `void print()` verwendet um die verschiedenen Sensordaten im Seriellen Monitor auszugeben und somit das Bugfixing und Verständnis von dem, was der Roboter tut, zu verbessern.
