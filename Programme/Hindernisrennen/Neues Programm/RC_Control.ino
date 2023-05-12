#include "variables.h"  // all the vars, actuators, and sensors are defined and initialized here.
#include "ControlRC.h"  // all functions for control, read sonsor and update variables are located here

//DAS IST DAS WAHRE PROGRAMM

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  initializeHardware();
}


unsigned long stopTime = 0;  // keep track of when to stop

void loop() {
  //drive();
  //this is the camera code
  updateControlData();
  drivingDC.drive(90);
  //control_DC();
  control_servo();
  
  print();




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
  Serial.print(controlDataArr[0]);
   Serial.print("       width ");
  Serial.print(cam.get_width());
    Serial.print("       color ");
 Serial.println(cam.get_color());
}
