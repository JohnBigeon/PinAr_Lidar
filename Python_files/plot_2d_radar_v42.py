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
from numpy import *
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import serial
import os, sys
import matplotlib
import numpy as np
import time
from drawnow import *
from tqdm import tqdm #progress bar

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
microstepping = 1
step_full_theta = int(200*microstepping/2)
step_theta = int(step_full_theta/2)

### START QtApp #####
#app = QtGui.QApplication([])            # you MUST do this once (initialize things)

###############################################
#   Open a window to display your information
win = pg.GraphicsWindow(title="Signal from serial port") # creates a window
p = win.addPlot(title="Realtime plot")  # creates empty space for the plot in the window
curve = p.plot(pen='g', name='red plot')                        # create an empty "plot" (a curve to plot)
# Add polar grid lines
p.hideAxis('bottom')
p.hideAxis('left')
p.addLine(x=0, pen=0.2)
p.addLine(y=0, pen=0.2)

# Display circle with different diameters
for r in range(2, 300, 100):
    circle = pg.QtGui.QGraphicsEllipseItem(-r, -r, r*2, r*2)
    circle.setPen(pg.mkPen(0.2))
    p.addItem(circle)

windowWidth = 1200                       # width of the window displaying the curve
Xm = linspace(0, 0, windowWidth)          # create array that will contain the relevant time series     
ptr = -windowWidth                      # set first x position

###############################################
#pre-load dummy data
values = []
values = [0] * step_theta

###############################################
def update():
    """
    Realtime data plot. Each time this function is called, the data display is updated
    """
    global curve, ptr, Xm, cnt, sens, values  

    theta = np.linspace(np.pi, 2 * np.pi, step_theta)

###############################################
    # Indicate the counts and the direction
    if (cnt==step_theta):
        sens = -sens
        cnt = 0
    print("Counts & direction: ", cnt, sens)

###############################################
    # Decode the incoming data
    try:
        decoded_bytes = ser.readline().decode('utf-8')[:-1]
    except:
        print(" I missed this one")
        pass        
    print(decoded_bytes)

###############################################
    # Convert to int
    try:
        int_lst = [float(x) for x in decoded_bytes.split(',')]
    except:
        print("not ok")
        int_lst = [0, 0, 0]       
    print ("Distance: ", int_lst[-1])
    Xm[-1-cnt] = int_lst[-1]

###############################################
    if (sens<0):
        values[-cnt] =int_lst[-1]  
    else:
        values[cnt] =int_lst[-1]

###############################################
    # Transform to cartesian and plot
    x = values * np.cos(theta)
    y = values * np.sin(theta)
    curve.setData(x, y)                     # set the curve in polar chart
    cnt=cnt+1
    QtGui.QApplication.processEvents()    # you MUST process the plot now

###############################################
### MAIN PROGRAM #####   
time.sleep(2)   #takes some time to initiate the port
ser.write(b'F')
time.sleep(2)
ser.flushInput()     #flush your input buffer

###############################################
cnt=0
sens=1

if __name__ == "__main__":
    while True: 
        try: 
            update()
        except:
            print("Error loading")
            ser.write(b'X') # emergency case of Arduino
            ser.close() 
            win.close()
            # Let's the LIDAR coming at the start position
            print("finish, lets coming back")
            for i in tqdm(range(10)):
                time.sleep(1)
                pass
            #break
            sys.exit()
    #f.close()
    
    ### END QtApp ####
    pg.QtGui.QApplication.exec_() # you MUST put this at the end
    ##################
