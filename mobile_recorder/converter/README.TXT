1- Connect Ekho record board to Teensy 3.6.
2- Flash Teensy 3.6 with mobile_record_revF.ino code in "mobile_record_revF" directory, using Arduino-teensy. In the code, change desired recording time by changing the value of "RecordTime" variable in miliseconds.
3- Make sure the power switch (on Ekho record board ) is on teensy to power Ekho record board from teensy, or on battery if you choose to (don't forget to connect a battery) .
4- Insert SD card into teensy and notice the led will be on while IV curves are being recorded and will turn off when the recording is completed. Note each curve needs 5128 bytes. So, make sure that you have enough space on the SD card.
5- To process the data from the SD card (stored in .sdraw format), connect the SD card to your computer, compile convert.cpp file using make command, and convert .sdraw file to .raw and .ivs formats using following:

 	./convert <surface_file_name(output)> <SDRaw_file_from_SDcard(input)>
 	
two files will be generated:

*.ivs
Regressed IV surface (65 points) in unreadable format that can be used for emulation.

*.raw
Raw IV surface (512 points) in unreadable binary format.