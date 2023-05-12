#ifndef SerialIO_h
#define SerialIO_h
#include <Arduino.h>
#include "variables.h"

class SerialIO {
private:
  int baud = 9600;
public:
  SerialIO() {}  //Default constructor
  void init() {
    Serial.begin(baud);
    Serial.flush();
  }

  bool available() {
    return Serial.available();
  }
  double getDouble() {
    double value;
    String line = Serial.readStringUntil('\n');
    line.trim();
    value = line.toDouble();
    return value;
  }
  String getLine() {
    String line;
    line = Serial.readStringUntil('\n');
    line.trim();
    return line;
  }

  void getMoveData(float moveDataArr[]) {
    String line;
    float setAngle, setSpeed;
    line = Serial.readStringUntil('\n');
    int angleIndex = line.indexOf("Angle=") + 6;
    int speedIndex = line.indexOf("Speed=") + 6;
    setAngle = line.substring(angleIndex, line.indexOf(';', angleIndex)).toFloat();
    setSpeed = line.substring(speedIndex, line.indexOf(';', speedIndex)).toFloat();
    moveDataArr[0] = setAngle;
    moveDataArr[1] = setSpeed;
  }
};

#endif