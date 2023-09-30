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
