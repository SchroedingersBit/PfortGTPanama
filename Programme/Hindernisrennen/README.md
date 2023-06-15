# Hindernisrennen

## Flowchart 
```mermaid
flowchart TB;
Hindernis --> P{PixyCam Farbcode} --> R{rot 2 rechts} & G{grün 1 links};
R -->|links und front USS ausschalten| RU{Rechts USS Mittelabstand zur Wand};
G -->|rechts und front USS ausschalten| LU{Links USS Mittelabstand zur Wand};
RU & LU --x |keine Farbcode mehr erkannt und kleiner Front USS | B{Beendet Hinderniscode};
```
Beim Hindernisrennen nutzen wir die Eigenschaft der Pixy Cam, verschiedene Farbblöcke, für en Wettbewerb grün und rot, zu erkennen. Solange die PixyCam keine Farbblöcke registriert, ist die Variabel color=0 und das Hindernisrennenprogramm mit den Ultraschallsensoren läuft normal weiter ab. Sobald ein Farbblock erkannt wird, wird d color für grün auf 1 und für rot auf 2 gesetzt.
