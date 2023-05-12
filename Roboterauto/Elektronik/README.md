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
Wir benutzen einen L298N-Driver der zwei Motoren kontrollieren kann. Ebenfalls hat er eine Möglichkeit, 5V auszugeben. Wie im unteren Bild ersichtlich, geben wir den Driver einen Input über den 12V-Eingang. Über den 5V-Ausgang versorgen wir unsere Sensoren sowie unsere IN1 bzw. IN3 für die Motor Aktivierung und Bestimmung der Spannungsrichtung. Über OUT1 und OUT2 kontrollieren wir unseren DC-Motor und über OUT3 und OUT4 kontrollieren wir unseren Servo-Motor. Ebenfalls gehen Kabel von ENA und ENB in unseren Arduino hinein, um die Spannung zu regulieren. Die Spannung des Servomotors wird konstant auf 6.4V gehalten.  
![driver](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/011ca2d6-0b49-4f2f-8c29-943fe6d20fa5)

## Arduino
Wir benutzen einen Arduino MEGA 2560 (Rev3) für eine erhöhte Rechenleistung. Zusätzlich zum Arduino benutzen wir einen Arduino Uno Prototype Proto Shield V5, um mehr Pins auf einen kleinen Platz zu haben. Wir haben Erdungspins und Spannungspins auf dem weißen board angeschlossen. Die Pins von 2-7 sind mit entweder mit Trig oder Echo für das Ultraschallsensor besetzt. Die Pins 11 bzw. 13 sind ENA bzw. ENB und Pin 9 Kontrollpin für den Servomotor. Das 3V orangene Kabel geht zum Gyro, weil es maximal 3.3V aushält. Das Gyro wird deshalb auch gleich über den arduino geerdet.
![Arduino](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/1ac03311-9406-42e7-9743-2a5356507fdc)

## Batterie

![Batterie + Knopf](https://github.com/SchroedingersBit/PfortGT-WRO/assets/93491768/e1a1a315-4fa6-4dfe-9000-c42643ecce16)



