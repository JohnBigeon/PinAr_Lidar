# PinAr_Lidar

<p float="left">
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Examples/animation_gif_2D_plot_v01.gif" height="300" />
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Examples/movie_progress.gif" height="300" /> 
</p>


<p float="center">
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/picture_PCB_and_dome.jpg" width="600" />
</p>

## Introduction
Here, we will build/assemble a three-dimensional mapping device from a CCTV dome camera setup and a Time-of-Flight (TOF) Distance Sensor to make 2D/3D cartography.

The first question: *Why “hack” a dome camera ?*

First answer, because a dome security camera is designed to monitor a wide range of angles (+/- 360° and +/- 180°). When you open it, you can find two stepper motors with belts, pulleys, and sprockets. All the mechanical parts can be easily hacked to use them as a base for your 2D positionning during cartography of your environment. Second answer, and maybe the best one, this dome was destined for the trash can. In other words, we decide to give a new life to something that will eventually be destroyed.

## Concept
From a pedagogical perspective, let’s start with a static problem. To make a long story short, a TOF setup calculates the distance to an object using time of flight. The time-of-flight principle is based on the time difference between the emission of a signal and its return to the sensor, after being reflected by an object, as presented below. Practically speaking, when a light is emitted from the source, this light will travel through the air until it hits an obstacle which reflects this light. In this case, the time between the emission and the reception is directly related to the distance.

<p float="center">
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/time_of_flight_principle_lightmode.png#gh-light-mode-only"/>
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/time_of_flight_principle_darkmode.png#gh-dark-mode-only"/>
</p>

