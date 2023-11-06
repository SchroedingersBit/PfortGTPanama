# Overall Race
The program contains a parameter that determines whether only the parts of the program that are important for the opening race must be used, since this does not require a camera script but only the ultrasonic sensors, or whether the camera data must also be used.



## RC_Control.ino
The program is started via the RC_Control.ino, which opens and manages all further classes.
```c++
//Include the required libraries and use the header files.
#include "variables.h"  // all the vars, actuators, and sensors are defined and initialized here.
#include "ControlRC.h"  // all functions for control, read sonsor and update variables are located here


//Create a serial connection, initialize the ports of the Arduino from the variable header and set th driving direction to forward.
void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);
  initializeHardware();
  drivingDC.setForward();
}


// Instructions for the car
void loop() {

  updateControlData();
  drivingDC.drive(velocity); // velocity is a constant
  control_servo();
  
  //print(); // will be turned on for Debugging

  // Termination condition after 3 laps have been run
  if (turns == 4 * numRounds) { // 12 turns achieved
    Stoptime = millis() + secs * 1000; // 
    while (distances[0] > 120 || !Sumcheck || Stoptime > millis()) {  // drive for at least Stoptime and until frontDistance is less than 120 and SumDistance is less than 100
      updateControlData();
      drivingDC.drive(velocity);
      control_servo();
    } // turn everathing off
    while (1) {
      control_servo();
      drivingDC.drive(0);
      delay(100);
    }
}


// different sensor data in the serial monitor for bugfixing and understanding what the robot is doing
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

```

## ControlRC.h
This header file contains all the logic for the car and ensures that the measured values are updated and calculated at the right time. This logic then passes on the new adjustments to the steering.

