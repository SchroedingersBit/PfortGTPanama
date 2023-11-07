## Signature
We use PixyMon with the help of Color Connected Components (CCC) to train our Pixy2 to see the color of an object. The color code for an object of the desired color is stored by holding it in front of the camera and selecting the object. After that, further adjustments are made to make the recognition as precise as possible. The PixyCam can detect an enormously high number of objects simultaneously. However, we only need four objects with a total of two different color codes at a time. The PixyCam then automatically sorts the object with the searched color code, which takes up the most frames on the screen, as the first block.
It is important to save the green Block in the first signature and the red Block in the second signature, because we 1 for green and 2 for red. 
<p align="center">
<img width="245" alt="Signature label" src="https://github.com/SchroedingersBit/PfortGTPanama/assets/93491768/324edef8-acaf-4093-b558-f4b9a49463af">
</p>

## communication
Since we want to communicate with the arduino, it is important to set the right Data out port and to make the baudrate high for faster communication
<p align="center">
<img width="249" alt="Interface" src="https://github.com/SchroedingersBit/PfortGTPanama/assets/93491768/3ff046c2-43ad-4dc7-8c36-63f9e340e9a2">
</p>

## Built-in functions
The camera itself has a number of corrections that should be activated. Auto exposure correction helps with an overexposed room. If you activate this function, the camera can also see in overexposed rooms.
In addition, flicker avoidance eliminates the flickering of the light source. The auto white balance can be switched on temporarily so that the camera gets used to the new light temperature.
<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGTPanama/assets/93491768/fb618b18-d8c1-40ad-ba50-d86d2964b746" alt="Image 1" width="245">
  <img src="https://github.com/SchroedingersBit/PfortGTPanama/assets/93491768/095c6196-242c-4df9-b49d-508a8fe22752" alt="Image 2" width="245">
</p>

## Tuning
There are many ways to tune the block detection. After some experiments, we came up with these values that need to be improved in the competition
<p align="center">
<img width="247" alt="Expert" src="https://github.com/SchroedingersBit/PfortGTPanama/assets/93491768/2427a50c-285e-4845-9bb8-90bc8893c211">
<img width="245" alt="Signature label" src="https://github.com/SchroedingersBit/PfortGTPanama/assets/93491768/3eb8ec8c-ef86-4f77-af0e-123553063c08">
</p>

## Result
With the right settings and values, we are able to recognize up to 4 blocks very clearly at the same time.
<p align="center">
<img width="1097" alt="Picture" src="https://github.com/SchroedingersBit/PfortGTPanama/assets/93491768/65e390ac-eaa0-42e4-8b80-3257d596aee9">
</p>
For more information on which setting has which effect and how to reduce false-positive and positive-false readings, please visit the official Pixyxcam website, where there is clear documentation on how to use PixyMon and how to set up Pixycam. 
The links can be found in

[sources](https://github.com/SchroedingersBit/PfortGTPanama/tree/main/sources/programming).
