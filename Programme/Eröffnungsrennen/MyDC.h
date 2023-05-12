#ifndef MyDC_h
#define MyDC_h

#include <Arduino.h>
class MyDC {
private:
  byte pin;
public:
  MyDC() {}  //Default constructor  // do not use

  MyDC(byte pin) {
    this->pin = pin;
  }

  void init() {
    pinMode(pin, OUTPUT);
  }
  //speed must be between 0 and 100
  void drive(float speed) {
    speed= (speed>100)? 100:speed;
    speed= (speed>0)?max(40,speed):speed;
    speed = map(speed, 0, 100, 0, 255);
    analogWrite(pin, speed);
  }
};
#endif