```c++
#ifndef Control_h
#define Control_h

//Include the required libraries.
#import <math.h>
#include "variables.h"

// returns the Sign of the number
int getSign(int number) {  // returns 1 when its positive and returns -1 when its negative
  return (number >= 0) ? 1 : -1;
}

// Verification that roll ic close to reference angle
void checkSafeAngle() {
  if (abs(referenceAngle - roll) <= 12)
    safeAngle = true;
  else
    safeAngle = false;
}

// check if he is near the wall when he sees a block and calculate a relative wallShift depending on the color seen and wallDistance
void checkSafeDistance() {                                                                            if (distances[1] * max(0, color - 1) + distances[2] * (1 - abs(color - 1)) - wallDistance * min(1, color) < 0) {
    wallcheck = 0;
    wallShift = (wallDistance - distances[1]) * (color - 1) + (distances[2] - wallDistance) * (2 - color);
  } else {
    wallcheck = 1;
    wallShift = 0;
  }
}

// checl if he is in a straight path
void checkSafeSum() { 
  if (distances[2] + distances[1] < 100 && safeAngle && distances[0] < 200) {
    Sumcheck = true;
  }
}


// Examination under various conditions as to whether a curve is recognized
void checkCurve() {
  if (distances[2] + distances[1] >= 140 && Sumcheck) {
    if (distances[0] < 100) {
      if (safeAngle) {
        if (Blockcheck || (!Blockcheck && abs(x_pos) - 45 > 0)) {
          if (abs(distances[2] - distances[1]) > 20) {
            sign = (1 - abs(direction)) * getSign(distances[1] - distances[2]) + direction;
            referenceAngle = referenceAngle + sign * 90;
            theoreticalAngle = theoreticalAngle + sign * 90;
            direction = sign;
            Sumcheck = false;
            turns += 1; 
          }
        }
      }
    }
  }
}

// when the robot isnt placed well he corrects that
void updateReference() {
  if (safeAngle)
    if (abs(rightShift) - abs(lastShift) >= 0) 
      referenceAngle = roll;
  sign = getSign(referenceAngle - theoreticalAngle);                                                
  referenceAngle = (abs(referenceAngle - theoreticalAngle) > 2 ? theoreticalAngle + sign * 2 : referenceAngle); 
}

// getting new sensor data
void updateSensorData() {
  manager.readDistances(distances);
  roll = orientation.getTotalRoll();
}

// getting the data from the camera
void updateCameraData() {
  cam.BlockData(Sumcheck);
  color = cam.get_color();
  x_pos = cam.get_x_pos();
  frameWidth = cam.get_frameWidth();
  Blockcheck = cam.get_Blockcheck();
  area = cam.get_Blocksize();
}

// Performs previous functions
void updateChecks() {
  checkSafeSum();
  checkSafeAngle();
  checkSafeDistance();
  checkCurve();
}

// it is started by RC_Control.ino and starts other functions, calculates the different angles and adjustments
void updateControlData() {
  updateSensorData();
  updateCameraData();
  updateChecks();

  int frontDistance = distances[0];
  int rightDistance = distances[1];
  int leftDistance = distances[2];

  // the following code calculates the targe_car_angle from the sensor Data

  // calculating rightShift
  rightShift = (leftDistance - rightDistance) * max(0, 1 - color * Run);

  // calculating camShift 
  float cam_Steering = (1.5 - 0.00635 * abs(x_pos)) * (abs(referenceAngle - roll) > 20 && (frontDistance < 20 || leftDistance * (2 -color) + rightDistance * (color - 1) < 30) ? 0 : 1)  + 2.5 * abs(Sumcheck - 1);
  camShift = (frameWidth * (3 * min(1, color) - 2 * color) - x_pos) * cam_Steering * wallcheck * (min(1, Sumcheck + direction * (1.5 - color) + 0.5)) * Run + wallShift;

  // calculating the target_car_angle
  const float FACTOR_Steering = 1.25f;
  float target_car_angle = -1 * FACTOR_Steering * (rightShift + camShift) + referenceAngle;

  updateReference();

  //looking if the target_car_angel is in the Bounds

  // lowerBound helps the robot to drive in the middle
  float lowerBound = (Sumcheck ? min(30, max(0, 0.30 * frontDistance - 30)) : 0);
  // upperBound makes sure that the car isn't far away from the referenceAngle
  int upperBound = 25;
  //Blockbound lets the car turn a lot if a Block is detected
  float BlockDistance = (frontDistance - (67578.1 / (area + 514.89) - 13.4431) - 80) * (0.5 * direction * (abs(color - 1) - min(1, 2 - color)) + max(0.5, 1 - 0.5 * color));
  float Blockbound = 120 * (BlockDistance < 0 && BlockDistance > -35 ? 0 : 1) + abs(1.25 * wallShift + referenceAngle) * abs(wallcheck - 1);

  // Here the Bounds are checked and if necessary, the target_car_angle will be updated
  sign = getSign(target_car_angle - referenceAngle);
  if (!Blockcheck * Run)
    target_car_angle = (abs(target_car_angle - referenceAngle) >= Blockbound ? referenceAngle + sign * Blockbound : target_car_angle);
  else if (abs(roll - referenceAngle) > upperBound)
    target_car_angle = referenceAngle;
  else
    target_car_angle = (abs(target_car_angle - referenceAngle) >= lowerBound ? referenceAngle + sign * lowerBound : target_car_angle);

  controlDataArr[0] = target_car_angle;
  lastShift = rightShift; 
}

// Forward the calculated data to the servo, thus adjusts the steering.
void control_servo() {
  float target_car_angle = controlDataArr[0];
  float err = target_car_angle - roll;
  steeringServo.drive(err * 0.1);
}
#endif
```

