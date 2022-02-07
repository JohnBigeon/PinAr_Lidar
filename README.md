# PinAr_Lidar
## Introduction
In this Instructable, we will present how to build/assemble a Laser imaging, detection, and ranging (LIDAR) [https://en.wikipedia.org/wiki/Lidar] from a dome CCTV and a Time-of-Flight Distance Sensor to make cartography.

We have developed this LIDAR as a “popular science project” within the Foton laboratory  [http://foton.cnrs.fr]. This project is an integral part of “Fete de la science” [https://www.fetedelascience.fr/], a Science Festival organized in France every year. During these days, the concept is simple, open our academic labs to the public.

The first question: *Why “hack” a dome camera to to use that as a base for 3D LIDAR ?*

First answer, because a Dome security camera is designed to monitor a wide range of vision. When you open it, you can find two stepper motors with belts, pulleys, and sprockets. All the mechanical parts can be easily hacked to use them as a base for your 2D positionning during cartography of your environment.
Second answer, and maybe the best one, this dome was destined for the trash can. In other words, we decide to give a new life to something that will eventually be destroyed.

## Concept
From a pedagogical perspective, let’s start with a static problem. To make a long story short, a LIDAR calculates the distance to an object using time of flight. The time-of-flight principle is based on the time difference between the emission of a signal and its return to the sensor, after being reflected by an object, as presented on figure 1. Practically speaking, when a laser is emitted from the source, this light will travel through the air until it hits an obstacle which reflects this light. In this case, the time between the emission and the reception is directly related to the distance.

![concept](https://github.com/JohnBigeon/PinAr_Lidar/blob/main/Documentation/time_of_flight_v01.png)

Now, aim this laser all around you at equally spaced intervals in both the zenithal (theta) and azimuthal ( Phi) directions and you can fully determine the distance of objects around you. This is how most of LIDAR cartography is done.

From a practical view, the telemetry information is obtained by a Time-of-Flight Distance Sensor (VL5353LXX-V2) and the positioning in the two angular directions sweeping 360 and 180 degrees respectively is achieved by the two stepper motors of the dome system with microstepping driver (Pololu A4988).

## Hardware & Integration
XXX

### Power supply
XXX

### Wiring
XXX

### Microstepping
 MS1   MS2   MS3
-----------------
 low   low   low   Full step
 high  low   low   Half step
 low   high  low   1/4 step
 high  high  low   1/8 step
 high  high  high  1/16 step
 
#### Set max current
XXX

## Software
XXX

### Third-party dependencies: Packages and librairies
XXX

### Arduino code
XXX

### Python code
XXX

## Usage
XXX

### 2D scanning
XXX

### 3D scanning
XXX
