# Open Challenge
In the opening race, the car incorporates ultrasonic and gyroscope sensor data in order to be able to drive in the center between the set-up boundaries. If it is not in the center, e.g. because of a curve or inaccuracies, a new angle - target angle from the different distances to the walls - rightshift is calculated, which should be aimed at. This angle is then slowly corrected to prevent a jerky movement and must not exceed a maximum size, which is defined by the gyro sensor. The gyro sensor also counts the covered curves to stop after three rounds. The ultrasonic sensor value results from the median of 5 measurements and is updated all the time.
## Flowchart for the ultrasonic sensors
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

## Flowchart for curve logic
```mermaid
flowchart TB;
Kurve --> |Betrag linker + rechter USS > 130| F{Front USS};
F --> |misst Wert < 100| G{Gyrosensor};
G --> |Neigung des Autos < 20 Grad|Kurvenart;
Kurvenart --> |rightshift < 0| R{Rechtskurve};
R --> |reference Winkel + 90 |K{Kurve beendet};
Kurvenart --> |rightshift > 0| L{Linkskurve};
L --> |reference Winkel - 90 |K;

```
