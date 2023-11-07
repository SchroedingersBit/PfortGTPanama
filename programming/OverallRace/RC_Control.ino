#include "variables.h"  // all the vars, actuators, and sensors are defined and initialized here.
#include "ControlRC.h"  // all functions for control, read sonsor and update variables are located here


void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);
  initializeHardware();
  drivingDC.setForward();
}


int numRounds = 3; 
int Stoptime = 3; // how many seconds to drice at least after seing the last curve
unsigned long tim = 0;
void loop() {
  updateControlData(); // updating the calculation
  drivingDC.drive(velocity); // driving the Dc_Motor
  control_servo(); // Driving the Servo Motor
  //print(); // For Debugging we can activate print, but for the runss it should be commented out ore it will take a lot of calculation time

  if (turns == 4 * numRounds) { // checking if we have turned enough times
    tim = millis() + Stoptime * 1000; // defining tim 
    while (distances[0] > 120 || !Sumcheck || tim > millis()) { // drive for at least tim and until frontDistance is less than 120 and SumDistance is less than 100 (which means you are not in a curve anymore)
      updateControlData();
      drivingDC.drive(velocity);
      control_servo();
    }
    while (1) { // when the previous while loop is done, stop the robot.
      control_servo();
      drivingDC.drive(0);
      delay(100);
    }
  }
}


// the things we print for debugging
void print() {
  float k = (distances[0] - (67578.1 / (cam.get_Blocksize() + 514.89) - 13.4431) - 80) * (0.5 * direction * (abs(cam.get_color() - 1) - min(1, 2 - cam.get_color())) + max(0.5, 1 - 0.5 * cam.get_color()));
  Serial.print("referenceAngle ");
  Serial.print(referenceAngle);
  Serial.print("       targetAngle ");
  Serial.print(controlDataArr[0]);
  Serial.print("       roll ");
  Serial.print(roll);
  Serial.print("       rightShift ");
  Serial.print(rightShift);
  Serial.print("       camShift ");
  Serial.print(camShift);
  Serial.print("       wallShift ");
  Serial.print(wallShift);
  Serial.print("       frontdistance ");
  Serial.print(distances[0]);
  Serial.print("       rightdistance ");
  Serial.print(distances[1]);
  Serial.print("       leftdistance ");
  Serial.print(distances[2]);
  Serial.print("       sumdistance ");
  Serial.print(distances[2] + distances[1]);
  Serial.print("       sumcheck ");
  Serial.print(Sumcheck);
  Serial.print("       size ");
  Serial.print(cam.get_Blocksize());
  Serial.print("       Blockdistance ");
  Serial.print(k);
  Serial.print("       function ");
  Serial.print(k < 0 && k > -35 ? 0 : 1);
  Serial.print("        x_pos  ");
  Serial.print(cam.get_x_pos());
  Serial.print("        frameWidth  ");
  Serial.print(cam.get_frameWidth());
  Serial.print("        correction ");
  Serial.print(1 - 1 / cam.get_frameWidth() * abs(cam.get_x_pos()));
  Serial.print("       color ");
  Serial.println(cam.get_color());
}
