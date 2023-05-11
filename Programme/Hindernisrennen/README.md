# Hindernisrennen
Hier wird unsere Programmstruktur für das Hindernisrennen besprochen. Besonders wird dabei das Kamera Programm eingegangen, welches die Daten derPixyCam verarbeitet. Außerdem wird die Gewichtung zwischen dem Ultraschall- und dem Pixy Cam Programm diskutiert.

```mermaid
graph TB;
A{Hindernis} -->B{PixyCam erkennt Farbcode};
Nein --> Lenkanpassung;
subgraph links
B-->E{grün}-->C1{Farbcode auf linker Screenhälfte};
C1 --> Nein;
Lenkanpassung-->C1;
C1-->J1{Ja};
J1-->R{Fährt links vorbei};
end

subgraph rechts
B-->C{rot}-->C2{Farbcode auf rechter Screenhälfte};
C2 --> Nein; 
Lenkanpassung -->C2;
C2-->J2{Ja};
J2-->L{Fährt rechts vorbei};
end
```

