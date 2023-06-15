# Eröffnungsrennen
Beim Eröffnungsrennen bezieht das Auto Ultraschall - und Gyrosensordaten um in der Mitte zwischen den aufgestellten banden fährt. Befindet es sich nicht mittig, z.B. wegen einer Kurve, wird ein neuer Winkel ausgerechnet, der angestrebt werden soll. Dieser wird dann langsam korrigiert um eine ruckartige bewegung zu verhindern und darf zudem eine maximale größe nicht überschreiten, welcher durch den Gyrosensor definiert wird. Dieser zählt zudem die zurückgelegten Kurven, um nach drei Runden zu stoppen.
## Flowchart 
```mermaid
flowchart TB;
USS-Code --> D{Distanz < 400} --> M{5 Messungen};
subgraph Median
M-->ME{Median berechen} -->N{Neue Messung} -->|Update ältester Messwert| M;
end
ME --> Rightshift;

subgraph Rightshift
LR{Differenz Links Rechts USS} --> |Formelverarbeitung| W{Winkel, welches Auto lenken muss};
end
Rightshift --> Lenkung;

subgraph Lenkung
Berrechnung --> S{Servowinkel der eingestellt wird}
end
Lenkung --> Z{Auto passt Lenkeinstellungen an}
```