Now, aim this source all around you at equally spaced intervals in both the zenithal (theta) and azimuthal (Phi) directions and you can fully determine the distance of objects around you. This is how most of Laser imaging, detection, and ranging (LIDAR) [https://en.wikipedia.org/wiki/Lidar] cartography is done.

From a practical view, the telemetry information is obtained by a Time-of-Flight Distance Sensor (VL5353LXX-V2) and the positioning in the two angular directions sweeping 360 and 180 degrees respectively is achieved by the two stepper motors of the dome system with microstepping driver (Pololu A4988).

## Hardware & Integration
### Dome security camera
<p float="left">
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/picture_dome_step_01.jpg" width="400" />
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/picture_dome_step_03.jpg" width="400" /> 
</p>

### Price
```
         Object           Price (€)
-----------------------   -----
            Arduino MEGA - 18
         Rotation system - 0 
     Power supply 12V 2A - 10
            Pololu A4988 - 3.50
 AZDelivery VL5353LXX-V2 - 9
    Capacitors ( 100 µF) - 1
                  Cables - 1
```
### Motors
<p float="left">
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/stepper_motors_image.jpg" width="400" />
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/documentation_stepper_v02-0.png" width="400" /> 
</p>

### Power supply
Stepper drivers are power converters, you need to do the calculation in terms of power consumption, not current as explained here [https://forum.arduino.cc/t/what-powersupply-do-i-need-to-power-multiple-stepper-motors/868595/9]. 

Using the power law:
```
P = U x I = 5 x 0.4 = 2 W (per motor/per phase)
```
Here, we are using two stepper motors, the power supply needs to be at least:
````
P_tot = 2 motors * 2 phase * 2W (per motor/per phase) = 8 W
````
Considering the fact that the motors will not always run in parallel, the power consumption is acceptable in our case. In other words, in a 2 axis system, not all motors are working at the same time, same load, meaning that the power value can be lower.
Higher voltage supply allows for faster operation of the steppers, 24V is a commonly chosen supply voltage for a reasonable compromise. If you chose 12V supply, 1.5 A would be appropriate.

**Be careful**, connecting or disconnecting a stepper motor while the driver is powered can **damage** or **destroy** the driver.

### Microstepping
<p float="center">
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/microstepping_pololu_A4988.jpg" width="400" />
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/microstepping_schematic.png" width="400" />
</p>
<p float="center">
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/microstepping_wiring_lightmode.png#gh-light-mode-only"/>
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/microstepping_wiring_darkmode.png#gh-dark-mode-only"/>
</p>

The first part is to connect and interface stepper drivers (A4988) to the two stepper motors. These stepper motors are able to provide 400 steps per revolution with a resolution of 0.9°. Based on the nice tutorial found here [https://howtomechatronics.com/tutorials/arduino/how-to-control-stepper-motor-with-a4988-driver-and-arduino/], the use of the A4988 stepper driver gives you higher resolution of stepping.
The resolution of the system can be tune by MS1, MS2 and MS3 pins as follow [https://www.robotshop.com/media/files/pdf/datasheet-1182.pdf]:

| MS1 | MS2 | MS3 | Microstepping |
|     :---:      |     :---:      |     :---:      |     :---:      |
| Low | Low | Low | 1/1 step |
| High | Low | Low | 1/2 step |
| Low | High | Low | 1/4 step |
| Low | Low | High | ??? step |
| High | High | Low | 1/8 step |
| High | High | High | 1/16 step |


As an example, when MS1 and MS2 are connected and not the MS3, the stepping step will be multiply by a factor 8.
 
### Wiring
Concerning the circuit, the 1A and 1B pins will be connected to the first coil of the motor and the 2A and 2B pins to the second. To protect the microstepper driver from voltages spikes, a capacitor (100 µF) is used here. The pin of step and direction of the first stepper driver are connected to the 3 and 4 pins of the Arduino board. For the second stepper driver, they are connected to pins 6 and 5. For both of them, the reset and sleep pins are need to be pulled high (default low are to reset and sleep). Finally, the sensors is directly connected to SCL and SDA pins.
```
A4988 (#1)   Arduino   Power supply
----------   -------   ------------
      VMOT -         - 12V
       GND -         - GND
       VDD - 5V
       GND - GND
       MS1 - DG44
       MS2 - DG46
      STEP - PWM6
       DIR - PWM5
```
```
A4988 (#2)   Arduino   Power supply
----------   -------   ------------
      VMOT -         - 12V
       GND -         - GND
       VDD - 5V
       GND - GND
       MS1 - DG24
       MS2 - DG26
      STEP - PWM3
       DIR - PWM4
```
```
Arduino   VL53LXX-V2 board
-------   -------------
    5V  - VIN
    GND - GND
    SDA - SDA
    SCL - SCL
```

![fritzing](https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Fritzing_files/double_stepper_motor_Arduino.png)

![KiCad](https://github.com/JohnBigeon/PinAr_Lidar/blob/main/KiCad_files/kicad_schematic.png)

![PCB](https://github.com/JohnBigeon/PinAr_Lidar/blob/main/KiCad_files/kicad_pcb.png)

##### Coil resistance
To confirm the wiring and specially 2B/2A/1A/AB, we can check the coil resistance simply by measuring the resistance across any two wires. As visible here, the Blue and Green wires are associated and if you try any other pairs, you should read the famous 'OL' Infinite resistance (open circuit).
<p float="center">
  <img src="https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/stepper_motors_coil.jpg" width="600" />
</p>
 
#### Set max current
To prevent overheating of your motors (and then, their durability), it is necessary to ensure that there are an adequate current limitation on your A4988 boards. All explanations can be found in the video [https://howtomechatronics.com/tutorials/arduino/how-to-control-stepper-motor-with-a4988-driver-and-arduino/].
This stepper driver allows tuning of the maximum current as explained here [https://www.rototron.info/raspberry-pi-stepper-motor-tutorial/] or here [https://lastminuteengineers.com/a4988-stepper-motor-driver-arduino-tutorial/] in order to protect your board of destructive voltage spikes.
My protocol is:
- Set the current limit by measuring the voltage (Vref) on the “ref” pin and the microstep pins disconnected. 
- Take a look at the datasheet for your stepper motor and specially the rated current.
- Measure the voltage (Vref) on the metal trimmer pot itself while you adjust it. 
- Adjust the Vref voltage using the formula:
```
Current Limit = Vref x 2.5
```
In our case, the motor is rated for 400mA, and the reference voltage is fixed to 0.16 V.
```
V_ref = 0.4 / 2.5 =  0.16 V
```
Generally, if the motors are noisy, too hot while stepping or missing steps that means the current limiting is set incorrectly. 

### Reduction gear
This dome use a reduction gear and the big gear has six times the diameter of the small one (giving a 6:1 ratio). In our case, a full rotation (360°) is equal to
````
360 steps = 360 / resolution * gear = 360 / 0.9 * 6 = 2400
````

### First integration
![Integration](https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/picture_PCB_integration.jpg)


### Simple test to control Stepper Motor and spin Direction
This code will also help you to fix the delay during the stepping process.
Arduino code (see 'Rotation_stepper_simple' folder):
```
//==============================================================================
//title              :one_stepper_rotation_v01.ino
//description        :Control one stepper motor with A4988
//                   :Inspired by https://how2electronics.com/control-stepper-motor-with-a4988-driver-arduino/
//author             :John Bigeon  (https://github.com/JohnBigeon/PinAr_Lidar)
//date               :20220206
//version            :for arduino MEGA (tested on 1.8.19 @ubuntu)    
//usage              :
//script_version     :0.9-release
//Output             :Control stepper motor
//notes              :MS1, MS2 and MS3 are LOW
//==============================================================================

const int dirPin = 6;
const int stepPin = 5;
const int stepsPerRevolution = 2400;
 
void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}
void loop()
{
  // Set motor direction clockwise
  digitalWrite(dirPin, HIGH);
 
  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  delay(1000); // Wait a second
  
  // Set motor direction counterclockwise
  digitalWrite(dirPin, LOW);
 
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second
}
```
Make sure after few minutes that the motor temperature is fine and that no steps have been missed during the rotation (i.e.: the full rotation needs to be equal to 360°).


## Software
The code is separated in two different entities: an arduino code which gives you the measured distances at every step of motors and the python code to display a pretty interface in real time.
The arduino has been tested on arduino-1.8.19 for ubuntu and *arduino-XXX* for Windows 10.

### Arduino code
#### Third-party dependencies: Packages and librairies
Use the Library Manager to install the library *VL53L0X by Pololu (version 1.3.0)* [https://github.com/pololu/vl53l0x-arduino] on the  Arduino Software (IDE).

#### First test
Once the arduino code (folder *Arduino_code*) is uploaded to the board, open your Serial Monitor and you should see something like that:
```
Welcome aboard to Air PinAr_Lidar, 3D positioning
Ignition, what is my purpose ?
Test (T) ? Fast 2D (F) ? Precise 3D (P) ? nb: Emergency case (X) 
```
To confirm that you have successfully integrated your board to the hardware component, use the test configuration by hitting the T and Enter characters of your keyboard in the console (No line ending). If your stepper motors perform the 180° movement in theta direction and the 90° in phi direction, WELL DONE ! In other cases, please see the section “Well-known bugs”.

Here, the position of each stepper motors is converted in spherical coordinate system (theta and phi). Every lines sent from the Arduino to serial monitor consists of phi and theta positions and distance.
So, during this test, you may have seen in the monitor, information about angle positions and distance as:
Theta position (°) Phi position (°) Distance (cm)
```
16.0		00.0 		225.0
16.0		00.9 		201.0
...
```
##### Explore different configurations
We designed four modes of configuration for our mapping system:
- The test configuration (T) where motors make a movement of 180 degrees in theta and 90 degrees in phi direction. It’s useful to check if your board and the microstepping is correctly connected.
- A fast cartography (F), where you measure distance from the dome according a 180-degree angle in the theta direction. 
- A full and precise (P for precise) cartography of space, which means 360 degrees over theta and 90 degrees over phi.
- The shutdown mode (X) where two steppers will return to the starting position.
These modes can be directly called by X, F, P or T characters in the baud of arduino.

### Python code
#### Third-party dependencies: Packages and librairies
To provide a visual interface to our cartography, we decide to use a *python interface*. The reason to do that is mainly because *python* is highly extensible [https://en.wikipedia.org/wiki/Zen_of_Python] and commonly used in Science or Engineering.
The python codes will command the configuration needed by sending the character X, T, F or P and read every values in the baud port. One important point to be stressed is the difficulty for matplotlib to display a figure at high speed rate, typically at more than 10 figures per second. In order to display the distance as fast as we can, we decided to use pyqtgraph packages [http://www.pyqtgraph.org/]. 
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
Here, your mapping system is gathering distance at every angles defined by the motor in theta direction and display it in real time. To **shutdown** your measure, hit **Ctrl+C** in your console. That will close the baud and let motors going to initial positions.

### 3D scanning
In your python environment, use the command:
```
python plot_3d_animation_vXX.py
```
Here, your mapping system is gathering distance at every angles defined by the motor in theta direction and phi direction, but save theses informations in a csv file. Once you have finished the cartography or decided to cancel it (by Ctrl + C), the cartography is displayed on a matplotlib window and on a gif in your directory file.

### Well-known bugs
#### Stepper motor is not actually moving correctly.
- Check the connection of your motor.
- Increase the delay between two pulses send to motor for stepping. In Arduino code, modify the line 34:
      int delay_switch = 1000;
- Use *delayMicroseconds()* and not *delay()* between the two pulses

#### Noise removal and readings that are far away from mean value are removed
TODO

#### Monitor return return the value NACK
- Check the LIDAR connection.

#### Problem to upload the arduino code to your board
- Carefully use the good board type & port in the menu of the Arduino IDE.

#### Uploading code to arduino gives me errors
- On fresh ubuntu installation, you may experience minor problems, that can fixed following this [https://askubuntu.com/questions/1056314/uploading-code-to-arduino-gives-me-the-error-avrdude-ser-open-cant-open-d].
    
### Future improvements
* Use enable pin of A4988 to reduce power consumption.
* Use battery as power supply ?
* Use a Bluetooth module to replace the wired connection for transmitting serial data.
