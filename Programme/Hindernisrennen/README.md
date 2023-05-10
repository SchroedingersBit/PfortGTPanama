# Hindernisrennen
Hier wird unsere Programmstruktur für das Hindernisrennen besprochen. Besonders wird dabei das Kamera Programm eingegangen, welches die Daten derPixyCam verarbeitet. Außerdem wird die Gewichtung zwischen dem Ultraschall- und dem Pixy Cam Programm diskutiert.

```mermaid
graph TB;
A{Hindernis} -->B{PixyCam erkennt Farbcode};
B-->E{grün}-->C1{Farbcode auf linker Screenhälfte};
C1-->J1{Ja};
C1 --> Nein;
J1-->R{Fährt links vorbei};
Nein --> Lenkanpassung;
Lenkanpassung-->C1;


```

