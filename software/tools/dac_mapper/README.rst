Generating DAC Map
=================

This directory contains the Teensy firmware for generating DAC map of op-amps used on the emulator. We notice that DAC map varies from hardware to hardware. Generating your own would help in emulating IV curves accurately.

To generate the DAC map, follow below steps

#. Connect Teensy with emulator ``A2 -> VSAMP``, ``GND -> GND``, ``A21 -> DAC``
#. Flash Teensy 3.6 with ``dac_mapper.ino`` code.
#. Make sure emulator is powered by a power supply/battery and not Teensy.
#. Insert SD card into Teensy.
#. Wait till the LED on the teensy goes off which means a text file has been created.
#. Transfer the text file ``dacmap.txt`` from SD card to your computer so you can use it to create the dacmap for your emulator.
#. For each value X in that text file, use the formula X*(3.3/4095)*3.2. So you ended up with 4096 values (you can write a script to do this calculation or use Excel sheet).  
#. Copy 4096 resulted values to replace the values in "dacmap.dat" file in ``dac_mapper`` directory.
  
