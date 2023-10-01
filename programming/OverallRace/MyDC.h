//Inkludieren der erforderlichen Bibliotheken.
#ifndef MyDC_h
#define MyDC_h

#include <Arduino.h>
// Vermittlung zwischen Pins des DC`s, Berechenen  der DC Spannung
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
  //Geschwindigkeit muss zwischen 0 und 100 sein
  void drive(float speed) {
    speed= (speed>100)? 100:speed;
    speed= (speed>0)?max(40,speed):speed;
    speed = map(speed, 0, 100, 0, 255);
    analogWrite(pin, speed);
  }
};
#endif
