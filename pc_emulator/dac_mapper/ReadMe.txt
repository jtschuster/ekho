Upload dac_mapper.ino to the emulate teensy
Connect the teensy to the emulator board the same as in emulation:

Teensy -> Emulator
A2 -> VSAMP
GND -> GND
A21 -> DAC

Power the teensy with an SD card inserted. This gives you a .txt file with 0-4095 on the left and the received voltage back on the right. 
In a spreadsheet, you can multiply the values on the right by 4095/3.3 and ensure the resulting values are close to the ones on the right.
The, multiply the original values on the right by (3.3/4095)*3.2 and copy the resulting column of values into a "dacmap.dat" into the dacmap folder within the emulate directory.

