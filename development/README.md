# Development

First, the chassis was planned. It was important to keep the turning radius as small as possible and therefore the car short, while the steering, the motor and all other hardware components needed to fit on it. So the following first sketch (1:1) was made:





<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGTPanama/assets/109133963/c11ec4bb-ceb6-43a3-9d0d-1883bdc31610" alt="first chassis sketch" width="600" />
</p>


Now the type steering system had to be considered. The standard solution is an Ackermann steering mechanism, which works mechanically pretty well, but is hard to construct. In the following image, our first schematic sketch is seen, which helped us understanding the principles of the Ackermann steering mechanism.




<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGTPanama/assets/109133963/5bf5ad23-10ae-4086-b562-f220c0167ff6" alt="Ackermann Steering sketch" width="600" />
</p>


The Rack and Pinion system is a more intuitive solution, which often does not work as good as the first one. Furthermore the lengths and dimensions of the steering knuckle and the tie-rod are important for preventing that the rear axle could break out. We developed a graphical calculation system using GeoGebra (which can be found in uploaded files/development) for determining fitting parameters. 

At the same time, we received the Funduino set, whose steering system we immediately adopted because it met our requirements. Instead we concentrated on minimizing the car dimensions and with that the turn radius and the chance of hitting obstacles.
We then printed more and more parts and were able to replace most of the parts with ours and also add additional components, such as the camera mount. Our first attempt of holding the camera was this construction:

<p align="center">
<img width="227" alt="Bildschirmfoto 2023-11-06 um 11 56 43" src="https://github.com/SchroedingersBit/PfortGTPanama/assets/109133963/f5576cf1-99cc-4bfd-b237-c8d6ffd0aaea">
</p>
Later, we realised, that a higher camera has higher chances to recognize the closer block of two obstacles and has generally a better overview. And just as predicted, the camera tower was created and performed better than the lower mount.
Still it had to be moved to the rear to ensure a better view. This helped to have a longer correction time and a better evaluation of the obstacle's position. 

This is our first attempt of placing the camera tower:

<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGTPanama/assets/109133963/41e67681-4d87-406a-a938-9dbbe58d593d" alt="front placing camera tower" width="200" />
  
</p>

You can find a picture of the currently camera tower placement in [picture](https://github.com/SchroedingersBit/PfortGTPanama/blob/main/pictures).

(For understanding how each components were designed and built, have a look at [components](https://github.com/SchroedingersBit/PfortGTPanama/tree/main/vehicle/components).

A crucial change is also the exchange of the two gears, so that instead of a large to small transmission, there is a small to large transmission. Therefore, the car drives slower, but has a greater torque and does not stop at low speeds, thus overcoming greater static friction forces / rolling friction forces. This was needed since it often slowed down during steering to a minimum speed risking to stop and not being able to continue its drive. A greater torque and a lower speed also provides more time for calculating the new steering correction.
Another important step for increasing the time and power of computing was replacing the Arduino Uno with the Arduino Mega.

One of the last changes is the removal of the battery box. Even though it was a good variant to keep the battery away from hot pieces and still saved space on the cover plate, it sometimes touched the obstacles because of its protruding design. That's why we decided to not use the battery box, but strap the battery to the camera tower with cable ties, because it's a simple and functional option.



<p align="center">
  <img src="https://github.com/SchroedingersBit/PfortGTPanama/assets/109133963/a877b934-9f80-4ece-ad34-8332e9ccf88f" alt="battery placement" width="200" />
  
</p>


For competition preparation we 3D-printed every component  and ordered our hardware electronic again, so we could substitute broken or loose pieces.
