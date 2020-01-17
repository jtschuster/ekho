Processing Mobile Recording Data
--------------------------------

To process the data from the SD card (stored in .sdraw format), connect the SD card to your computer, compile convert.cpp file using ``make`` or ``make all``, and convert .sdraw file to .raw and .ivs formats using following:

``./convert <surface_file_name(output)> <sdraw_file_from_sdcard(input)>``

Use "make clean" if needed.

Following files will be generated after running ``convert``

***.ivs**
    Regressed IV surface in unreadable format that can be used for emulation.

***.raw**
    Raw IV surface in unreadable binary format.