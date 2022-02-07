# PinAr_Lidar
## Introduction
Here, we will build/assemble a Laser imaging, detection, and ranging (LIDAR) [https://en.wikipedia.org/wiki/Lidar] from a CCTV dome camera setup and a Time-of-Flight Distance Sensor to make 2D/3D cartography.

The first question: *Why “hack” a dome camera to to use that as a base for 3D LIDAR ?*

First answer, because a dome security camera is designed to monitor a wide range of angles (+/- 360° and +/- 180°). When you open it, you can find two stepper motors with belts, pulleys, and sprockets. All the mechanical parts can be easily hacked to use them as a base for your 2D positionning during cartography of your environment. Second answer, and maybe the best one, this dome was destined for the trash can. In other words, we decide to give a new life to something that will eventually be destroyed.

We have developed this LIDAR as a “popular science project” within the Foton laboratory [http://foton.cnrs.fr]. This project is an integral part of “Fete de la science” [https://www.fetedelascience.fr/], a Science Festival organized in France every year. During these days, the concept is simple, open our academic labs to the public.

## Concept
From a pedagogical perspective, let’s start with a static problem. To make a long story short, a LIDAR calculates the distance to an object using time of flight. The time-of-flight principle is based on the time difference between the emission of a signal and its return to the sensor, after being reflected by an object, as presented on figure 1. Practically speaking, when a laser is emitted from the source, this light will travel through the air until it hits an obstacle which reflects this light. In this case, the time between the emission and the reception is directly related to the distance.

![concept](https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/time_of_flight_v01.png)

Now, aim this laser all around you at equally spaced intervals in both the zenithal (theta) and azimuthal ( Phi) directions and you can fully determine the distance of objects around you. This is how most of LIDAR cartography is done.

From a practical view, the telemetry information is obtained by a Time-of-Flight Distance Sensor (VL5353LXX-V2) and the positioning in the two angular directions sweeping 360 and 180 degrees respectively is achieved by the two stepper motors of the dome system with microstepping driver (Pololu A4988).

## Hardware & Integration
XXX

### Power supply
XXX

### Microstepping
The first part is to connect and interface your stepper driver (A4988) to the two stepper motors. These stepper motors are able to provide 200 steps per revolution with a resolution of 1.8 °. Based on the nice tutorial found here [https://howtomechatronics.com/tutorials/arduino/how-to-control-stepper-motor-with-a4988-driver-and-arduino/], the use of the A4988 stepper driver gives you higher resolution of stepping:
Resolution = \Delta_{step} * X, 
where X = 2, 4, 8 or 16. 
The resolution of the system can be tune by MS1, MS2 and MS3 pins as follow [https://www.robotshop.com/media/files/pdf/datasheet-1182.pdf]:

| MS1 | MS2 | MS3 | microstepping |
|     :---:      |     :---:      |     :---:      |     :---:      |
| Low | Low | Low | 1/1 step |
| High | Low | Low | 1/2 step |
| Low | High | Low | 1/4 step |
| Low | Low | High | ??? step |
| High | High | Low | 1/8 step |
| High | High | High | 1/16 step |


As an example, when MS1 and MS2 are connected and not the MS3, the stepping step will be multiply by a factor 8.
 
### Wiring
Concerning the circuit, the 1A and 1B pins will be connected to the first coil of the motor and the 2A and 2B pins to the second as described on the figure XXX. In our case, we don't have any information about the connections to these motors, we used the famous “trial-and-error” approach to connect the 4 pins to the arduino board.
The pin of step and direction of the first stepper driver are connected to the 3 and 4 pins of the Arduino board. For the second stepper driver, they are connected to pins 6 and 7. Finally, the reset and sleep pins are need to be pulled high (default low are to reset and sleep).
In our case, MS1, MS2 and MS3 pins are in High mode, thereby for a motion of 360 degrees with our dome, sending of 200x16 pulses into the Step Pin is required.

![fritzing](https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Fritzing_files/double_stepper_motor_Arduino_v03_bb.png)

![KiCad](https://github.com/JohnBigeon/PinAr_Lidar/blob/main/KiCad_files/20220203_kicad_schematic_screenshot.png)

![PCB](https://github.com/JohnBigeon/PinAr_Lidar/blob/main/KiCad_files/20220203_kicad_pcb.png)

 
#### Set max current
To prevent overheating of your motors (and then, their durability), it is necessary to ensure that there are an adequate current limitation on your A4988 boards. All explanations can be found in the video [https://howtomechatronics.com/tutorials/arduino/how-to-control-stepper-motor-with-a4988-driver-and-arduino/].

## Software
The code is separated in two different entities: an arduino code which gives you the measured distances at every step of motors and the python code to display nice interface in real time.
The arduino has been tested on arduino-1.8.13 with the LIDARLite.h library for ubuntu and arduino-XXX for Windows 10.

### Arduino code
#### Third-party dependencies: Packages and librairies
XXX
XXX
#### First test
Once the arduino code is uploaded to the board, open your Serial Monitor and you should see something like that:
```
Welcome aboard to Air Foton-DOP, 3D positioning
Ignition, what is my purpose ?
Test (T) ? Fast 2D (F) ? Precise 3D (P) ? nb: Emergency case (X) 
```
To confirm that you have successfully integrated your board to the hardware component, use the test configuration by hitting the T and the Enter character of your keyboard in the console (No line ending). If your stepper motors perform the 180° movement in theta direction and the 90° in phi direction, WELL DONE ! In other cases, please see the section “Well-known bugs”.

Here, the position of each stepper motors is converted in spherical coordinate system (theta and phi). Every lines sent from the Arduino to serial monitor consists of phi and theta positions and distance.
So, during this test, you may have seen in the monitor, information about angle positions and distance as:
Theta position (°) Phi position (°) Distance (cm)
```
16.0		00.0 		225.0
16.0		01.8 		225.0
...
```
##### Explore different configurations
We designed four modes of configuration for our LIDAR system:
- The test configuration (T) where motors make a movement of 180 degrees in theta and 90 degrees in phi direction. It’s useful to check if your board and the microstepping is correctly connected.
- A fast cartography (F), where you measure distance from the dome according a 180-degree angle in the theta direction. 
- A full and precise (P for precise) cartography of space, which means 360 degrees over theta and 90 degrees over phi.
- The shutdown mode (X) where two steppers will return to the starting position.
These modes can be directly called by X, F,  P  or T characters in the baud of arduino.

### Python code
#### Third-party dependencies: Packages and librairies
To provide a visual interface to our cartography, we decide to use a python interface. The reason to do that is mainly because is highly extensible [https://en.wikipedia.org/wiki/Zen_of_Python] and commonly used in Science or Engineering.
Our python codes will command the configuration needed by sending the character X, T, F or P and read every values in the baud port. One important point to be stressed is the difficulty for matplotlib to display a figure at high speed rate, typically at more than 10 figures per second. In order to display the distance as fast as we can, we decided to use pyqtgraph packages [http://www.pyqtgraph.org/]. 
To run these scripts, the easiest way is to create a virtual conda environment [https://docs.conda.io/projects/conda/en/latest/user-guide/getting-started.html] with all packages required.
To install theses packages, you have two option, load our environment here and use the command:
```
conda env create -f list_lidar_env.yml
```
or do it manually by:
```
conda create --name lidar_env python=3.7 numpy spyder matplotlib pyqtgraph pyserial tqdm
```
Now, you can simply activate your virtual environment by the command:
```
conda activate lidar_env
```
You also need the package “drawnow”, not available on the conda package manager. To install it, use: 
```
pip install drawnow
```

## Usage
XXX

### 2D scanning
In your python environment, use the command:
```
python plot_2d_radar_vXXX.py
```
Here, your LIDAR is gathering distance at every angles defined by the motor in theta direction and display it in real time. To shutdown your measure, hit Ctrl+C in your console. That will close the baud and let motors going to initial positions.

### 3D scanning
In your python environment, use the command:
```
plot_3d_animation_vXX.py
```
Here, your LIDAR is gathering distance at every angles defined by the motor in theta direction and phi direction, but save theses informations in a csv file. Once you have finished the cartography or decided to cancel it (by Ctrl + C), the cartography is displayed on a matplotlib window and on a gif in your directory file.

### Well-known bugs
Stepper motor is not actually moving correctly.
- Check the connection of your motor.
- Increase the delay between two pulses send to motor for stepping. In Arduino code, modify the line 34:
      int delay_switch = 5;

Noise removal and readings that are far away from mean value are removed.

Monitor return return the value NACK.
- Check the LIDAR connection.

Problem to upload the arduino code to your board
- Carefully use the good board type & port in the menu of the Arduino IDE.
Uploading code to arduino gives me errors
- On fresh ubuntu installation, you may experience minor problems with fresh ubuntu installation, that can fixed following this [https://askubuntu.com/questions/1056314/uploading-code-to-arduino-gives-me-the-error-avrdude-ser-open-cant-open-d].
    
### Future improvements
XXX
