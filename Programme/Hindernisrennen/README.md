# Hindernisrennen
Hier wird unsere Programmstruktur für das Hindernisrennen besprochen. Besonders wird dabei das Kamera Programm eingegangen, welches die Daten derPixyCam verarbeitet. Außerdem wird die Gewichtung zwischen dem Ultraschall- und dem Pixy Cam Programm diskutiert.

```mermaid
graph TB;
A{Hindernis} -->B{PixyCam erkennt Farbcode};
B-->E{grün}-->Code2{Farbcode auf linker Screenhälfte}-->F{links};
B-->C{rot}-->Code{Farbcode auf rechter Screenhälfte}-->D{rechts};

```
