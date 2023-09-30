# Bauteile
Hier wird der Aufbau des Autos und die 3D gedruckten Halterungen besprochen

## Chassis
Zuerst wurde mit der Planung des Chassis begonnen (siehe Entwicklung). Das momentane Chassis besteht aus einer großen 3D-gedruckten Grundplatte, an welcher die Lenkung, der Motor mit dem Getriebe, die Achsen und die restliche Hardware befestigt sind. Die wichtigsten Ansprüche an die Grundplatte sind, dieses im Vergleich zur vorherigen Grundplatte zu verkürzen (Δl= -5cm) und genug Befestigungsmöglichkeiten für die restliche Hardware bereit zu stellen. Dabei kam das folgende Design zustande (es ist nur die Hälfte zu sehen, weil es dann später gespiegelt wurde, sodass eine vollständige Platte entsteht).
<img width="1040" alt="Bildschirmfoto 2023-05-31 um 15 49 12" src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/39ea1220-8d36-4189-aabf-6ac741cf9721">

Gedruckt und verschraubt mit den ersten weiteren Bauteilen sieht es so aus:


![IMG_4836](https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/75c43213-0515-4701-9de5-d3b49cc06784)

Auf diese Grundplatte wird die Deckplatte gesetzt, auf welcher sich der Arduino, der Kameraturm und der Regler befindet. Dabei sind besonders die Löcher in der Mitte zu betrachten, welche das Kabelmanagement erleichtern:

![IMG_4838](https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/f46aae51-b388-4639-8cf1-9b15b68bc356)


## Sensorengrundplatte
Die Ultraschallsensoren sollten möglichst weit vorne am Roboter befestigt werden, um möglichst früh auf die kommenden Hindernisse oder Bahnänderungen reagieren zu können. Um dieses dementsprechend zu befestigen, wurde eine Sensorengrundplatte designed, welche den Anforderungen gerecht wird:

<p align="center">
  <img src="Bild1.png" alt="Bild 1" width="600" />
</p>

Die Löcher sind so angeordnet, dass drei Sensorenhalterungen mit je zwei Löchern, vier Löcher für den Kameraturm und vier für den Anbau an das Chassis zur Verfügung stehen. 
Im Folgenden ist noch ein Bild des Bauteils am Roboter zu sehen:

<p align="center">
  <img src="Bild2.png" alt="Bild 2" width="500" />
  <img src="Bild3.png" alt="Bild 3" width="300" />
</p>

  
Der Kameraturm wurde später noch weiter nach hinten auf das Auto gebaut. -> siehe Entwicklung und Kameraturm.

# Ultraschall-Sensorenhalterung
Die Sensorenhalterung musste die Ultraschall-Sensoren statisch und gegenüber bzw. rechtwinklig zueinander befestigen, um vertrauenswürdige, reproduzierbar signifikante Messwerte zu erhalten. Daher ist die Genauigkeit der Befestigung entscheidend. Zwei große Löcher dienen als Halterung für den Sender und Empfänger des Ultraschall-Sensors, die vier Löcher drum herum dienen der Befestigung des Sensors an der Halterung.

<p align="center">
  <img src="Bild4.png" alt="Bild 4" width="400" />
</p>

Durch diese werden kleine Pins oder Drähte geschoben, welche auf der anderen Seite eingeschmolzen oder umgebogen werden können. Zwischen Halterung und Sensor werden noch die Abstandshalter eingebaut, um eine möglichst feste Verbindung zu schaffen, damit kein Spielraum entsteht und die Sensoren unnötig wackeln. Über die zwei Löcher auf der Bodenseite ist die Halterung an der Grundplatte verschraubt (siehe Sensorengrundplatte).
  
<p align="center">
  <img src="Bild5.png" alt="Bild 5" width="300" />
  <img src="Bild6.png" alt="Bild 6" width="300" />
</p>

  
## Abstandshalter
Die Abstandshalter werden benötigt, um verschiedene Kontakte zu isolieren bzw. eine Erdung zu verhindern und um Abstände zwischen verschiedenen Bauteilen auf eine länge zu beschränken. Die Template, welche gedruckt wurde, enthält Abstandshalter mit einer Höhe von 2.5mm und 5.0 mm, um verschiedene Abstände zu erstellen. Der Innenradius beträgt 3.5mm, sodass die Schrauben, welche hauptsächlich verwendet wurden, genau so passen, dass Widerstand besteht, man die schrauben aber durchschauen kann. Die Abstandshalter sind also auch als Muttern verwendbar.

