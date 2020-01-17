Mobile Recording
================

To record IV surfaces in the wild, you need to follow below steps

#. Connect Ekho record board to Teensy 3.6.
#. Flash Teensy 3.6 with mobile_record_revF.ino code in "mobile_record_revF" directory, using Arduino-teensy. In the code, change desired recording time by changing the value of "RecordTime" variable in miliseconds.
#. Make sure the power switch (on Ekho record board ) is on teensy to power Ekho record board from teensy, or on battery if you choose to (don't forget to connect a battery) .
#. Insert SD card into teensy and notice the led will be on while IV curves are being recorded and will turn off when the recording is completed. Note each curve needs 5128 bytes. So, make sure that you have enough space on the SD card.
#. To process the data from the SD card (stored in .sdraw format), connect the SD card to your computer, and run following to convert it to .raw, .ivs, a raw .gnu, and a regressed .gnu formats. Last two will then generate .png files that will be displayed on the screen.

        ``./convert <surface_file_name(output)> <SDRaw_file_from_SDcard(input)>``

| Note
- Run above command after compiling convert.cpp and plot_surface.cpp. This can be done individually by compiling in their respective directories i.e., by running "make" or "make all". Or all files can be compiled at once by just running "make" or "make all" in ekho home directory.
- Use "make clean" if needed .

| ** \*.ivs**
Regressed IV surface (65 points) in unreadable format that can be used for emulation.

| *.raw
Raw IV surface (512 points) in unreadable binary format.

| *.gnu
GNU plot code for plotting the 65-point (per IV curve) IV surface

| *_raw.gnu
GNU plot code for plotting the raw (512 points per IV curve) IV surface