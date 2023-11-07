# Sensors
Here the sensors are described and their properties explained.
## PixyCam
The PixyCam 2 was chosen for the camera. This is a popular choice for robotics projects involving autonomous driving cars for several reasons. First, it provides a reliable and accurate image processing function specifically designed for object detection and tracking. This enables the autonomous vehicle to acquire visual information in real time and make decisions based on the detected objects.
Another advantage of the PixyCam 2 is its ease of use. It has a simple interface and a well-documented API that facilitates integration with different platforms and programming languages. This is very convenient for us, since the library for Arduino is very easy to set up and there are many sources available. In addition, the PixyCam 2 is characterized by its compact size and low power consumption, which makes it ideal for use in autonomous vehicles, where space and energy efficiency are important factors.

The PixyCam2 is connected to the Arduino via a ICSP SPI cable with three ports and communicates through the ICSP connector. To train the PixyCam 2 to the color codes of certain objects, the PixyMon software is used.


<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/eb3a5cff-a5a3-4a08-8798-f2fcd5b6ecfd" alt="PixyCam" width="400" />
</p>

## Ultrasonic sensors
The use of the HC-SR04 ultrasonic sensors allows us, especially for the opening race, detect distances to walls in the vicinity of the vehicle. They emit ultrasonic waves and measure the distance through the time it takes for the reflected signal to return: $d=\lambda \nu *t$.  This is crucial to avoid collisions with walls and drive the curves safely. Another advantage is their versatility. They can be mounted in different directions and angels to cover a wide field of view. This allows us to guarantee comprehensive and reliable environmental awareness for the car. Ultrasonic sensors operate independently of lighting conditions such as glare or reflections, which can occur with other sensors such as cameras. However, we had to deal with a big problem regarding reflections of the ultrasonic waves from slanted walls. Only a slightly slanted wall which often occurs in the race areas, could affect the distance to it a lot. We tried to fix this as much as possible in our program through error tolerances and sensor data conditions. Nevertheless, we can not trust the sensors enough to rely only on their data. We therefore installed a gyro sensor, which operates more accurate. The ultrasonic sensors then only work in the event of deviations that are no greater than 12 degrees or if threshold values to the walls are exceeded and therefore largely serve as a hedge.

<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/676ffe3f-2a61-47e1-96a9-5eed88ad20de" alt="US sensor" width="400" />
   <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/181f1490-14f6-43e2-95a2-e8d1b493da5a" alt="US sensor" width="400" />
</p>

## Gyroscope
To stop the car after three laps and know the spatial orientation, the Adafruit 9-DOF Orientation IMU Fusion Breakout - BNO085 (BNO080) gyroscope was installed. It measures the rotated angles during turns and brings the motor to a stop after 1080° of rotation or 12 curves. To prevent stopping earlier due to rotations while steering, a specific limit must be exceeded to count as a curve. As we described earlier, the gyroscope is important for the steering, as it ensures a straight driving. If there is no block detected or the front distance to a wall is not too small, the steering angle is calculated through the gyroscope data with the goal to do not leave a specific interval of the car orientation.
The BNO085 uses a combination of accelerometer, gyroscope, and magnetometer data to calculate its orientation, and it does so using a sensor fusion algorithm that is more accurate than any of the individual sensors on its own. The high precision of the sensor was the moving reason for choosing it.

<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/86dd095a-c874-4c95-8dc8-72763162c80c" alt="Gyroscope" width="400" />
</p>


