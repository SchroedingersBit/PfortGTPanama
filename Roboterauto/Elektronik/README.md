# Elektronik
## Farbcodierung der Kabel
* rot (5V-Driver, Motoren, Gyro)
* orange (Motor IN)
* gelb (Ultraschallsensoren): Spannungskabel
* schwarz: Erdungskabel
* grün: Kontrollkabel für ENA, ENB und Servo
* gelb: Kontrollkabel Gyro (SCL)
* blau: Gyro (SDA)
* violett: Ultraschallsensor (Trig)
* grau: Ultraschallsenso (Echo)

## Driver
Wir benutzen einen L298N-Driver der zwei Motoren kontrollieren kann.Er hat die Möglichkeit, 5V auszugeben. Wie im unteren Bild ersichtlich, geben wir dem Driver einen Input über den 12V-Eingang. Über den 5V-Ausgang versorgen wir unsere Sensoren sowie unsere IN1 bzw. IN3 für die Motor Aktivierung und Bestimmung der Spannungsrichtung. Über OUT1 und OUT2 regeln wir unseren DC-Motor und über OUT3 und OUT4 kontrollieren wir unseren Servo-Motor. Über eine Verbindung von ENA und ENB zu unserem Arduino wird die Spannung reguliert. Die Spannung des Servomotors wird konstant auf 6.4V gehalten. Wir nutzen den MG996R Servomotor und einen 3-9V DC-Motor vom Funduino Set.

![driver](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/011ca2d6-0b49-4f2f-8c29-943fe6d20fa5)

## Arduino
Wir verwenden den Arduino MEGA 2560 (Rev3) für eine erhöhte Rechenleistung. Zusätzlich zum Arduino benutzen wir einen Arduino Uno Prototype Proto Shield V5, um mehr Pins auf kleinem Platz zur Verfügung zu haben. Wir haben Erdungspins und Spannungspins auf dem weißen Board angeschlossen. Die Pins 2-7 sind entweder mit Trig oder Echo für den Ultraschallsensor besetzt. Die Pins 11 bzw. 13 sind ENA bzw. ENB und Pin 9 ist der Kontrollpin für den Servomotor. Das orangene 3V Kabel versorgt den Gyro Sensor, da dieser maximal 3.3V aushält. Der Gyro Sensor wird daher direkt über den Arduino geerdet.

![Arduino](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/d9f0fbc2-7ac8-450e-85b8-eed33ab25070)

## Batterie
Wir benutzen einen URGENEX 7.4V Li-ion Akku für die Stromversorgung. Der Akku liefert 7.4V mit 2000mAh was nach unseren Berechnungen genügt. Denn der DC-Motor arbeitet mit 7.4V sehr gut und unsere Stromstärke kann maximal für eine kurze Zeit 2A betragen, wodurch wir eine Stunde arbeiten könnten. Die Erdung ist mit dem Schalter verbunden, welche anschließend mit den Erdungspins verbunden ist.  

![Batterie + Knopf](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/2f4fe213-dc1e-48c4-af92-1f101684ee40)

## Ultraschallsensoren
Wir verwenden die HC-SR04 Ultraschallsensoren, welche vor den Servomoter angeschlossen sind.

![Servo + Ultraschall](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/8122f87d-3152-4029-9389-b9e9dc073e89)

## Gesamtbild
Die gesamte Darstellung wurde mithilfe von Fritzing erstellt, wobei zusätzliche Bauteile, wie Driver, Gyro bzw. Proto Shield V5 implementiert werden mussten.

![Gesamtbild](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/91a0c85f-1f8b-421b-811c-65d37a91613d)