## variables.h
Here all important variables as well as pins for the Arduino are stored, which are needed for the program.
```c++
#ifndef variables_H
#define variables_H

// include the required libarries
#include <Arduino.h>
#include "CarOrientation.h"
#include "UltrasonicManager.h"
#include "MyServo.h"
#include "MyDC.h"
#include "camera.h"


// declaring the pins

// declaring the pins for the ultrasonic sensors
const uint8_t frontTrigPin = 2;
const uint8_t frontEchoPin = 3;
const uint8_t rightTrigPin = 4;
const uint8_t rightEchoPin = 5;
const uint8_t leftTrigPin = 6;
const uint8_t leftEchoPin = 7;
UltrasonicManager manager(frontTrigPin, frontEchoPin, rightTrigPin, rightEchoPin, leftTrigPin, leftEchoPin);


// declaring the pin for the Servo motor
byte servoPin = 9;
MyServo steeringServo(servoPin, servo_PWMpin);

// declaring the pin for the DC-motor
byte DC_PWMpin = 11;
MyDC drivingDC(DC_PWMpin);

// declaring the gyro-sensor
CarOrientation orientation;

// declaring the camera
camera cam;

//defining global variables
float controlDataArr[] = { 0, 0 };  //[desired Car Angle, Speed]
float distances[] = { 0, 0, 0 };
float roll;
float referenceAngle = 0;
float theoreticalAngle = 0;
int wallDistance = 20;
int wallcheck;
float wallShift;
float rightShift;
float camShift;
int turns = 0;
bool safeAngle;
bool Sumcheck = true;
float lastShift = 10000000;
int velocity = 40;
int direction = 0;
int sign;
int color;
int x_pos;
int frameWidth;
int Blockcheck;
int area;
int Run = 1;  //0 for open Challenge and 1 for obstacle Challenge


// initialize the sensors and motors
void initializeHardware() {
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

## UltrasonicManager.h
Here, the ultrasonic sensor data is read out and then the median is calculated for five values each.
```c++
#ifndef UltraSonic_h
#define UltraSonic_h

// include the required libarries
#include <NewPing.h>
#include <RunningMedian.h>


#define MAX_DISTANCE 400  // maximal distance in cm
#define BUFFER_SIZE 7    // Number of measurements saved for the median calculation

// connection between Pins and Arduino, median for minimizing measurement errors 
class UltrasonicManager {
private:
  NewPing frontSensor;  // Front Sensor
  NewPing rightSensor;  // Right Sensor
  NewPing leftSensor;   // Left  Sensor
  unsigned int frontDistance;
  unsigned int rightDistance;
  unsigned int leftDistance;
  RunningMedian median_front;
  RunningMedian median_right;
  RunningMedian median_left;

public:
  UltrasonicManager(uint8_t frontTrigPin, uint8_t frontEchoPin, uint8_t rightTrigPin, uint8_t rightEchoPin, uint8_t leftTrigPin, uint8_t leftEchoPin)
    : frontSensor(frontTrigPin, frontEchoPin, MAX_DISTANCE),
      rightSensor(rightTrigPin, rightEchoPin, MAX_DISTANCE),
      leftSensor(leftTrigPin, leftEchoPin, MAX_DISTANCE),
      median_front(BUFFER_SIZE),
      median_right(BUFFER_SIZE),
      median_left(BUFFER_SIZE) {}


  // get the Data from the Ultrasonic sensors
  void readDistances(float distances[3]) {
    int frontMess = frontSensor.ping_cm();
    int rightMess = rightSensor.ping_cm();
    int leftMess = leftSensor.ping_cm();

    // sort our mismeasurements
    if (frontMess != 0)
      median_front.add(frontMess);
    if (rightMess != 0)
      median_right.add(rightMess);
    if (leftMess != 0)
      median_left.add(leftMess);

    // take the median
    frontDistance = median_front.getMedian();
    rightDistance = median_right.getMedian();
    leftDistance = median_left.getMedian();

    // save the data in an array
    distances[0] = frontDistance;
    distances[1] = rightDistance;
    distances[2] = leftDistance;
  }
};
#endif

```

## MyServo.h
Here the servo is addressed and the steering instructions from ControlRC.h are converted into the appropriate servo voltages.
```c++

#ifndef MyServo_h
#define MyServo_h

// include the required libarries
#include <Arduino.h>
#include <Servo.h>


class MyServo {
private:
  // define the Control Pin
  byte pin;
  byte limitR = 133;   // Right limit for servo angle
  byte limitL = 20;    // Left limit for servo angle
  byte straightAngle = 90;  // Default angle for a straight position
  Servo steeringServo;

public:
  MyServo() {}    // Default constructor (empty)

  MyServo(byte pin) {
    this->pin = pin;
  }

  // Initialize the servo and set its initial position
  void init() {
    steeringServo.attach(pin);
    pinMode(pin, OUTPUT);
    reset();
  }

