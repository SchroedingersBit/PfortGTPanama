# Hindernisrennen
Hier wird unsere Programmstruktur für das Hindernisrennen besprochen. Besonders wird dabei das Kamera Programm eingegangen, welches die Daten derPixyCam verarbeitet. Außerdem wird die Gewichtung zwischen dem Ultraschall- und dem Pixy Cam Programm diskutiert.

```mermaid
flowchart TB;
B-->C{rot};
A{Hindernis} -->B{PixyCam erkennt Farbcode};
B-->E{grün};
C1 --> Nein;
Nein --> Lenkanpassung;
C2 --> Nein; 
Lenkanpassung-->C1;
Lenkanpassung -->C2;
J-->R{Fährt links vorbei};
J-->L{Fährt rechts vorbei};

subgraph links
E-->C1{Farbcode auf linker Screenhälfte};


C1-->J{Ja};

end

subgraph rechts
C-->C2{Farbcode auf rechter Screenhälfte};


C2-->J;

end
```

