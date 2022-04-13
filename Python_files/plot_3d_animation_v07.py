#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
###############################################
##title              :one_stepper_rotation_v01.ino
##description        :Control stepper motors with A4988 and TOF with VL5353LXX-V2
##author             :John Bigeon  (https://github.com/JohnBigeon/PinAr_Lidar)
##date               :20220406
##version            :for arduino MEGA (tested on 1.8.19 @ubuntu)    
##usage              :
##script_version     :0.9-release
##Output             :2D scanning
##notes              :
###############################################
"""

###############################################
# Import libraries
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import axes3d
import os, sys
import numpy as np
import serial
import time
from numpy import genfromtxt
from tqdm import tqdm #progress bar
import datetime
from animation_module import *

###############################################
# Timing
date_time = datetime.datetime.now().strftime("%Y-%m-%d")

###############################################
# OS used
if os.name == 'posix':
    portName = '/dev/ttyACM0'     # replace this port name by yours!
else :
    portName = 'COM4'         # Windows port
    
###############################################
# Create object serial port
baudrate = 9600
ser = serial.Serial(portName, baudrate, timeout=None)
ser.flushInput()

###############################################
f = open('output.csv', 'w+')
 
###############################################
# Realtime data plot. Each time this function is called, the data display is updated
def update():    
    decoded_bytes =ser.readline().decode('utf-8')[:-1]
    print(decoded_bytes)
    
    # break the script at the end of the loop
    if (decoded_bytes.startswith('Job done')):
        print("finish, lets coming back")
        sys.exit()
    
    with open("output.csv","a") as f:
        if decoded_bytes:     # If it isn't a blank line
            f.write(decoded_bytes)

 ###############################################           
### MAIN PROGRAM #####    
time.sleep(2)   #takes some time to initiate the port
ser.write(b'P')
time.sleep(2)
ser.flushInput()     #flush your input buffer

###############################################
# this is a brutal infinite loop calling your realtime data plot
while True: 
    try: 
        update()
    except:
        #print("Error loading or Job done")
        ser.write(b'X') # emergency case of Arduino
        ser.close() 
                # Let's the LIDAR coming at the start position
        for i in tqdm(range(10)):
            time.sleep(1)
            pass
        break

f.close()
 
###############################################
# Load data
pos_3d = genfromtxt('output.csv', delimiter=",", skip_header=6, skip_footer=6)

# skip missing value
pos_3d = pos_3d[~np.isnan(pos_3d).any(axis=1)]

# Sample to reduce time 
# pos_3d = pos_3d[::3]

###############################################
theta = (np.pi / 180) * pos_3d[:,0]
phi   = (np.pi / 180) * pos_3d[:,1]
THETA, PHI = np.meshgrid(theta, phi)
R = pos_3d[:,2]
X = R * np.sin(PHI) * np.cos(THETA)
Y = R * np.sin(PHI) * np.sin(THETA)
Z = R * np.cos(PHI)

###############################################
# Show me what you got
fig = plt.figure()
ax = fig.add_subplot(1,1,1, projection='3d')
surface = ax.plot_surface(X, Y, Z, cmap='viridis', antialiased=False)

plt.axis('off') # remove axes for visual appeal
angles = np.linspace(0,360,21)[:-1] # Take 20 angles between 0 and 360
# create an animated gif (20ms between frames)
rotanimate(ax, angles,'movie.gif',delay=20) 
plt.axis('on')
plt.colorbar(surface)

plt.savefig("output_3Dmode.pdf", dpi=None, facecolor='w', edgecolor='w', orientation='portrait', papertype=None, format=None, transparent=False, bbox_inches='tight', pad_inches=0.1, metadata=None)        
plt.pause(0.1)
plt.show()
