PC Recording
================

This directory contains Teensy firmware for recording IV surfaces when connected to a computer and tools for converting the recorded data into useable formats. The converted data is used to emulate and display IV surfaces on the screen.

Directories
-----------

- ``recorder`` Tool for recording sampled data into ``.raw`` format.
- ``Teensy_firmware_pc_record_revF`` Teensy firmware to record IV surfaces when connected with a PC.

Usage
-----

To record IV surfaces in when connected to a PC, you need to follow below steps

#. Connect Ekho record board to Teensy 3.6.
#. Flash Teensy 3.6 with ``mobile_record_revF.ino`` code in ``Teensy_firmware_mobile_record_revF`` directory, using Arduino-teensy. In the code, change desired recording time by changing the value of ``RecordTime`` variable in miliseconds.
#. Make sure the power switch on Ekho record board is either on teensy to power Ekho record board from teensy, or on battery if you choose to (don't forget to connect a battery) .
#. To start recording, and run following command
    
``./record.sh <output file name> <time in ms> <usb port>``

#. Both raw and regressed IV surfaces will be displayed on the screen.

| Note
- Run above command after compiling record.cpp and generate_surface.cpp. This can be done individually by compiling in their respective directories i.e., by running ``make`` or ``make all``. Or both files can be compiled at once by just running ``make`` or ``make all`` in ekho home directory.
- Use ``make clean`` or ``make remake`` if needed.

Following files will be generated after running ``convert.sh``

***.ivs**
    Regressed IV surface in unreadable format that can be used for emulation.

***.raw**
    Raw IV surface in unreadable binary format.

***.gnu**
    GNU plot code for plotting regressed IV surface.

***_raw.gnu**
    GNU plot code for plotting the raw(recorded) IV surface.

***_render.png**
    IV surface made up of regressed IV curves.

***_render_raw.png**
    IV surface made up of raw(recorded) IV curves.
