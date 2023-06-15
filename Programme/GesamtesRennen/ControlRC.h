//Inkludieren der erforderlichen Bibliotheken.
#ifndef Control_h
#define Control_h
#import <math.h>
#include "variables.h"

// Überprüfung, ob der Referenzwinkel in einem sicheren Berreich liegt, sodass dabei die Ultraschall-Sensoren sichere Werte verwenden
void checkSafeAngle() {
  if (abs(referenceAngle - roll) <= 12) {
    safeAngle = true;
  } else {
    safeAngle = false;
  }
}

//  Untersuchung durch die Ultraschall-Sensoren, ob eine Kurve detektiert wird
void checkCurve() {
  if (distances[2] + distances[1] >= 130) {
    if (distances[0] < 110 && cam.getX_pos() >=5) {
      if (safeAngle) {
        cam.set_color();
        if (distances[2] - distances[1] > 20) {
          referenceAngle -= 90;
        } else if (distances[2] - distances[1] < -20) {
          referenceAngle += 90;
        }
      }
    }
  }
}
// Untersuchung durch die Gyrosensoren, ob der Auto sich bereits 3*360° gedreht hat, dann Auto stoppen
void stopCheck() {
  /*if (referenceAngle > 0)
    secs = 2.7;
  else
    secs = 3.5;*/
  int numberOfRounds = 3;
  if (abs(referenceAngle) >= numberOfRounds * 360 - 20)
    stop = true;
  else
    stop = false;
}
// neue Gyro-Sensordaten
void updateSensorData() {
  manager.readDistances(distances);
  roll = orientation.getTotalRoll();
}

// neue Kameradaten
void updateCameraData() {
  cam.firstBlockData();
  color = cam.get_color() * Run;
}

// Führt vorherige Funktionen aus
void updateChecks() {
  checkSafeAngle();
  checkCurve();
  stopCheck();
}
// wird durch RC_Control.ino gestartet und startet andere Funktionen, berechnet die verschiedenen Winkel und Anpassungen
void updateControlData() {
  updateChecks();
  updateSensorData();
  updateCameraData();
  int frontDistance = distances[0];
  int rightDistance = distances[1];
  int leftDistance = distances[2];

  //berechnet Richtgeschwindigkeit

  const float FACTOR = 1.0f;     // 2
  const float FACTOR2 = 0.005f;  //0.01
  float target_velocity = frontDistance * FACTOR * tanh(frontDistance * FACTOR2);

  //berechnet Steuerwinkel

  const float FACTOR_Steering = 5.0f * 0.8;//5
  const float wandAbstand = 7.0f;
  //const float FACTOR_Camera = 10.0f; ?

  // Calcualte relative lateral position on the road
  // (how far towards right border is the cart located)
  // + Kamera erklären
  rightShift = (leftDistance - wandAbstand) * abs(max(-1, color - 2)) - (rightDistance - wandAbstand) * abs(color - 1);

  // The closer the cart is to the right border, the more it should steer to the left (and vice versa).
  // (Steering to the right = positive steering angle)
  // To achieve this in a smooth way, a parameter-tuned tanh function can be used.

 float target_car_angle = -1 * (FACTOR_Steering / (abs(min(0, color - 1)) * min(frontDistance, 40) + min(1, color) * 5)) * rightShift * tanh(abs(rightShift)) + referenceAngle;
   
  if (cam.get_blocks() * Run) {
    target_car_angle = (target_car_angle >= referenceAngle + 50 ? referenceAngle + 50 : target_car_angle);
    target_car_angle = (target_car_angle <= referenceAngle - 50 ? referenceAngle - 50 : target_car_angle);
  } else if (abs(roll - referenceAngle) > 30) {
    target_car_angle = (target_car_angle <= referenceAngle - 35 ? referenceAngle - 35 : target_car_angle);
    target_car_angle = (target_car_angle >= referenceAngle + 35 ? referenceAngle + 35 : target_car_angle);
  } else {
    target_car_angle = (target_car_angle <= referenceAngle - 5 ? referenceAngle - 5 : target_car_angle);
    target_car_angle = (target_car_angle >= referenceAngle + 5 ? referenceAngle + 5 : target_car_angle);
  }
  controlDataArr[0] = target_car_angle;
  controlDataArr[1] = target_velocity;
  lastShift = rightShift;
  //if (abs(rightShift) < 5)
  //velocity = 50;
}

// Weiterleiten der berechneten Daten zum Servo, passt damit die Lenkung an
void control_servo() {
  float target_car_angle = controlDataArr[0];
  float err = target_car_angle - roll;
  steeringServo.drive(err * 0.1);  // 0.1 Eröffnungsrennen
}

// passt den DC Motor und damit die Geschwindigkeit an
void control_DC() {
  float target_velocity = controlDataArr[1];
  drivingDC.drive(target_velocity);
}

// führt vorherige Funktionen aus
void drive() {
  updateControlData();
  control_servo();
  control_DC();
}

#endif