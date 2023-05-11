# Hindernisrennen
Hier wird unsere Programmstruktur für das Hindernisrennen besprochen. Besonders wird dabei das Kamera Programm eingegangen, welches die Daten derPixyCam verarbeitet. Außerdem wird die Gewichtung zwischen dem Ultraschall- und dem Pixy Cam Programm diskutiert.

```mermaid
flowchart TB;

A{Hindernis} -->B{PixyCam erkennt Farbcode};
B-->C{rot} & E{grün};
Lenkanpassung --- Nein;
Lenkanpassung --> C2 & C1;

subgraph Rechts
C-->C2{Farbcode auf rechter Screenhälfte};
end
C1 & C2 --> Nein;
subgraph Links
E-->C1{Farbcode auf linker Screenhälfte};
end

C1 & C2---Ja;
```

