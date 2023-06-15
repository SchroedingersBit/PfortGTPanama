# Eröffnungsrennen
Beim Eröffnungsrennen bezieht das Auto Ultraschall - und Gyrosensordaten um in der Mitte zwischen den aufgestellten banden fährt. Befindet es sich nicht mittig, z.B. wegen einer Kurve, wird ein neuer Winkel ausgerechnet, der angestrebt werden soll. Dieser wird dann langsam korrigiert um eine ruckartige bewegung zu verhindern und darf zudem eine maximale größe nicht überschreiten, welcher durch den Gyrosensor definiert wird. Dieser zählt zudem die zurückgelegten Kurven, um nach drei Runden zu stoppen.
## Flowchart 
```mermaid
flowchart TB;
USS-Code --> D{Distanz < 400} --> M{5 Messungen};
subgraph Median
M-->ME{Median berechen} -->N{Neue Messung} -->|Update ältester Messwert| M;
end
ME --> Rightshift;

subgraph Rightshift
LR{Differenz Links Rechts USS} --> |Formelverarbeitung| W{Winkel, welches Auto lenken muss};
end
Rightshift --> Lenkung;

subgraph Lenkung
Berrechnung --> S{Servowinkel der eingestellt wird}
end
Lenkung --> Z{Auto passt Lenkeinstellungen an}
```

## RC_Control.ino
Gestartet wird das Programm über das RC_Control.ino, welches alle weiteren Klassen öffnet und managed.
```c++
//Inkludieren der erforderlichen Bibliotheken und Verwendung der Header Datein
#include "variables.h"  // all the vars, actuators, and sensors are defined and initialized here.
#include "ControlRC.h"  // all functions for control, read sonsor and update variables are located here


//serielle Verbindung erstellen und die Ports des Arduino aus dem Variable Header initialisieren
void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  initializeHardware();
}


unsigned long stopTime = 0;  // um zu wisen, wann gestoppt werden muss
//  Anweisungen für das Auto
void loop() {
  //drive();
  updateControlData();
  drivingDC.drive(45);
  //control_DC();
  control_servo();
  
  //print();





  //Abbruchbedingung nachdem 3 Runden gefahren wurden
  if (stop && stopTime == 0) {
    stopTime = millis() + 2000;  // weiterfahren für 3 weitere Sekunden
  }

  if (stopTime > 0 && millis() >= stopTime) {
    while (1) {
      steeringServo.drive(0);
      drivingDC.drive(0);
      delay(100);
    }
  }
}


// verschiedenen Sensordaten im Seriellen Monitor für Bugfixing und Verständnis von dem, was der Roboter tut
void print() {
  //manager.printDistances();
  Serial.print("referenceAngle ");
  Serial.print(referenceAngle);
  Serial.print("       rightShift ");
  Serial.print(rightShift);
  Serial.print("       roll ");
  Serial.print(roll);
  Serial.print("       targetAngle ");
  Serial.println(controlDataArr[0]);
}

```

## ControlRC.h
In dieser Header Datei sind alle Funktionen zur Steuerung des Fahrzeugs basierend auf den Sensordaten enthalten.
Es werden Überprüfungen für die Sicherheitswinkel, Kurven und zur Aktualisierung der Referenzwinkeln initialisiert wodurch die Fahrzeugsteuerung garantiert wird.

```c++
//Inkludieren der erforderlichen Bibliotheken.
#ifndef Control_h
#define Control_h
#import <math.h>
#include "variables.h"

// Überprüfung, ob der Referenzwinkel in einem sicheren Berreich liegt, sodass dabei die Ultraschall-Sensoren sichere Werte verwenden
void checkSafeAngle() {
  if (referenceAngle - 10 <= roll && roll <= referenceAngle + 10) {
    safeAngle = true;
  } else {
    safeAngle = false;
  }
}

//  Untersuchung durch die Ultraschall-Sensoren, ob eine Kurve detektiert wird
void checkCurve() {
  if (abs(rightShift) >= MAX_DISTANCE - 340) {
    if (distances[0] < 90)
      if (safeAngle)
        if (rightShift > 0)
          referenceAngle -= 90;
        else referenceAngle += 90;
  }
}
// Anpassung des Referenz Winkels, um ein gerades Fahren zu garantieren
void updateReference() {
  if (safeAngle)
    if ((abs(rightShift) - abs(lastShift)) >= 0)
      referenceAngle = roll;
}
// Untersuchung durch die Gyrosensoren, ob der Auto sich bereits 3*360° gedreht hat, dann Auto stoppen
void stopCheck() {
  int numberOfRounds = 3;
  if (abs(referenceAngle) >= numberOfRounds * 360-20)
    stop = true;
  else
    stop = false;
}
// neue Sensordaten
void updateSensorData() {
  manager.readDistances(distances);
  roll = orientation.getTotalRoll();
}
// Führt vorherige Funktionen aus
void updateChecks() {
  checkSafeAngle();
  checkCurve();
  stopCheck();
}
// wird durch RC_Control.ino gestartet und startet andere Funktionen, berechnet die verschiedenen WInkel und Anpassungen
void updateControlData() {
  updateSensorData();
  updateChecks();
  int frontDistance = distances[0];
  int rightDistance = distances[1];
  int leftDistance = distances[2];

  //berechnet Richtgeschwindigkeit

  const float FACTOR = 1.0f;     // 2
  const float FACTOR2 = 0.005f;  //0.01
  float target_velocity = frontDistance * FACTOR * tanh(frontDistance * FACTOR2);

  //berechnet Steuerwinkel 

  const float FACTOR_Steering = 5.0f;  //5
  // Calcualte relative lateral position on the road
  // (how far towards right border is the cart located)
  rightShift = leftDistance - rightDistance;
  // The closer the cart is to the right border, the more it should steer to the left (and vice versa).
  // (Steering to the right = positive steering angle)
  // To achieve this in a smooth way, a parameter-tuned tanh function can be used.
  float target_car_angle = -1 * (FACTOR_Steering / min(frontDistance, 40)) * rightShift * tanh(abs(rightShift)) + referenceAngle;
  updateReference();

  controlDataArr[0] = target_car_angle;
  controlDataArr[1] = target_velocity;
  lastShift = rightShift;
}
// Weiterleiten der berechneten Daten zum Servo, passt damit die Lenkung an
void control_servo() {
  float target_car_angle = controlDataArr[0];
  float err = target_car_angle - roll;
  steeringServo.drive(err * 0.1);
}
// passt den DC`s und damit die Geschwindigkeit an
void control_DC() {
  float target_velocity = controlDataArr[1];
  drivingDC.drive(target_velocity);
}
// führt vorherige Funktionen aus
void drive() {
  updateControlData();
  control_servo();
  control_DC();
}

