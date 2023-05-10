# Hindernisrennen
Hier wird unsere Programmstruktur für das Hindernisrennen besprochen. Besonders wird dabei das Kamera Programm eingegangen, welches die Daten derPixyCam verarbeitet. Außerdem wird die Gewichtung zwischen dem Ultraschall- und dem Pixy Cam Programm diskutiert.

```mermaid
graph TB;
A{Hindernis} -->B{PixyCam erkennt Farbcode};
B-->E{grün}-->C1{Farbcode auf linker Screenhälfte};
B-->C{rot}-->C2{Farbcode auf rechter Screenhälfte};
C1-->J1{Ja};
C2 --> Nein; C1 --> Nein;
C2-->J2{Ja};
Nein --> Lenkanpassung;
Lenkanpassung -->C2;
Lenkanpassung-->C1;
J1-->R{Fährt links vorbei};
J2-->L{Fährt rechts vorbei};
```

```mermaid
graph TB;
A((Hindernis)) -->|PixyCam erkennt Farbcode|B((Farbcode));
B-->|grün|C1((Screenhälfte links));
B-->|rot|C2((Screenhälfte rechts));
C1-->|Ja|J1(Fährt links vorbei);
C1-->|Nein|L((Lenkanpassung));
C2-->|Ja|J2(Fährt rechts vorbei);
C2-->|Nein|L;
L-->|rechts|C1;
L-->|links|C2;
```