<p align="center">
  <img src="Bild7.png" alt="Bild 7" width="300" />
</p>
<p align="center">
  <img src="Bild8.png" alt="Bild 8" width="300" />
  <img src="Bild9.png" alt="Bild 9" width="300" />
</p>

## Befestigungspins

Die Befestigungspins bestehen aus kleinen Plastik-Stäben und einem Kopf. Die Stäbe schiebt man durch Befestigungslöcher, bis der Kopf auf Anschlag an der Befestigung befindet.
Danach kann man das andere Ende kurz erwärmen und dann umbiegen. So hat man eine statische, präzise Lösung, um die Sensoren an den Halterungen zu befestigen. Für Testfahrten wurden jedoch noch Kabel statt den Pins benutzt, weil diese nicht so schnell kaputt gehen. Der Nachteil von Kabeln bzw Drähten besteht darin, dass man diese nicht so eng befestigen kann, weshalb mehr Spielraum entsteht.

<p align="center">
  <img src="Bild10.png" alt="Bild 10" width="500" />
  <img src="Bild11.png" alt="Bild 11" width="300" />
</p>

## Kameraturm

Die Kamera sollte sich möglichst hoch auf dem Auto befinden (siehe [Kameramontage Future Engineers Docs](https://github.com/World-Robot-Olympiad-Association/future-engineers-gs/blob/main/docs/p04-cv.md)), des Weiteren möglichst weit hinten, um eine große Übersicht zu gewährleiste, Hindernisse möglichst lange detektieren und Hindernisse hintereinander in Bezug auf die Nähe unterscheiden zu können. Daher hat sich ein Kameraturm entwickelt, auf welchen später die Kamera mit den Kamera-Winkelhaltern geschraubt wird:

<p align="center">
  <img src="Bild12.png" alt="Bild 12" width="400" />
  <img src="Bild13.png" alt="Bild 13" width="300" />
</p>

Die Dreiecke in dem Turm wurden eingefügt, um Material zu sparen und möglicherweise in der Zukunft jegliche Bauteile grob zu befestigen.

## Kamerawinkelhalter

Der Winkelhalter befestigt die Halterung der Kamera an dem Kameraturm. Benötigt wurde dieser, weil die mit der Kamera gelieferten Winkel ein Gewinde hatten und dadurch nicht mit unseren Schrauben kompatibel waren. Da aber die Kamera kippbar sein soll, was mit dem gelieferten 	Set-Up nicht möglich ist, hilft der gedruckte Winkelhalter, um die Kamera auf ein Podest mit den Abstandhaltern zu bringen und diese drehbar zu machen.

<p align="center">
  <img src="Bild14.png" alt="Bild 14" width="500" />
  <img src="Bild15.png" alt="Bild 15" width="300" />
</p>

## Zahnradschutz

Der Zahnradschutz ist ein Bauteil, welches verhindert, dass jegliche Kabel in die Nähe von den Zahnrädern kommen:

<p align="center">
  <img src="Bild16.png" alt="Bild 16" width="500" />
  <img src="Bild17.png" alt="Bild 17" width="300" />
</p>

## Getriebe
Umstellung zu klein auf Groß.
<p align="center">
  <img src="g.jpg" alt="Getriebe" width="500" />
 
</p>

## Akkuhalterung

Die eleganteste Lösung schien es zu sein, den Akku hinten am Auto zu befestigen. Dafür haben wir eine Akkubox entwurfen, in welche man den Akku legen kann. Er besteht aus zwei Teilen (siehe 3D-Modelle), dem A und B Teil, welche als Einzelteile gedruckt und dann verklebt werden. Dadurch erhält man einen genaueren Druck, weil dadurch keine Druckstützen benötigt wurden. Im folgenden sind Bilder von den Einzelnen Designs zu sehen und von dem fertigen Bauteil:

<img width="1003" alt="Bildschirmfoto 2023-06-15 um 20 21 15" src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/a395969a-1b5f-4e7b-ac37-7960cb3b19fd">
<img width="896" alt="Bildschirmfoto 2023-06-15 um 20 21 27" src="https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/5c42064f-466b-4462-a41d-6b9ad68e0b5e">

![IMG_4853](https://github.com/SchroedingersBit/PfortGT-WRO/assets/109133963/61f941a0-7ccd-498a-ae1c-ac3c1dc8f9e0)

