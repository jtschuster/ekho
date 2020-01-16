This repository contains the code required to run the ekho.

Folders:
    - pc_record: Teensy code and a real-time processing code to record the IV curves while connected to a computer. Includes a real-time plot of the IV curves.
    - mobile_record: Teensy code to record IV curves when disconnected from a computer, and code to convert the raw data from the SD card to useable formats.
    - emulate: Computer and teensy code necessary to emulate a recorded IV surface.
    
Requirements:
    - GSL (GNU Scientific Library). 
            This may need to be installed using Homebrew, even on Linux machines. 
            $ brew install gsl
    - OpenGL Utility Kit (GLUT)
            $ brew install freeglut
    - g++ and gcc
    - Arduino and Teensyduino
    - Make
    - GNUplot

File formats
.raw:
    This format is what the data from the recording is placed into. The data is in binary format and not readable by other programs. It can be converted into a text file using the tools in pc_record. It has the following format repeated for a number of timestamps:

    <timestamp, 1 double value> <voltages, 500 long double values> <currents, 500 long double values>

    The j'th voltage value corresponds to the j'th current value. Together, the 500 (voltage, current) points can be regressed to create an IV curve. 

    Note the .raw from pc_record and mobile_record are not the same format. mobile_record must go through a conversion to be the .raw decribed here. This is because of optimizations that are made to avoid floating point operatins on Teensy so to make the processing faster.

.ivs
    This format is used by the emulator. It is created by finding a regression of the .raw data points. It is also in unreadable binary format
    It has the following format repeated for a number of timestamps:

    <timestamp, 1 double value> <currents, 65 long double values>

    The voltages to go with the currents are predetermined. Where do these values come from ?
    10.8 volts is max voltage range can be produced by the emulator board and we have 65 points on x axis (voltage).
	 so 10.8/65 = 0.16615384615L. to get all 65 points value on x axis, 0.16615384615 is added to get next point on the axis.
    long double v=0;
    for (int j=1;j<65;j++)
    {
        v+=0.16615384615L; // 10.8(max voltage range)/65points
    }

.gnu
    gnuplot file of the recorded raw data or curve made from regression. See gnuplot online documentation for more info.
