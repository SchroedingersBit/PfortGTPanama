#ifndef UltraSonic_h
#define UltraSonic_h
#include <NewPing.h>
#include <RunningMedian.h>


#define MAX_DISTANCE 400  // Maximum distance in centimeters
#define BUFFER_SIZE 5    // Number of measurements to store for median calculation


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