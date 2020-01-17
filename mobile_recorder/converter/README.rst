Processing Mobile Recording Data
================================

This folder contains tool for converting raw data from in-wild reording into ``.raw`` and ``.ivs`` formats

To process the data from SD card (stored in .sdraw format) follow below steps

#. Connect the SD card to your computer
#. Compile convert.cpp file using ``make`` or ``make all`` 
#. Convert ``.sdraw`` file to ``.raw`` and ``.ivs`` formats using following:

``./convert <surface_file_name(output)> <sdraw_file_from_sdcard(input)>``

Use ``make clean`` if needed.

Following files will be generated after running ``convert``

***.ivs**
    Regressed IV surface in unreadable format that can be used for emulation.

***.raw**
    Raw IV surface in unreadable binary format.