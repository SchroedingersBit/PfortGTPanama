# Hindernisrennen

## RC_Control.ino
```c++
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
```

## variables.h
Zu den alten variables.h Programm kommen nun zusätzliche Variablen für die Kamera hinzu.
```c++
#ifndef variables_H
#define variables_H
#include <Arduino.h>
#include "CarOrientation.h"
//1) Include necessary libraries for your sensors and actuators here
#include "UltrasonicManager.h"
#include "MyServo.h"
#include "MyDC.h"
#include "SerialIO.h"
#include "camera.h"


//Declare your variables, sensors, and actuators

//UltrasonicManager
const uint8_t frontTrigPin = 2;
const uint8_t frontEchoPin = 3;
const uint8_t rightTrigPin = 4;
const uint8_t rightEchoPin = 5;
const uint8_t leftTrigPin = 6;
const uint8_t leftEchoPin = 7;
UltrasonicManager manager(frontTrigPin, frontEchoPin, rightTrigPin, rightEchoPin, leftTrigPin, leftEchoPin);


//ServoMotor
byte servoPin = 9;
MyServo steeringServo(servoPin);

//DC motor
byte DC_PWMpin = 11;
MyDC drivingDC(DC_PWMpin);


//Serial IO
//SerialIO IO;

//CarOrientation
CarOrientation orientation;

//Camera
camera cam;

//Vars
float controlDataArr[] = { 0, 0 };  //[desired Car Angle, Speed]
float distances[] = { 0, 0, 0 };
float roll;
float referenceAngle = 0;
float rightShift;
bool stop;
bool safeAngle;
float lastShift = 10000000;
uint8_t color = 0;


void initializeHardware() {
  //IO.init();
  steeringServo.init();
  drivingDC.init();
  cam.init();
  if (!orientation.init()) {
    Serial.println("Failed to initialize CarOrientation");
    while (1) {
      delay(10);
    }
  }
}


#endif
```

## ControlIRC.h

```c++
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
```

## camera.h

```c++
#include <Pixy2.h>



class camera {
private:
  Pixy2 pixy;
  int blocks;
  int frameWidth;
  int x_pos;
  uint16_t signature;
  int width;

public:
  camera() {}

  void init() {
    pixy.init();
    frameWidth = pixy.frameWidth;  //316
  }

  void firstBlockData() {
    blocks = pixy.ccc.getBlocks();
    width = pixy.ccc.blocks[0].m_width;
    if (blocks) {
      if (width > 15) {
        x_pos = pixy.ccc.blocks[0].m_x - frameWidth / 2;
        signature = pixy.ccc.blocks[0].m_signature;
      }
    }
  }

  int getX_pos() {
    return x_pos;
  }
  int get_color() {
    return signature;
  }
  float get_width() {
    return width;
  }
  float get_blocks() {
    return blocks;
  }
   void set_color() {
    signature=0;
  }
};
```

