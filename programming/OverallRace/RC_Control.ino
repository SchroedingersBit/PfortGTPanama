#include "variables.h"  // all the vars, actuators, and sensors are defined and initialized here.
#include "ControlRC.h"  // all functions for control, read sonsor and update variables are located here


void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);
  initializeHardware();
  drivingDC.setForward();
}


int numRounds = 3;
int Stoptime = 6;
unsigned long tim = 0;
void loop() {
  updateControlData();
  drivingDC.drive(velocity);
  control_servo();
  //print();  //zzzzzzz

  if (redturn == 2 && turns == 8 && Sumcheck && redcheck) { // color = 2, end of round 2 and redcheck = true
    tim = millis() + 1000;  // drive for a second in such a way you will become straight
    while (tim > millis()) {
      drivingDC.drive(velocity);
      steeringServo.drive(-1 * roll);
    }

    //moving direction is backwards
    drivingDC.drive(0);
    drivingDC.setBackward();

    // drive until you are out of the straight section
    while (distances[1] + distances[2] < 150 || distances[0] < 150) {
      drivingDC.drive(velocity);
      steeringServo.drive(0);
    }

    //drive for one more second backwards
    tim = millis() + 1000;
    while (tim > millis()) {
      drivingDC.drive(velocity);
      steeringServo.drive(0);
    }

    // change the moving direction
    drivingDC.drive(0);
    drivingDC.setForward();
    referenceAngle = referenceAngle + 90 * direction; // For the car now appears so as if a curve is on his left which is why he will turn to the left.
    theoreticalAngle = theoreticalAngle + 90 * direction;

    // let him drive till he is in the straight section
    while (distances[1] + distances[2] > 150 || distances[0] > 150) {
      updateControlData();
      drivingDC.drive(velocity);
      control_servo();
    }

    direction = direction * (-1);     // changing the sign of direction so he turns right
    redcheck = false; // to ensure he will never get in here again.
  }

  if (turns == 4 * numRounds) {
    tim = millis() + Stoptime * 1000;
    while (distances[0] > 120 || !Sumcheck || tim > millis()) {  // drive for at least tim and until frontDistance is less than 120 and SumDistance is less than 100
      updateControlData();
      drivingDC.drive(velocity);
      control_servo();
    }
    while (1) {
      control_servo();
      drivingDC.drive(0);
      delay(100);
    }
  }
}



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
  Serial.print("       cam steerin ");
  Serial.print(hh);
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
