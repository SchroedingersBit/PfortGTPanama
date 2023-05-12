# Elektronik
## Farbcodierung
rot (5V-Driver, Motoren, Gyro), orange (Motor IN) und gelb (Ultraschallsensoren): Spannungskabel

schwarz: Erdungskabel

grün: Kontrollkabel für ENA, ENB und Servo

gelb: Kontrollkabel Gyro (SCL)

blau: Gyro (SDA)

violett: Ultraschallsensor (Trig)

grau: Ultraschallsenso (Echo)

## Driver
Wir benutzen einen L298N-Driver der zwei Motoren kontrollieren kann. Ebenfalls hat er eine Möglichkeit, 5V auszugeben. Wie im unteren Bild ersichtlich, geben wir dem Driver einen Input über den 12V-Eingang. Über den 5V-Ausgang versorgen wir unsere Sensoren sowie unsere IN1 bzw. IN3 für die Motor Aktivierung und Bestimmung der Spannungsrichtung. Über OUT1 und OUT2 regeln wir unseren DC-Motor und über OUT3 und OUT4 kontrollieren wir unseren Servo-Motor. Ebenfalls gehen Kabel von ENA und ENB in unseren Arduino, um die Spannung zu regulieren. Die Spannung des Servomotors wird konstant auf 6.4V gehalten. Allgemein nutzen wir den MG996R Servomotor und eine 3-9V DC-Motor von Funduino.

![driver](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/011ca2d6-0b49-4f2f-8c29-943fe6d20fa5)

## Arduino
Wir benutzen einen Arduino MEGA 2560 (Rev3) für eine erhöhte Rechenleistung. Zusätzlich zum Arduino benutzen wir einen Arduino Uno Prototype Proto Shield V5, um mehr Pins auf kleinem Platz zur Verfügung zu haben. Wir haben Erdungspins und Spannungspins auf dem weißen Board angeschlossen. Die Pins von 2-7 sind entweder mit Trig oder Echo für den Ultraschallsensor besetzt. Die Pins 11 bzw. 13 sind ENA bzw. ENB und Pin 9 Kontrollpin für den Servomotor. Das 3V orangene Kabel verläuft zum Gyro, weil dieser maximal 3.3V aushält. Der Gyro wird deshalb auch gleich über den Arduino geerdet.

![Arduino](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/d9f0fbc2-7ac8-450e-85b8-eed33ab25070)

## Batterie
Wir benutzen eine URGENEX 7.4V Li-ion Akku für die Versorgung von unserem Roboter. Der Akku liefert 7.4V mit 2000mAh was genügend ist laut unseren Berechnungen. Denn der DC-Motor arbeitet sehr gut mit 7.4V und unsere Stromstärke kann maximal für eine kurze Zeit 2A betragen, wodurch wir eine Stunde arbeiten könnten. Die Erdung ist mit dem Schalter verbunden, welche anschließend mit den Erdungspins verbunden ist.  

![Batterie + Knopf](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/2f4fe213-dc1e-48c4-af92-1f101684ee40)

## Ultraschallsensoren
Der Servomotor ist hinter den Ultraschallsensoren angeschlossen und wir benutzen die HC-SR04 Ultraschallsensoren.

![Servo + Ultraschall](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/8122f87d-3152-4029-9389-b9e9dc073e89)
der 

## Gesamtbild
Die gesamte Darstellung wurde mithilfe von Fritzing erstellt, wo ich auch zusätzlich mehrere Bauteile implementieren müsste, wie der Driver, Gyro bzw. den Proto Shield V5.

![Gesamtbild](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/91a0c85f-1f8b-421b-811c-65d37a91613d)


