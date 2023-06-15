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

  updateControlData();
  drivingDC.drive(velocity); // Grundgeschwindigkeit des DC Motors
  control_servo();
  
  //print(); // Debugging

  //Abbruchbedingung nachdem 3 Runden gefahren wurden
  if (stop && stopTime == 0) {
    stopTime = millis() + secs * 1000;  // weiterfahren für 3 weitere Sekunden
  }

  if (stopTime > 0 && millis() >= stopTime) {
    while (1) {
      control_servo();//steeringservo.drive?
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
  Serial.print("       right ");
  Serial.print(distances[1]);
  Serial.print("       left ");
  Serial.print(distances[2]);
  Serial.print("       front ");
  Serial.print(distances[0]);
  Serial.print("       roll ");
  Serial.print(roll);
  Serial.print("       targetAngle ");
Serial.print(controlDataArr[0] - referenceAngle);
   Serial.print("       width ");
  Serial.print(cam.get_width());
    Serial.print("       color ");
 Serial.println(cam.get_color());
}
