# Sensoren
Hier werden die Sensoren beschrieben und ihre Eigenschaften erklärt.
## PixyCam
Für die Kamera wure die PixyCam 2 gewählt. Diese ist eine beliebte Wahl für Robotikprojekte mit autonom fahrenden Autos aus mehreren Gründen. Erstens bietet sie eine zuverlässige und präzise Bildverarbeitungsfunktion, die speziell für die Erkennung und Verfolgung von Objekten entwickelt wurde. Dies ermöglicht dem autonomen Fahrzeug, visuelle Informationen in Echtzeit zu erfassen und Entscheidungen basierend auf den erkannten Objekten zu treffen.
Ein weiterer Vorteil der PixyCam 2 ist ihre Benutzerfreundlichkeit. Sie verfügt über eine einfache Schnittstelle und eine gut dokumentierte API, die die Integration in verschiedene Plattformen und Programmiersprachen erleichtert. Dies ist für uns sehr praktisch, da sich die Bibliothek für Arduino sehr leicht einrichten lässt und viele QUellen im Internet dazu zur Verfügung stehen. Darüber hinaus zeichnet sich die PixyCam 2 durch ihre kompakte Größe und geringen Stromverbrauch aus, was sie ideal für den Einsatz in autonomen Fahrzeugen macht, bei denen Platz und Energieeffizienz wichtige Faktoren sind.

Um die PixyCam 2 auf die Farbcodes bestimmter Objekte zu trainieren, wird die PixyMon Software verwendet. Dabei wird für ein Objekt der gewünschten Farbe in diesem Programm der Farbcode gespeichert, indem man es vor die Kamera hält. Danach werden weitere Aapassungen getroffen um die Erkennung möglichst präzise zu gestalten. Die PixyCam kann eine enorm hohe Anzahl an Objekten gleichzeitig detektieren. Allerdings benötigen wir immer nur gleichzeitig zwei Objekte mit insgesamt zwei verschiedenen Farbcodes. Die PixyCam sortiert dann das Objekt mit dem gesuchten Farbcode, welches die meisten Frames auf dem Bilfschirm einnimt, automatisch als ersten Block.


<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/eb3a5cff-a5a3-4a08-8798-f2fcd5b6ecfd" alt="Bild" width="400" />
</p>

## Ultraschall-Sensoren
Die Verwendung von den Ultraschallsensoren  HC-SR04 erlaubt es uns, vorallem für das Eröffnungsrennen, präzise, einfach und schnell Entfernungen zu Wändenn in der Umgebung des Fahrzeugs zu erfassen. Sie senden Ultraschallwellen aus und messen die Zeit, die benötigt wird, um das reflektierte Signal zurückzuerhalten. Dadurch können sie die Abstände zu Hindernissen oder anderen Fahrzeugen genau bestimmen. Dies ist entscheidend, um Kollisionen zu vermeiden und sicher, gerade und schnell die Kurven zu fahren.

Ein weiterer Vorteil von Ultraschallsensoren ist ihre Vielseitigkeit. Sie können in verschiedenen Richtungen montiert werden, um ein breites Sichtfeld abzudecken und Hindernisse aus verschiedenen Blickwinkeln zu erfassen. Dies ermöglicht es uns eine umfassende und zuverlässige Umgebungswahrnehmung für das Auto zu garantieren.
Für das Hindernisrennen erleichtern sie zudem die Unterscheidung zwischen den Blöcken und Problemfälle bei Kurven zu umgehen.
Darüber hinaus arbeiten Ultraschallsensoren unabhängig von Lichtverhältnissen, was sie zuverlässig in verschiedenen Umgebungen, sowohl bei Tageslicht als auch bei Dunkelheit, macht. Sie sind unempfindlich gegenüber äußeren Einflüssen wie Blendung oder Reflexionen, die bei anderen Sensoren wie Kameras auftreten können. Allerdings hatten wir auch mit einem großen Problem zu kämpfen, was die Reflexion an schiefen Wänden betrifft, da eine nur geringfügig geneigte Wand, den Abstand zu dieser sehr stark beeinflussen konnte. Dies haben wir versucht in unserem Programm durch Fehlertoleranzen und Sensordatenbedingungen so weit wie möglich zu beheben.

<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/676ffe3f-2a61-47e1-96a9-5eed88ad20de" alt="Bild" width="400" />
   <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/181f1490-14f6-43e2-95a2-e8d1b493da5a" alt="Bild" width="400" />
</p>

## Gyroskop
Um das Auto nach drei Runden zum Stoppen zu bringen wurde zusätzlich das Gyroskop Adafruit 9-DOF Orientation IMU Fusion Breakout - BNO085 (BNO080) installiert, welches die gedrehten Winkel bei Kurven misst und nach 1080° den Motor zum stoppen bringt. Außerdem hilft es bei der Lenkkorrektur.

<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/86dd095a-c874-4c95-8dc8-72763162c80c" alt="Bild" width="400" />
</p>


