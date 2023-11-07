#ifndef Control_h
#define Control_h
#import <math.h>
#include "variables.h"


int getSign(int number) {  // returns 1 when its positive and returns -1 when its negative
  return (number >= 0) ? 1 : -1;
}

void checkSafeAngle() {  // it returns true when roboter is in a 12 degree range of the referenceAngle and false when not
  if (abs(referenceAngle - roll) <= 12)
    safeAngle = true;
  else
    safeAngle = false;
}

void checkSafeDistance() {                                                                                          // to ensure that the car does not hit the wall when the car sees a block
  if (distances[1] * max(0, color - 1) + distances[2] * (1 - abs(color - 1)) - wallDistance * min(1, color) < 0) {  // when color is green and left Distance is lass than wallDistance, wallcheck = 0 and a correction term called wallShift is calculated. the same applies for a red block but there we use the right sensor
    wallcheck = 0;
    wallShift = (wallDistance - distances[1]) * (color - 1) + (distances[2] - wallDistance) * (2 - color);
  } else {  // if it isnt near the wall for the specific block it returns wallcheck = 1 and no correction term is needed
    wallcheck = 1;
    wallShift = 0;
  }
}

void checkSafeSum() {  // when the car is in a straight section it is true, it is used in many areas.
  if (distances[2] + distances[1] < 160 && abs(referenceAngle - roll) <= 15 && distances[0] < 200) {
    Sumcheck = true;  // from now on i will speak about sumcheck as no curve detected when it is true and curve detected when its false
  }
}

void checkCurve() {                                                                          // Here we check if a curve is seen, where many conditions contribute
  if (distances[2] + distances[1] >= 140 && Sumcheck) {                                      //                                    // the car sees a great Sumdistance from left and right sensor and he was in a straight section before the great Sumdistance
    if (distances[0] < 100) {                                                                // the frontdistance is small
      if (safeAngle) {                                                                       // we are near the referenceAngle, to ensure the robot doesnt see a curve when he is correcting because of the block or because he is near a wall
        if (Blockcheck || (!Blockcheck && abs(x_pos) - 45 > 0)) {                            // The car either sees no Block or the Block he sees is not in the mid of the frame to ensure the robot doesnt see a curve when he has finished turning a curve
          if (abs(distances[2] - distances[1]) > 40) {                                       // there is difference between the reading of the side sensors which helps whith mismeausrments of the side sensors
            sign = (1 - abs(direction)) * getSign(distances[1] - distances[2]) + direction;  // the rotationdirection is determined through the difference of the side sensors whit the getSign function. After the first Curve the variable direction is set to 1 or -1 and therefore the getSign function is always 0 and we get either 1 or -1. In summary the first curve determines the rotationdirection for correcting false readings
            direction = sign;
            if (getSign(distances[1] - distances[2]) == direction) {
              referenceAngle = referenceAngle + sign * 90;      // the referenceAngle is updated whit the rotationdirection which comes from the variable sign
              theoreticalAngle = theoreticalAngle + sign * 90;  // the same as referenceAngl                              // Before direction was set after the first curve it was 0 and didnt have any influence on the sign variable.
              Sumcheck = false;                                 // This means that we are in a curve now and will only be set true again if checkSafeSum returns true. which means more or less when the curve ends, there are special cases which has no big contribuition to the code. This makes sure that no second curve is detected before we were in a straight path
              turns += 1;
            }  // this is used for the stop check. when it is 12 the car will stop after the conditions in RC_Control.ino
          }
        }
      }
    }
  }
}

// when the robot isnt placed well
void updateReference() {                        // We assume that we will be able to place the robot almost straight with an error of +-2 degrees.
  if (safeAngle)                                // So when we are in a 12degree range of ReferenceAngle
    if (abs(rightShift) - abs(lastShift) >= 0)  // For simplificatin i will assume that the roboter is placed with 2 degrees to the right from the straight line. Now he moves after ReferenceAngle which is making him get near the right wall, which is opposed by rightShift and he moves with an offset to the left of the ReferenceAngle. Therfore Roll is near to true zero than ReferenceAngle which is why we set it equall to roll. playing this game many times, our car gets closer to true zero.
      referenceAngle = roll;
  sign = getSign(referenceAngle - theoreticalAngle);                                                             // here we get in which direction referenceAngle was improved
  referenceAngle = (abs(referenceAngle - theoreticalAngle) > 2 ? theoreticalAngle + sign * 2 : referenceAngle);  // with this function we make sure that referenceAngle isnt changed more than two degrees if so we set it to only be 2 degrees apart in the direction determined by sign
}

