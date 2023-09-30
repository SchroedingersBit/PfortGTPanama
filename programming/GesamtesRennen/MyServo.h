//Inkludieren der erforderlichen Bibliotheken.
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
    int PWMSignal = 200;  //can vary betwwen 0 and 255
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
