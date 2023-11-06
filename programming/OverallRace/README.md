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
//Include the required libraries
#ifndef Control_h
#define Control_h
#import <math.h>
#include "variables.h"

// returns the Sign of the number
int getSign(int number) {  // returns 1 when its positive and returns -1 when its negative
  return (number >= 0) ? 1 : -1;
}

// Verification that the reference angle is in a safe range, so that the ultrasonic sensors use safe values.
void checkSafeAngle() {
  if (abs(referenceAngle - roll) <= 12) {
    safeAngle = true;
  } else {
    safeAngle = false;
  }
}

// Examination by the ultrasonic sensors whether a curve is detected.
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
// Examination by the gyro sensors whether the car has already turned 3*360°, then stop car
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
// new gyro sensor data
void updateSensorData() {
  manager.readDistances(distances);
  roll = orientation.getTotalRoll();
}

// new camera data
void updateCameraData() {
  cam.firstBlockData();
  color = cam.get_color() * Run;
}

// Performs previous functions
void updateChecks() {
  checkSafeAngle();
  checkCurve();
  stopCheck();
}
// is started by RC_Control.ino and starts other functions, calculates the different angles and adjustments
void updateControlData() {
  updateChecks();
  updateSensorData();
  updateCameraData();
  int frontDistance = distances[0];
  int rightDistance = distances[1];
  int leftDistance = distances[2];

  //calculates reference speed

  const float FACTOR = 1.0f;     // 2
  const float FACTOR2 = 0.005f;  //0.01
  float target_velocity = frontDistance * FACTOR * tanh(frontDistance * FACTOR2);

  //calculates steering angle

  const float FACTOR_Steering = 5.0f * 0.8;//5
  const float wandAbstand = 7.0f;
  //const float FACTOR_Camera = 10.0f; ?

  // calculates relative distance to walls, how far it is relative to the right wall, wandAbstand = distance to wall
  rightShift = (leftDistance - wandAbstand) * abs(max(-1, color - 2)) - (rightDistance - wandAbstand) * abs(color - 1);

  // the closer it gets to the walls, the more it should steer to the side, right steering corresponds to positive steering angle
  // in order not to make the steering too jerky, a parameter tanh function is used

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

// Forward the calculated data to the servo, thus adjusts the steering.
void control_servo() {
  float target_car_angle = controlDataArr[0];
  float err = target_car_angle - roll;
  steeringServo.drive(err * 0.1);  // 0.1 Eröffnungsrennen
}

// adjusts the DC motor and thus the speed
void control_DC() {
  float target_velocity = controlDataArr[1];
  drivingDC.drive(target_velocity);
}

// executes previous functions
void drive() {
  updateControlData();
  control_servo();
  control_DC();
}

#endif
```

## variables.h
Here all important variables as well as pins for the Arduino are stored, which are needed for the program.
```c++
//include required libraries for sensors 
#ifndef variables_H
#define variables_H
#include <Arduino.h>
#include "CarOrientation.h"
#include "UltrasonicManager.h"
#include "MyServo.h"
#include "MyDC.h"
#include "camera.h"


// declare variables, sensors

// UltrasonicManager für die Arduino Ports
const uint8_t frontTrigPin = 2;
const uint8_t frontEchoPin = 3;
const uint8_t rightTrigPin = 4;
const uint8_t rightEchoPin = 5;
const uint8_t leftTrigPin = 6;
const uint8_t leftEchoPin = 7;
UltrasonicManager manager(frontTrigPin, frontEchoPin, rightTrigPin, rightEchoPin, leftTrigPin, leftEchoPin);


//ServoMotor Port Deklarierung
byte servoPin = 9;
byte servo_PWMpin = 38;

MyServo steeringServo(servoPin, servo_PWMpin);

//DC Motor Port Deklarierung
byte DC_PWMpin = 11;
MyDC drivingDC(DC_PWMpin);

//CarOrientation Deklarierung
CarOrientation orientation;

//Camera Deklarierung
camera cam;

//weitere nützliche Variablen
float controlDataArr[] = { 0, 0 };  //[desired Car Angle, Speed]
float distances[] = { 0, 0, 0 };
float roll;
float referenceAngle = 0;
float rightShift;
float lastShift = 10000000;
bool stop;
bool safeAngle;
uint8_t color = 0;
int secs = 3.5;
int velocity = 90;//Grundgeschwindigkeit
int Run = 1;// Unterscheidung Hindernisrennen, Eröffnungsrennen

// Pins reservieren und initialisieren
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
//include required libraries
#ifndef UltraSonic_h
#define UltraSonic_h
#include <NewPing.h>
#include <RunningMedian.h>


#define MAX_DISTANCE 400  // maximale DIstanz in cm
#define BUFFER_SIZE 7    // Anzahl der Messungen, welche für die Median Berechnung gespeichert werden

// Vermittlung zwischen Pins und Arduino, Median für die Wichtigkeit der Sensordaten um Messfehler zu minimieren
class UltrasonicManager {
public:
  UltrasonicManager(uint8_t frontTrigPin, uint8_t frontEchoPin, uint8_t rightTrigPin, uint8_t rightEchoPin, uint8_t leftTrigPin, uint8_t leftEchoPin)
    : frontSensor(frontTrigPin, frontEchoPin, MAX_DISTANCE),
      rightSensor(rightTrigPin, rightEchoPin, MAX_DISTANCE),
      leftSensor(leftTrigPin, leftEchoPin, MAX_DISTANCE),
      median_front(BUFFER_SIZE),
      median_right(BUFFER_SIZE),
      median_left(BUFFER_SIZE) {}



  void readDistances(float distances[3]) {
    int frontMess = frontSensor.ping_cm();
    int rightMess = rightSensor.ping_cm();
    int leftMess = leftSensor.ping_cm();
    if (frontMess != 0)
      median_front.add(frontMess);
    if (rightMess != 0)
      median_right.add(rightMess);
    if (leftMess != 0)
      median_left.add(leftMess);

    frontDistance = median_front.getMedian();
    //frontDistance = (frontDistance != 0) ? frontDistance : MAX_DISTANCE;
    rightDistance = median_right.getMedian();
    //rightDistance = (rightDistance != 0) ? rightDistance : MAX_DISTANCE;
    leftDistance = median_left.getMedian();
    //leftDistance = (leftDistance != 0) ? leftDistance : MAX_DISTANCE;

    distances[0] = frontDistance;
    distances[1] = rightDistance;
    distances[2] = leftDistance;


    /*frontDistance = frontSensor.ping_cm();
    frontDistance = (frontDistance != 0) ? frontDistance : MAX_DISTANCE;
    distances[0] = frontDistance;
    rightDistance = rightSensor.ping_cm();
    rightDistance = (rightDistance != 0) ? rightDistance : MAX_DISTANCE;
    distances[1] = rightDistance;
    leftDistance = leftSensor.ping_cm();
    leftDistance = (leftDistance != 0) ? leftDistance : MAX_DISTANCE;
    distances[2] = leftDistance;*/

    //printDistances();
  }
// Sensordaten für Bugfixing und Verständnis
  void printDistances() {
    Serial.print("front: ");
    Serial.print(frontDistance);
    Serial.print(" cm\ right: ");
    Serial.print(rightDistance);
    Serial.print(" cm\ left: ");
    Serial.print(leftDistance);
    Serial.println(" cm");
  }

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
};


#endif
```

## MyServo.h
Here the servo is addressed and the steering instructions from ControlRC.h are converted into the appropriate servo voltages.
```c++

// include required libraries
#ifndef MyServo_h
#define MyServo_h

#include <Arduino.h>
#include <Servo.h>
// Vermittlung zwischen Pins des Servos und Arduino, Berechnen der passenden Servospannung durch Servo Bibliothek
class MyServo {
private:
  byte pin;
  byte servo_PWMpin;

  byte limitR = 120;  //Winkel muss < 120
  byte limitL = 40;   //Winkel muss >35
  byte straightAngle = 90;
  Servo steeringServo;
public:
  MyServo() {}  //Default constructor  // nicht benutzen!

  MyServo(byte pin,byte servo_PWMpin) {
    this->pin = pin;
        this->servo_PWMpin = servo_PWMpin;
  }

  void init() {
    steeringServo.attach(pin);
    pinMode(pin, OUTPUT);
    int PWMSignal = 200;  //kann zwischen 0 and 255 varrieren
    analogWrite(servo_PWMpin, PWMSignal);
    reset();
  }

  void reset() {
    steeringServo.write(straightAngle);
  }
  
  //Rotate with in the limits
  double rotatAngle(byte angle) {
    if (angle > limitR) {
      angle = limitR;
    }
    if (angle < limitL) {
      angle = limitL;
    }
    steeringServo.write(angle);
    return angle;
  }

  // value =0 ==> straightAngle / value =1 ==> limitR / value =-1 ==> limitL
  double drive(float value) {
    if (value >= 0) {
      if (value > 1) {
        value = 1;
      }
      value = (limitR - straightAngle) * value + straightAngle;
    } else {
      if (value < -1) {
        value = -1;
      }
      value = (-limitL + straightAngle) * value + straightAngle;
    }
    steeringServo.write(value);
    return value;
  }
};


#endif

```

## MyDC.h
Here the required DC-Moter voltage is calculated, which is necessary to guarantee the required speed from the other programs.
```c++
//include required libraries
#ifndef MyDC_h
#define MyDC_h

#include <Arduino.h>
// Communication between DC pins, calculation of the DC voltage
class MyDC {
private:
  byte pin;
public:
  MyDC() {}  //Default constructor  // nicht benutzen!

  MyDC(byte pin) {
    this->pin = pin;
  }

  void init() {
    pinMode(pin, OUTPUT);
  }
  //velocity threshold value between 0 and 100
  void drive(float speed) {
    speed= (speed>100)? 100:speed;
    speed= (speed>0)?max(40,speed):speed;
    speed = map(speed, 0, 100, 0, 255);
    analogWrite(pin, speed);
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