#endif
```

## variables.h

```c++
//Inkludieren der erforderlichen Bibliotheken für die Sensoren 
#ifndef variables_H
#define variables_H
#include <Arduino.h>
#include "CarOrientation.h"

#include "UltrasonicManager.h"
#include "MyServo.h"
#include "MyDC.h"
#include "SerialIO.h"


// Deklarieren der Variablen, Sensoren

// UltrasonicManager für die Arduino Ports?
const uint8_t frontTrigPin = 2;
const uint8_t frontEchoPin = 3;
const uint8_t rightTrigPin = 4;
const uint8_t rightEchoPin = 5;
const uint8_t leftTrigPin = 6;
const uint8_t leftEchoPin = 7;
UltrasonicManager manager(frontTrigPin, frontEchoPin, rightTrigPin, rightEchoPin, leftTrigPin, leftEchoPin);


//ServoMotor Ports
byte servoPin = 9;
MyServo steeringServo(servoPin);

//DC motor Ports
byte DC_PWMpin = 11;
MyDC drivingDC(DC_PWMpin);


//Serial IO
//SerialIO IO;

//CarOrientation
CarOrientation orientation;

//weitere Vars
float controlDataArr[] = { 0, 0 };  //[desired Car Angle, Speed]
float distances[] = { 0, 0, 0 };
float roll;
float referenceAngle = 0;
float rightShift;
bool stop;
bool safeAngle;
float lastShift=10000000;

