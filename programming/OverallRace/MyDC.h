#ifndef MyDC_h
#define MyDC_h

#include <Arduino.h>
class MyDC {
private:
  byte speedPin;
  byte pinIN1;
  byte pinIN2;
public:
  MyDC() {}  //Default constructor  // do not use

  MyDC(byte speedPin, byte pinIN1, byte pinIN2) {
    this->speedPin = speedPin;
    this->pinIN1 = pinIN1;
    this->pinIN2 = pinIN2;
  }

  void init() {
    pinMode(speedPin, OUTPUT);
    pinMode(pinIN1, OUTPUT);
    pinMode(pinIN2, OUTPUT);
  }
  //speed must be between 0 and 100
  void drive(float speed) {
    speed = (speed > 100) ? 100 : speed;
    speed = (speed > 0) ? max(40, speed) : speed;
    speed = map(speed, 0, 100, 0, 255);
    analogWrite(speedPin, speed);
  }
  void setForward() {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, HIGH);
  }
    void setBackward() {
    digitalWrite(pinIN1, HIGH);
    digitalWrite(pinIN2, LOW);
  }
};
#endif