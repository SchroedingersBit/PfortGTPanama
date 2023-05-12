#ifndef Control_h
#define Control_h
#import <math.h>
#include "variables.h"


void checkSafeAngle() {
  if (referenceAngle - 12 <= roll && roll <= referenceAngle + 12) {
    safeAngle = true;
  } else {
    safeAngle = false;
  }
}
void checkCurve() {
  if (abs(distances[2] + distances[1]) >= 100) {
    if (distances[0] < 90)
      if (safeAngle) {
        cam.set_color();
        if (distances[2] - distances[1] > 0)
          referenceAngle -= 90;
        else referenceAngle += 90;
      }
  }
}
void updateReference() {
  if (safeAngle)
    if ((abs(rightShift) - abs(lastShift)) * abs(min(0, color - 1)) > 0)
      referenceAngle = roll;
}
void stopCheck() {
  int numberOfRounds = 3;
  if (abs(referenceAngle) >= numberOfRounds * 360 - 20)
    stop = true;
  else
    stop = false;
}

void updateSensorData() {
  manager.readDistances(distances);
  roll = orientation.getTotalRoll();
}

void updateCameraData() {
  cam.firstBlockData();
  color = cam.get_color();
}

void updateChecks() {
  checkSafeAngle();
  checkCurve();
  stopCheck();
}

void updateControlData() {
  updateSensorData();
  updateCameraData();
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
  const float FACTOR_Camera = 10.0f;
  // Calcualte relative lateral position on the road
  // (how far towards right border is the cart located)
  rightShift = (leftDistance - FACTOR_Camera) * abs(max(-1, color - 2)) - (rightDistance - FACTOR_Camera) * abs(color - 1);
  // The closer the cart is to the right border, the more it should steer to the left (and vice versa).
  // (Steering to the right = positive steering angle)
  // To achieve this in a smooth way, a parameter-tuned tanh function can be used.
  float target_car_angle = -1 * (FACTOR_Steering / (abs(min(0, color - 1)) * min(frontDistance, 40) + min(1, color) * 5)) * rightShift * tanh(abs(rightShift)) + referenceAngle;
  updateReference();
/*
  if (cam.get_blocks() || roll < referenceAngle - 30 || roll > referenceAngle + 30) {
    target_car_angle = (target_car_angle >= referenceAngle + 40 ? referenceAngle + 40 : target_car_angle);
    target_car_angle = (target_car_angle <= referenceAngle - 40 ? referenceAngle - 40 : target_car_angle);
  } else {
    target_car_angle = (target_car_angle <= referenceAngle - 5 ? referenceAngle - 5 : target_car_angle);
    target_car_angle = (target_car_angle >= referenceAngle + 5 ? referenceAngle + 5 : target_car_angle);
  }
*/

  if (cam.get_blocks() ) {
    target_car_angle = (target_car_angle >= referenceAngle + 55 ? referenceAngle + 55 : target_car_angle);
    target_car_angle = (target_car_angle <= referenceAngle - 55 ? referenceAngle - 55 : target_car_angle);
  } else
  if( roll < referenceAngle - 30 || roll > referenceAngle + 30) {
    target_car_angle = (target_car_angle <= referenceAngle - 30 ? referenceAngle - 30 : target_car_angle);
    target_car_angle = (target_car_angle >= referenceAngle + 30 ? referenceAngle + 30 : target_car_angle);
  }
  else{
    target_car_angle = (target_car_angle <= referenceAngle - 5 ? referenceAngle - 5 : target_car_angle);
    target_car_angle = (target_car_angle >= referenceAngle + 5 ? referenceAngle + 5 : target_car_angle);
  }
  controlDataArr[0] = target_car_angle;
  controlDataArr[1] = target_velocity;
  lastShift = rightShift;
}

void control_servo() {
  float target_car_angle = controlDataArr[0];

  float err = target_car_angle - roll;
  steeringServo.drive(err * 0.2);
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