// Pins reservieren und initialisieren
void initializeHardware() {
  //IO.init();
  steeringServo.init();
  drivingDC.init();
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

```c++
//Inkludieren der erforderlichen Bibliotheken.
#ifndef UltraSonic_h
#define UltraSonic_h
#include <NewPing.h>
#include <RunningMedian.h>


#define MAX_DISTANCE 400  // Maximum distance in centimeters
#define BUFFER_SIZE 1    // Number of measurements to store for median calculation

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

## SeriallO.h

```c++
//Inkludieren der erforderlichen Bibliotheken.
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

```
## MyServo.h

```c++
//Inkludieren der erforderlichen Bibliotheken.
#ifndef MyServo_h
#define MyServo_h

#include <Arduino.h>
#include <Servo.h>
// Vermittlung zwischen Pins des Servos und Arduino, Berechnen der passenden Servospannung durch Servo Bibliothek
class MyServo {
private:
  byte pin;
  byte limitR = 120;  //Winkel muss < 120
  byte limitL = 40;   //Winkel muss >35
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

```

## MyDC.h

```c++
//Inkludieren der erforderlichen Bibliotheken.
#ifndef MyDC_h
#define MyDC_h

#include <Arduino.h>
// Vermittlung zwischen Pins des DC`s, Berechenen  der DC Spannung
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
  //Geschwindigkeit muss zwischen 0 und 100 sein
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

```c++
//Inkludieren der erforderlichen Bibliotheken.
#include <Wire.h>
#include <Adafruit_BNO08x.h>
//Erfassung und Berechnung der Fahrzeugausrichtung durch Daten des BNO08x-Sensors (Gyroskop)
class CarOrientation {
private:
  Adafruit_BNO08x bno;
  sh2_SensorValue_t sensorValue;
  float initialAngle[3] = { 0.0, 0.0, 0.0 };
  float lastAngle[3] = { 0.0, 0.0, 0.0 };
  float totalRotation[3] = { 0.0, 0.0, 0.0 };
  bool initialAngleSet = false;

public:
  //Konstruktor für die CarOrientation-Klasse.
  CarOrientation() {}
  //Initialisiert den BNO08x-Sensor und setzt die entsprechenden Berichte.
  bool init() {
    if (!bno.begin_I2C()) {
      Serial.println("Failed to find BNO08x chip");
      return false;
    }
    setReports();
    delay(100);
    return true;
  }
  // Aktiviert die erforderlichen Berichte für den BNO08x-Sensor
  void setReports() {
    if (!bno.enableReport(SH2_GAME_ROTATION_VECTOR)) {
      Serial.println("Could not enable game vector");
    }
  }
  /*
    float quaternionToYaw(float qw, float qx, float qy, float qz) {
      float yaw = atan2(2.0f * (qy * qz + qw * qx), qw * qw - qx * qx - qy * qy + qz * qz);
      return yaw * (180.0 / M_PI);
    }
    */
  /*
    float quaternionToPitch(float qw, float qx, float qy, float qz) {
      float pitch = asin(2.0f * (qw * qy - qz * qx));
      return pitch * (180.0 / M_PI);
    }
*/
  float quaternionToRoll(float qw, float qx, float qy, float qz) {
    float roll = atan2(2.0f * (qw * qz + qx * qy), qw * qw + qx * qx - qy * qy - qz * qz);
    return roll * (180.0 / M_PI);
  }
  //Aktualisiert die Sensorwerte und berechnet die Fahrzeugausrichtung basierend auf den Quaternionen
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

      //float yaw = quaternionToYaw(qw, qx, qy, qz);
      //float pitch = quaternionToPitch(qw, qx, qy, qz);
      float roll = quaternionToRoll(qw, qx, qy, qz);

      if (!initialAngleSet) {
        //lastAngle[0] = yaw;
        //lastAngle[1] = pitch;
        lastAngle[2] = roll;
        //initialAngle[0] = yaw;
        //initialAngle[1] = pitch;
        initialAngle[2] = roll;

        initialAngleSet = true;
      } else {
        // float deltaYaw = yaw - lastAngle[0];
        // float deltaPitch = pitch - lastAngle[1];
        float deltaRoll = roll - lastAngle[2];
        /*
        if (deltaYaw > 180.0) {
          deltaYaw -= 360.0;
        } else if (deltaYaw < -180.0) {
          deltaYaw += 360.0;
        }

        if (deltaPitch > 180.0) {
          deltaPitch -= 360.0;
        } else if (deltaPitch < -180.0) {
          deltaPitch += 360.0;
        }
*/
        if (deltaRoll > 180.0) {
          deltaRoll -= 360.0;
        } else if (deltaRoll < -180.0) {
          deltaRoll += 360.0;
        }

        //        totalRotation[0] += deltaYaw;
        //        totalRotation[1] += deltaPitch;
        totalRotation[2] += deltaRoll;

        //        lastAngle[0] = yaw;
        //        lastAngle[1] = pitch;
        lastAngle[2] = roll;
      }
    }
  }
  /*
    float getRelativeYaw() {
      update();
      return quaternionToYaw(sensorValue.un.gameRotationVector.real, sensorValue.un.gameRotationVector.i, sensorValue.un.gameRotationVector.j, sensorValue.un.gameRotationVector.k) - initialAngle[0];
    }
*/
  /*
    float getRelativePitch() {
      update();
      return quaternionToPitch(sensorValue.un.gameRotationVector.real, sensorValue.un.gameRotationVector.i, sensorValue.un.gameRotationVector.j, sensorValue.un.gameRotationVector.k) - initialAngle[1];
    }
*/
  //Gibt die relative Rollwinkeländerung des Fahrzeugs zurück.
  //Der relative Rollwinkel ist der Rollwinkel im Vergleich zur Initialposition des Fahrzeugs.
  //Der Wert ist negiert, um den Rollwinkel in der richtigen Ausrichtung zu liefern.
   
  float getRelativeRoll() {
    update();
    return (quaternionToRoll(sensorValue.un.gameRotationVector.real, sensorValue.un.gameRotationVector.i, sensorValue.un.gameRotationVector.j, sensorValue.un.gameRotationVector.k) - initialAngle[2]) * -1;
  }
  /*
  float getRelativeYaw() {
    update();
    return totalRotation[0];
  }

  float getRelativePitch() {
    update();
    return totalRotation[1];
  }
*/
  //Gibt den Gesamtrollwinkel des Fahrzeugs zurück.
  //Der Gesamtrollwinkel ist die Summe aller Rollwinkeländerungen seit der Initialposition des Fahrzeugs.
  //Der Wert ist negiert, um den Rollwinkel in der richtigen Ausrichtung zu liefern.
   
  float getTotalRoll() {
    update();
    return totalRotation[2] * -1;
  }
  //Setzt die Initialwinkel und den Status für die Fahrzeugausrichtung zurück
  //Dies wird normalerweise aufgerufen, wenn das Fahrzeug neu ausgerichtet wird
   
  void reset() {
    initialAngleSet = false;
  }
};

```

