# PfortGT-WRO

## Enleitung
Dies ist das GitHub-repository für das autonom fahrende Auto des **PfortGT** Teams der **Landesschule Pforta** für den **WRO Future Engineers** Wettbewerb.
In diesem repository gehen wir auf unseren Planungs - und Bauprozess für unser Auto ein. Wir beschreiben unsere Probleme und  Ideen, welche wir auf dem Weg hatten und erläutern unser Baupläne und Programme.
@startuml
actor User
User-right->(select template)
User-right->(write uml)
@enduml
## Überblick
Unser Auto wird durch einen Arduino und einen Raspberry Pi gesteuert. Es besteht aus einem fertigen Chassi welches mit individuell 3D-gedruckten Halterungen für die verschiedenen Sensoren und die Kamera ausgestattet ist (siehe ). Die Stromversorgung läuft über einen Akku. Das Sensorenequipment besteht aus drei Ultraschall-Sensoren, einem Gyro-Sensor und einer Pixy Cam2 (siehe [Sensoren](/Roboterauto/Sensoren)). Für das Eröffnungsrennen verarbeitet ein Arduino Code den durch die Ultraschall-Sensoren gemessenen Abstand zu den Wänden, um kleine Lenkanpassugen des Autos zu treffen (siehe [Eröffnungsrennen](/Programme/Eröffnungsrennen)). Beim Hindernisrennen wird neben dem Ultraschall-Sensoren Code auch ein Kamera Script verwendet (siehe [Hindernisrennen](/Programme/Hindernisrennen)). Dieses sucht über die Pixy Cam nach roten und grünen Farbsignaturen und hält diese auf dem Bildschirmabschnitt (rechts oder links), welcher dem jeweiligen Abbiegen des Autos entspricht.
## Aufbau
[Roboterauto](/Roboterauto/)

Das Auto besteht aus 4-Rädern und zwei Motoren und noch vieles mehr. 

