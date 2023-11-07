## Motors (general)
First we have to distinguish between 3 types of motor: Servos, Stepper and DC. AC motors can be excluded because the battery provides direct current.

For our application, we need a very precise motor that can still overcome the forces of the steering while remaining quite accurate and reliable. Servo motors are the main option for this because they typically use closed-loop control systems that can monitor the motor's position and adjust it as needed to maintain precise positioning accuracy. Even though stepper motors are often used in CNC machines or 3D printers due to their good mix of accuracy and strength, they do not come close to the accuracy of a servo.

It was important for the driving motor to be as powerful and fast as possible. Once again, two motors are available: the stepper and the DC motor. The stepper offers the advantage of being able to measure the distance traveled accurately. In contrast, the DC motor does not stop and thus ensures that the car is inert. Its advantage lies in its high speeds. Ultimately, we opted for a DC brushless motor here, as the accuracy of the distance traveled is not important to us, as the composition of the parkour also varies. Thats why we needed high velocities and a powerfull motor.


## Servo motor

We tried to make our robot as small as possible so that our rotation radius is small. This enables us to maneuver faster on curves and to avoid blocks better. For steering we use a servo motor which can rotate $60°$ to the right and to the left. We use Ackermann steering to translate the rotation of the servo motor to the wheels. Even though the Ackermann steering is not optimal, it is sufficient for our purposes. The servo motor has a standstill torque of about $4.3 \frac{kg}{cm}$, which we take off at a distance of about 3cm. This allows the servo to apply a force of about $14N$. Our robot has a mass of no more than $1kg$ and the friction coefficient can be no more than $1$, which makes a friction force of about $10N$. So our robot would be able to overcome this situation.

## DC-Brushless motor
Another important consideration is the DC brushless motor. With the DC motor we have the possibility to drive forward and backward, which does not exclude possible solutions of the challenges. Also, with this drive motor, we have no risk of connecting it incorrectly, as it is independent of this. We made a gear ratio of about 1 to 2 for torque amplification and at the expense of speed.
