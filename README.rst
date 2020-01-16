Ekho
====

This repository contains the code required to run the ekho.

Tool Requirements
-----------------
- GNU Scientific Library (GSL) 
- OpenGL Utility Kit (GLUT)
- g++ and gcc
- Arduino and Teensyduino
- Make
- GNUplot

Directories
-----------

- pc_recorder: Teensy code and a real-time processing code to record the IV surface (combination of IV curves) while connected to a computer. Includes a real-time plot of the IV curves.
- mobile_recorder: Teensy code to record IV curves when disconnected from a computer (for recording in the wild), and code to convert the raw data from the SD card to useable formats.
- pc_emulator: Computer and teensy code required to emulate a recorded IV surface.

File Formats
------------

**.raw** 
    format is what the recorded data is placed into after initial processing. The data is in binary format and can be converted into a text file using the tools in pc_recorder. It has the following format repeated for a number of timestamps:\
    ``<timestamp, 1 double value> <voltages, 512 long double values> <currents, 512 long double values>``

    The j'th voltage value corresponds to the j'th current value. Together, the 512 (voltage, current) points can be regressed to create an IV curve. 

**.sdraw**
    This format is used by mobile_recorder to store recorded data in a simpler form compared to .raw format. This is done because of the optimizations that are made to avoid floating point operatins on Teensy so to make the processing faster on the mobile platform. It has the following format repeated for a number of timestamps:
    
    ``<timestamp, 4 bytes> <raw values, 10 bytes>``

**.ivs** 
    This format is used by pc_emulator to store processed data after finding a regression on the .raw data points. This regression coverts 500 raw data points per IV curve from .raw file into 65 data points which are simpler and faster to emulate. Several IV curves made up 65 data points together make an IV surface (.ivs). This file is also in unreadable binary format. It has the following format repeated for a number of timestamps:
    ``<timestamp, 1 double value> <currents, 65 long double values>``
**.gnu** is a gnuplot file of the IV surfaces. See gnuplot online documentation for more info.


