# Hindernisrennen
Hier wird unsere Programmstruktur für das Hindernisrennen besprochen. Besonders wird dabei das Kamera Programm eingegangen, welches die Daten derPixyCam verarbeitet. Außerdem wird die Gewichtung zwischen dem Ultraschall- und dem Pixy Cam Programm diskutiert.

```mermaid
graph TB;
A{Hindernis} -->B{PixyCam erkennt Farbcode};
B-->E{grün}-->C1{Farbcode auf linker Screenhälfte};
B-->C{rot}-->C2{Farbcode auf rechter Screenhälfte};
C2 --> Nein; C1 --> Nein;
Nein --> Lenken;
C1-->J1{Ja};
C2-->J2{Ja};
```
