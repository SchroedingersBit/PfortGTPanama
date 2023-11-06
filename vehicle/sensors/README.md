# Sensors
Here the sensors are described and their properties explained.
## PixyCam
The PixyCam 2 was chosen for the camera. This is a popular choice for robotics projects involving autonomous driving cars for several reasons. First, it provides a reliable and accurate image processing function specifically designed for object detection and tracking. This enables the autonomous vehicle to acquire visual information in real time and make decisions based on the detected objects.
Another advantage of the PixyCam 2 is its ease of use. It has a simple interface and a well-documented API that facilitates integration with different platforms and programming languages. This is very convenient for us, since the library for Arduino is very easy to set up and there are many sources available. In addition, the PixyCam 2 is characterized by its compact size and low power consumption, which makes it ideal for use in autonomous vehicles, where space and energy efficiency are important factors.

The PixyCam2 is connected to the Arduino via a ICSP SPI three ports cable with three ports and communicates through the ICSP connector. To train the PixyCam 2 to the color codes of certain objects, the PixyMon software is used. The color code for an object of the desired color is stored in this program by holding it in front of the camera. After that, further adjustments are made to make the recognition as precise as possible. The PixyCam can detect an enormously high number of objects simultaneously. However, we only need two objects with a total of two different color codes at a time. The PixyCam then automatically sorts the object with the searched color code, which takes up the most frames on the screen, as the first block.


<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/eb3a5cff-a5a3-4a08-8798-f2fcd5b6ecfd" alt="PixyCam" width="400" />
</p>

## Ultrasonic sensors
The use of the HC-SR04 ultrasonic sensors allows us, especially for the opening race, to accurately, easily and quickly detect distances to walls in the vicinity of the vehicle. They emit ultrasonic waves and measure the time it takes for the reflected signal to return. This allows them to accurately determine distances to obstacles or other vehicles. This is crucial to avoid collisions and drive the curves safely, straight and fast.

Another advantage of ultrasonic sensors is their versatility. They can be mounted in different directions to cover a wide field of view and detect obstacles from different angles. This allows us to guarantee comprehensive and reliable environmental awareness for the car.
In addition, ultrasonic sensors operate independently of lighting conditions, making them reliable in a variety of environments, both daylight and dark. They are insensitive to external influences such as glare or reflections, which can occur with other sensors such as cameras. However, we also had to deal with a big problem regarding reflections from slanted walls, since a wall that was only slightly slanted, could affect the distance to it a lot. We tried to fix this as much as possible in our program through error tolerances and sensor data conditions.

<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/676ffe3f-2a61-47e1-96a9-5eed88ad20de" alt="US sensor" width="400" />
   <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/181f1490-14f6-43e2-95a2-e8d1b493da5a" alt="US sensor" width="400" />
</p>

## Gyroscope
To stop the car after three laps, the Adafruit 9-DOF Orientation IMU Fusion Breakout - BNO085 (BNO080) gyroscope was installed, which measures the rotated angles during turns and brings the motor to a stop after 1080°. It also helps with steering correction.

<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/86dd095a-c874-4c95-8dc8-72763162c80c" alt="Gyroscope" width="400" />
</p>


