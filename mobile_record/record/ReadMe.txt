1-  install GSL (GNU Scientific Library)
2- install Arduino and Teensyduino.
3- install SD card library for Arduino and Teensy 3.x, LC ADC library https://github.com/pedvide/ADC 
4- connect Ekho record board to Teensy 3.6.
5- flash Teensy 3.6 with record_reF.ino code in "Teensy-firmware_record_revF-AA folder", using Arduino-teensy.(in the code change the desired recording time by changing the value of "RecordTime" variable in milisecond.
6- make sure the power switch (on Echo record board ) is on teensy to power Ekho record board from teensy, or on battery if you choose to (don't forget to connect a battery) 
7-  insert SD card into teensy and notice the led will be on while IV curves are recording and it will turn off when the record is completed. Note each curve has 500 points where each point stored in 10 bytes. So, make sure that you have enough space on the SD card. (Each curve needs 5000 bytes)
8- to process the data from the SD card, plug the SD to your computer, and after compiling record.cpp file using make command:



 	./record <surface_file_name> <rawData_file_from_SDcard>
 	
two files will be generated:

1- *.ivs
this file has IV curves that you can use to emulate.

2- *.raw
this file has IV curves raw data pairs of (voltage, current), and you can convert to to text by using convert code in "convert_rawData_to_txt_file" folder.
