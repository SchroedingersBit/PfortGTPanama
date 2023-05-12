#ifndef MyServo_h
#define MyServo_h

#include <Arduino.h>
#include <Servo.h>
class MyServo {
private:
  byte pin;
  byte limitR = 120;  //angle must be< 120
  byte limitL = 40;   //angle must be >35
  byte straightAngle = 90;
  Servo steeringServo;
public:
  MyServo() {}  //Default constructor  // do not use

  MyServo(byte pin) {
    this->pin = pin;
  }

  void init() {
    steeringServo.attach(pin);
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
