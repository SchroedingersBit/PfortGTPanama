#ifndef MyServo_h
#define MyServo_h

#include <Arduino.h>
#include <Servo.h>
class MyServo {
private:
  byte pin;
  byte servo_PWMpin;
  byte limitR = 133;  //angle must be< 120
  byte limitL = 20;   //angle must be >35
  byte straightAngle = 90;
  Servo steeringServo;

public:
  MyServo() {}  //Default constructor  // do not use

  MyServo(byte pin, byte servo_PWMpin) {
    this->pin = pin;
  }

  void init() {
    steeringServo.attach(pin);
    pinMode(pin, OUTPUT);
    int PWMSignal = 255;  //can vary betwwen 0 and 255
    analogWrite(servo_PWMpin, PWMSignal);
    reset();
  }

  void reset() {
    steeringServo.write(straightAngle);
  }

  // value =0 ==> straightAngle / value =1 ==> limitR / value =-1 ==> limitL
  double drive(float value) {
    value = min(1, max(-1, value));
    int sgn = (value == 0 ? 0 : 0.5 * value / abs(value) - 0.5);
    value = (limitR * (sgn + 1) + limitL * sgn - straightAngle * 2 * (sgn + 0.5)) * value + straightAngle;
    steeringServo.write(value);
    return value;
  }
};


#endif
