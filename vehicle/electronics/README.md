# Electronics
## Cable color coding
**Voltage cable:**
* 🔴 red (5V driver, motors, gyro)
* 🟠 orange (motor IN)
* 🟡 yellow (ultrasonic sensors)

**Control cable:**
* 🟢 green: control cable for ENA, ENB and servo
* 🟡 yellow: Control cable gyro (SCL)
<br/>

* 🔵 blue: Gyro (SDA)
* 🟣 purple: ultrasonic sensor (Trig)
* 🔘 gray: ultrasonic senso (Echo)
* ⚫️ black: ground cable

## Driver
We use a L298N driver that can control two motors and has the ability to output 5V. As you can see in the picture below, we give the driver an input via the 12V input. Via the 5V output we supply our sensors as well as our IN1 or IN3 for motor activation and determination of the voltage direction. Via OUT1 and OUT2 we control our DC motor and via OUT3 and OUT4 we control our servo motor. Via a connection of ENA and ENB to our Arduino we regulate the voltage. The voltage of the servo motor is kept constant at 6.4V. We use the MG996R servo motor and a 3-9V DC motor from the Funduino set.

![driver](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/011ca2d6-0b49-4f2f-8c29-943fe6d20fa5)

## Arduino
We use the Arduino MEGA 2560 (Rev3) for increased computing power. In addition to the Arduino, we use an Arduino Uno Prototype Proto Shield V5 to have more pins available in a small space. We connected ground pins and voltage pins on the white board. Pins 2-7 are either trig or echo for the ultrasonic sensor. Pins 11 and 13 are ENA and ENB respectively and pin 9 is the control pin for the servo motor. The orange 3V cable supplies the gyro sensor, since it can withstand a maximum of 3.3V. The gyro sensor is therefore grounded directly via the Arduino.

![Arduino](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/d9f0fbc2-7ac8-450e-85b8-eed33ab25070)

## Battery
We use a URGENEX 7.4V Li-ion battery for the power supply. The battery provides 7.4V with 2000mAh which is enough according to our calculations. Because the DC motor works very well with 7.4V and our current can be maximum 2A for a short time, which would allow us to work for an hour. The ground is connected to the switch, which is then connected to the ground pins.  

![Batterie + Knopf](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/2f4fe213-dc1e-48c4-af92-1f101684ee40)

## Ultrasonic sensors
We use the HC-SR04 ultrasonic sensors, which are connected in front of the servomotors.

![Servo + Ultraschall](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/8122f87d-3152-4029-9389-b9e9dc073e89)

## Overall picture
The entire representation was created using Fritzing, whereby additional components such as Driver, Gyro and Proto Shield V5 had to be implemented.
When building the circuit, we made sure to hide the cables as much as possible. This way our car looks nicer and the cables do not hinder the camera view.

![Gesamtbild](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/91a0c85f-1f8b-421b-811c-65d37a91613d)

## Charging the battery
We use the HTRC Lipo charger to charge our batteries. For this we have created our own charging program that charges each cell of the battery to a voltage of 3.7V with a current of 1A. The program can be accessed by pressing the Start button.