  // Reset the servo to the straight position
  void reset() {
    steeringServo.write(straightAngle);
  }

  // Control the servo angle based on the provided value
  // value = 0 => straightAngle; value = 1 => limitR; value = -1 => limitL
  double drive(float value) {
    value = min(1, max(-1, value));  // Ensure value is between -1 and 1
    int sgn = (value == 0 ? 0 : 0.5 * value / abs(value) - 0.5);
    value = (limitR * (sgn + 1) + limitL * sgn - straightAngle * 2 * (sgn + 0.5)) * value + straightAngle;
    // Calculate the angle based on the provided value
    steeringServo.write(value);  // Set the servo angle
    return value;
  }
};
#endif

```

## MyDC.h
Here the required DC-Moter voltage is calculated, which is necessary to guarantee the required speed from the other programs.
```c++
#ifndef MyDC_h
#define MyDC_h

#include <Arduino.h>

// Define a class named MyDC
class MyDC {
private:
  byte speedPin;  // Pin to control motor speed
  byte pinIN1;    // Motor input 1
  byte pinIN2;    // Motor input 2

public:
  // Default constructor (empty)
  MyDC() {}  

  // Parameterized constructor to initialize the motor pins
  MyDC(byte speedPin, byte pinIN1, byte pinIN2) {
    this->speedPin = speedPin;
    this->pinIN1 = pinIN1;
    this->pinIN2 = pinIN2;
  }
  // Initialize the motor pins as outputs
  void init() {
    pinMode(speedPin, OUTPUT);
    pinMode(pinIN1, OUTPUT);
    pinMode(pinIN2, OUTPUT);
  }

  // Drive the motor at a specified speed (0-100)
  void drive(float speed) {
    speed = (speed > 100) ? 100 : speed;  // Ensure speed is <= 100
    speed = (speed > 0) ? max(40, speed) : speed;  // Ensure a minimum speed of 40
    speed = map(speed, 0, 100, 0, 255);  // Map speed to the PWM range
    analogWrite(speedPin, speed);  // Set the motor speed
  }

  // Set the motor to move forward
  void setForward() {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, HIGH);
  }
  // Set the motor to move backward
  void setBackward() {
    digitalWrite(pinIN1, HIGH);
    digitalWrite(pinIN2, LOW);
  }
};
#endif



```

## CarOrientation.h
Here, the gyro sensor data is handled to determine the vehicle orientation and the angle of rotation traveled.
```c++
//include the required libarries
#include <Wire.h>
#include <Adafruit_BNO08x.h>

//Detection and calculation of vehicle alignment using data from the BNO08x sensor (gyroscope)
class CarOrientation {
private:
  Adafruit_BNO08x bno;
  sh2_SensorValue_t sensorValue;
  float initialAngle[3] = { 0.0, 0.0, 0.0 };
  float lastAngle[3] = { 0.0, 0.0, 0.0 };
  float totalRotation[3] = { 0.0, 0.0, 0.0 };
  bool initialAngleSet = false;

public:
  CarOrientation() {}
  //Initializes the BNO08x sensor and sets the corresponding reports.
  bool init() {
    if (!bno.begin_I2C()) {
      Serial.println("Failed to find BNO08x chip");
      return false;
    }
    setReports();
    delay(100);
    return true;
  }
  // Activates the required reports for the BNO08x sensor
  void setReports() {
    if (!bno.enableReport(SH2_GAME_ROTATION_VECTOR)) {
      Serial.println("Could not enable game vector");
    }
  }

