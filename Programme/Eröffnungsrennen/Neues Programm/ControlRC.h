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
  if (distances[2] + distances[1] >= 100) {
    if (distances[0] < 110)
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

  if( roll < referenceAngle - 25 || roll > referenceAngle + 25) {
    target_car_angle = (target_car_angle <= referenceAngle - 25 ? referenceAngle - 25 : target_car_angle);
    target_car_angle = (target_car_angle >= referenceAngle + 25 ? referenceAngle + 25 : target_car_angle);
  }
  else{
    target_car_angle = (target_car_angle <= referenceAngle - 8 ? referenceAngle - 8 : target_car_angle);
    target_car_angle = (target_car_angle >= referenceAngle + 8 ? referenceAngle + 8 : target_car_angle);
  }

  controlDataArr[0] = target_car_angle;
  controlDataArr[1] = target_velocity;
  lastShift = rightShift;
}

void control_servo() {
  float target_car_angle = controlDataArr[0];
  float err = target_car_angle - roll;
  steeringServo.drive(err * 0.1);//0.2
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