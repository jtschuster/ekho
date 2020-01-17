Recording Real-Time IV Surfaces
===============================

This folder contains tool for recording real-time IV curves into ``.raw`` format. It also displays a real-time plot of IV data points.

To record the data, follow below steps

#. Connect Teensy (soldered with Ekho record board) to the PC
#. Compile record.cpp file using ``make`` or ``make all`` 
#. Start recording by entering below command

``./record <output file name> <time in ms> <usb port>``

Use ``make clean`` or ``make remake`` if needed.

Following file(s) will be generated after running ``record``

***.raw**
    Raw IV surface in unreadable binary format.