  float quaternionToRoll(float qw, float qx, float qy, float qz) {
    float roll = atan2(2.0f * (qw * qz + qx * qy), qw * qw + qx * qx - qy * qy - qz * qz);
    return roll * (180.0 / M_PI);
  }
  //Updates the sensor values and calculates the vehicle alignment based on the quaternions
  void update() {
    if (bno.wasReset()) {
      setReports();
    }

    if (!bno.getSensorEvent(&sensorValue)) {
      return;
    }

    if (sensorValue.sensorId == SH2_GAME_ROTATION_VECTOR) {
      float qw = sensorValue.un.gameRotationVector.real;
      float qx = sensorValue.un.gameRotationVector.i;
      float qy = sensorValue.un.gameRotationVector.j;
      float qz = sensorValue.un.gameRotationVector.k;
      float roll = quaternionToRoll(qw, qx, qy, qz);

      if (!initialAngleSet) {
        lastAngle[2] = roll;
        initialAngle[2] = roll;
        initialAngleSet = true;
      } else {
        float deltaRoll = roll - lastAngle[2];

        if (deltaRoll > 180.0) {
          deltaRoll -= 360.0;
        } else if (deltaRoll < -180.0) {
          deltaRoll += 360.0;
        }

        totalRotation[2] += deltaRoll;
        lastAngle[2] = roll;
      }
    }
  }

  // Returns the relative roll angle change of the vehicle.
  // The relative roll angle is the roll angle compared to the initial position of the vehicle.
  // The value is negated to provide the roll angle in the correct orientation.
  float getRelativeRoll() {
    update();
    return (quaternionToRoll(sensorValue.un.gameRotationVector.real, sensorValue.un.gameRotationVector.i, sensorValue.un.gameRotationVector.j, sensorValue.un.gameRotationVector.k) - initialAngle[2]) * -1;
  }

  // Returns the total roll angle of the vehicle.
  // The total roll angle is the sum of all roll angle changes since the initial position of the vehicle.
  // The value is negated to provide the roll angle in the correct orientation.
  float getTotalRoll() {
    update();
    return totalRotation[2] * -1;
  }

  //Resets the initial angle and the status for the vehicle alignment
  void reset() {
    initialAngleSet = false;
  }
};


```

## camera.h
In camera the properties of the largest detected color block, such as color value height and width are output.
```c++
//include the required libarry
#include <Pixy2.h>

//search for red or green Block: green = 1 and red = 2
class camera {
private:
// declare the variables
  Pixy2 pixy;
  uint16_t Width;
  uint16_t Height;
  int8_t Blocks;
  uint16_t signature;
  int m_x;
  uint16_t m_y;
  unsigned long time;
  int last_signature = 0;
  int new_signature;
  bool Block_test;
  bool timer = true;
  int size;

public:
  camera() {}

  void init() {
    pixy.init();
    Width = pixy.frameWidth / 2 ;  // Width = 158
    Height = pixy.frameHeight; // Height = 208
  }

  // looking for a block
  void BlockData(bool Adder) {
    Block_test = true * timer;
    Blocks = pixy.ccc.getBlocks();  // getting the data from the camera
    if (Blocks && timer) {  // if any Block it detected and no change of Blocks were detected
      for (int i = 0; i < Blocks; i++) { // look through the blocks from the biggest to the smallest
        m_y = pixy.ccc.blocks[i].m_y;
        if (m_y >= Height * 0.2 && m_y <= Height * 0.85) { // the Block shouldn't lie high or low in the frame
          // get the Block Data
          new_signature = pixy.ccc.blocks[i].m_signature;
          m_x = pixy.ccc.blocks[i].m_x - Width;
          Block_test = false;
          size = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
          break; // stop if a block was detected
        }
      }
  }
  // to ensure the car doesn't hit the block immediatly when he stops seeing the Block we built a timer, to save the old data of the Block untill timer becomes true
  if (Block_test && timer) { // reset the variables
      new_signature = 0;
      m_x = 0;
      size = 0;
    } // start timer when he switches from block seen to no block seen or their was a change of color
    if (last_signature != 0 && (Block_test || (!Block_test && last_signature != new_signature))) {
      signature = last_signature;
      timer = false;
      time = millis() + 100UL + 100UL * !Adder;
    } else if (timer) {
      signature = new_signature;
    } // check whether the time has elapsed
    if (time < millis() && !timer)
      timer = true;

    last_signature = new_signature;

  int get_x_pos() {
    return m_x;
  }
  int get_color() {
    return signature;
  }
  int get_frameWidth() {
    return Width;
  }
  int get_Blockcheck() {
    return Block_test;
  }
  int get_Blocksize() {
    return size;
  }
};

```