void updateSensorData() {  // getting the ultrasonic senor data and the roll from the gyro
  manager.readDistances(distances);
  roll = orientation.getTotalRoll();
}

void updateCameraData() {  // getting the data from the camera and giving the class Sumcheck
  cam.BlockData(Sumcheck);
  color = cam.get_color();  // 1 is green and 2 is red, 0 is no color detected
  x_pos = cam.get_x_pos();
  frameWidth = cam.get_frameWidth();
  Blockcheck = cam.get_Blockcheck();  // true is no block seen and false means block seen
  area = cam.get_Blocksize();
}

void updateChecks() {  // the methods are called
  checkSafeSum();
  checkSafeAngle();
  checkSafeDistance();
  checkCurve();
}

void updateControlData() {  // here the true calculation happens
  // updating everything
  updateSensorData();
  updateCameraData();
  updateChecks();
  // defining frontDistance, LeftDistance and rightDistance for a cleared view through the code
  int frontDistance = distances[0];
  int rightDistance = distances[1];
  int leftDistance = distances[2];

  if (turns < 8)  // save all colors till the seventh curve
    redturn = color;
  else if (turns == 8 && area > 1500 && !Sumcheck) // if on the last turn there was a block in front of the car, this is the last Block!
    redturn = color;

    //compute Steering Angle

    const float FACTOR_Steering = 1.25f;                                                                                                                                                                                                                             // this is an empiracally determined factor which is a p factor to transform rightShift and camShift in a useful target car angle
  float cam_Steering = ((1.4 - 0.4) / (2 * frameWidth) * (2 * color - 3) * x_pos + (1.4 + 0.4) / 2) * (abs(referenceAngle - roll) > 20 && (frontDistance < 10 || leftDistance * (2 - color) + rightDistance * (color - 1) < 20) ? 0 : 1) + 2.5 * abs(Sumcheck - 1);  // this is a p factor for the camShift to transform it into something usable like rightshift. It varies depending if a curve was detected or not. when a curve is detected it is bigger to allow the robot to correct more,
  hh = cam_Steering;                                                                                                                                                                                                                                                 //so he can absolve the curve

  rightShift = (leftDistance - rightDistance) * max(0, 1 - color * Run);                                                                                                                                                               // the rightShift is calculated as the difference between the left and the right sensor. Howerver when a block is seen, color is either one or two and therfore rightShift is zero which does not apply when color is zero.
  camShift = (frameWidth * (3 * min(1, color) - 2 * color) - x_pos) * cam_Steering * wallcheck * min(1, Sumcheck + (direction * x_pos * (color * (color + direction) == 2 ? 0 : 1) >= -80 && area >= 200 ? 1 : 0)) * Run + wallShift;  // camShift is calculated differently for different blocks. for green blocks we do frameWidth -x_pos and for red Block it is -frameWidth - x_pos. this is then multiplied with the p factor cam_steering. This is multiplied with wallcheck to ensure it does not hit the wall.
                                                                                                                                                                                                                                       //The second last factor is important for the case
                                                                                                                                                                                                                                       // when the ca turns to the left and sees a green Block sinse there isnt any boundary it will start correcting to the left and it will hit the wall. to perimt this from happening we prohibit the car to correct when it turns left and sees green or turns right and sees
                                                                                                                                                                                                                                       // red till the curve is finished. wallShift is summed to CamShift for the case wallchek happens to be zero so it gets away from the wall. it is important to notice that CamShift is zero when no Block is detected
  float target_car_angle = -1 * FACTOR_Steering * (rightShift + camShift) + referenceAngle;                                                                                                                                            // the target_car_angle is simply calculated with a p factor FACTOR_Steering and adding referenceAngle. the p factor is multiplied to the sum of the rightShift and CamShift where only one of the two has a value in the calculation

  updateReference();  // the method is called here, it could have been done above but since it came last it just stayed there
  // Now comes the last part of the hard calculation where we insert Bounds.
  int upperBound = 25;                                                                                                                                                        // This is important for the case the car is way of from the referenceAngle
  float lowerBound = (Sumcheck ? min(30, max(0, 0.30 * frontDistance - 30)) : 0);                                                                                             // This Bound is for the correction towards the mid of the field, it also depends on two Factors: if there is a curve detected it is zero so he will go after referenceAngle first. if no curve is detected the Bound term should shrink when the frontDistance gets smaller so when we go into the curve we are in the safeangle.
                                                                                                                                                                              //It has its maximum at 30 degrees when frontdistance is 200 and shrinks linearly till 0 when frontdistance is at 100
  float BlockDistance = (frontDistance - (67578.1 / (area + 514.89) - 13.4431) - 80) * (0.5 * direction * (abs(color - 1) - min(1, 2 - color)) + max(0.5, 1 - 0.5 * color));  // this is an empirically determined equation which resulted from the observation that the car corrects for the Block at the curve before getting to the curve. Since this Block doesnt get out of the view of the camera the car hits thw wall in the worst case. Firstly we saw, that the Blocksize of the Block didnt vary much
                                                                                                                                                                              // with a constant parallel distance when the Block was moved horzontally. Here the idea emerged to calculate the parallel distance of the Block to the car using the area of the Block. Measuring the size of the Block at different parallel distances and plotting them in a diagramm we estimated them through an a/(b+x) + c
                                                                                                                                                                              // function where the numbers can be seen in the equation. Subtracting this distance from the frontdistance we get the Blocks distance to thw wall. since we know that the Block in front the curve has a distance of 100cm and the Block at the curve a distance of less than 60cm to the wall, we subtracted the Blockdistance with
                                                                                                                                                                              // eighty. summarily Blockdistance is >= 0 when the block is in front and less than zero when it is at the curve. Later we observed that it was good when he corrected for the red Blocks at the curve when he turned to the left. so we multiply this distance with a function which is 1 if he turns left and there is a green block
                                                                                                                                                                              // or turns to the right and there is a red block and returns one when the contrary.
  float Blockbound = (130 + 55 * abs(Sumcheck - 1)) * (BlockDistance < 0 && BlockDistance > -35 || area < 100 ? 0 : 1) + abs(1.25 * wallShift + 25) * abs(wallcheck - 1);     // Blockbound is set to 120 but is 0 when there is a block at the curve, which meanas Blockdistance is less than zero. Later on we saw the robot not correcting if the front Block was exactly in front of the frontsensor, because it changed the measurment of the frontsensor. so we added also a lowerbound which is -35.
                                                                                                                                                                              // last but not least wr add the angle calculated through the wallShift which only applies if wallcheck is false. which means when he is near a wall
  sign = getSign(target_car_angle - referenceAngle);                                                                                                                          // here we get the sign of the required correction
  if (!Blockcheck * Run)                                                                                                                                                      // The first thing he looks for is the Blockbound, if a Block is detected than you should move according to the Blockbound and nothing else, so the car maneuvers the Block
    target_car_angle = (abs(target_car_angle - referenceAngle) >= Blockbound ? referenceAngle + sign * Blockbound : target_car_angle);                                        // if the calculated target_car_angle is greater than the Blockbound set it equal to the Blockbound else it can stay as it is
  else if (abs(roll - referenceAngle) > upperBound)                                                                                                                           // the next thing in the herachy is the upperBound, if we are far from referenceAngle set target_car_angle equal to referenceAngle excluding everything else
    target_car_angle = referenceAngle;
  else                                                                                                                                  // if we see no Block and we are around the referenceAngle only then he is allowed to think about moving towards the mid which is done here
    target_car_angle = (abs(target_car_angle - referenceAngle) >= lowerBound ? referenceAngle + sign * lowerBound : target_car_angle);  // if target_car_angle is greater than lowerBound make it equall to lower Bound

  controlDataArr[0] = target_car_angle;  // Saving target_car_angle in an array
  lastShift = rightShift;                // saving rightShift as LastShift
}

void control_servo() {
  float target_car_angle = controlDataArr[0];  //Aquiring the data
  float err = target_car_angle - roll;         // Since target_car_angle is the angle of the car we should not correct anymore if target_car_angle = roll, which is why we calculate an error
  steeringServo.drive(err * 0.1);              //0.1 // The error is lastly multiplied by an p factor to transform it in something more usable to the Servo
}

#endif
