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

```mermaid
flowchart TB;

A{Hindernis}-->B{PixyCam erkennt Farbcode};
B-->C1{Farbcode im rechten Screen?}--> Nein;
B-->C2{Farbcode im linken Screen?}--> Nein;
C1--- Lenken;
Nein --> Lenken;
C2 --- Lenken;
```

```mermaid
flowchart TB;

Hindernis -->B{PixyCam Farbcode} --> Lenkanpassung;

subgraph Lenkanpassung
L{Lenkanpassung} --> C1 & C2;

C2{Farbcode auf rechter Screenhälfte?} -->|Nein|L;

C1{Farbcode auf linker Screenhälfte?}-->|Nein|L;
end
```
