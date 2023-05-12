#include "variables.h"  // all the vars, actuators, and sensors are defined and initialized here.
#include "ControlRC.h"  // all functions for control, read sonsor and update variables are located here



void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  initializeHardware();
}


unsigned long stopTime = 0;  // keep track of when to stop
int secs=2.7;
void loop() {
  //drive();
  updateControlData();
  drivingDC.drive(55);
  //control_DC();
  control_servo();

  //print();




  //Abbruchbedingung
  if (stop && stopTime == 0) {
    stopTime = millis() + secs*1000;  // keep driving for 3 more seconds
  }

  if (stopTime > 0 && millis() >= stopTime) {
    while (1) {
      control_servo();
      drivingDC.drive(0);
      delay(100);
    }
  }
}



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
