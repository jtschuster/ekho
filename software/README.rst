Ekho Software
=============

This directory contains firmwares and tools required to run the Ekho

Sub-Directories
---------------

- ``pc_recorder`` Teensy firmware and a real-time processing code to record IV surfaces while connected to a computer. Includes a real-time plot of the IV curves.
- ``mobile_recorder`` Teensy firmware to record IV surfaces when disconnected from a computer (for recording in the wild), and code to convert the recorded data to useable formats.
- ``pc_emulator`` Computer and teensy code to emulate a recorded IV surface.
- ``tools`` Tools for visualizing and converting binary data to other readable formats.

File Formats
------------

**.raw**
    *Raw* format is what the recorded data is placed into after initial processing. The data is in binary format and can be converted into a text file using the tools in pc_recorder. It has the following format repeated for a number of timestamps:
    
    ``<timestamp, 1 double value> <voltages, 512 long double values> <currents, 512 long double values>``

    The j'th voltage value corresponds to the j'th current value. Together, the 512 (voltage, current) points can be regressed to create an IV curve. 

**.sdraw**
    *SDRaw* format is used by mobile_recorder to store recorded data in the SD card in a simpler form compared to ``.raw`` format. This is done because of the optimizations that are made to avoid floating point operatins on Teensy so to make the processing faster on the mobile platform. It has the following format repeated for a number of timestamps:
    
    ``<timestamp, 4 bytes> <raw values, 10 bytes>``

**.ivs** 
    *IV surface* format is used by pc_emulator and is generated after finding a regression on the raw data points. The regression coverts 512 raw data points per IV curve from ``.raw`` file into 65 data points. This makes emulation process faster. Several 65-point IV curves together make an IV surface (``.ivs``). This file is also in unreadable binary format. It has the following format repeated for a number of timestamps:
    
    ``<timestamp, 1 double value> <currents, 65 long double values>``

**.gnu** 
    This is a gnuplot file of the IV surfaces. See gnuplot online documentation for more info.


Usage
-------

- Run ``make clean`` to remove executable from all sub-directories.
- Run ``make`` or ``make all`` to compile and generate execuatables.
- Run ``remake`` to clean, compile and generate execuatables.
- Follow instructions in ``pc_recorder`` to record IV surfaces by connecting Ekho reorder with the PC.
- Follow instructions in ``mobile_recorder`` to record IV surfaces in the wild.
- Follow instructions in ``pc_emulator`` to emulate IV surface by connecting Ekho emulator with the PC.
- Follow instructions in ``tools`` to convert and visualize the data.



