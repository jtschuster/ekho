1- install GSL (GNU Scientific Library)
2- install Arduino and Teensyduino.
3- ensure the dacamp.dat file in emulate.cpp is the correct map for the emulate board.
    Upload test.ino to the teesny connected to the emulator. Connect A3 -> VS_AMP and make the connections as below. Plug in an SD card, and connect the teensy to power. An LED will light up and turn off when the power can be removed. Use the FORMULA to make the dacmap.
3- connect Ekho emulate board to Teensy 3.6.,  specifically 

 VS_AMP -> A2 (Note: VS_AMP is not the pin labeled VS. It is the Pin next to it on the load side.)
 GND -> GND
 VDAC -> A21

4- connect 12-18 volts battery to Ekho emulate board.
5- place a capacitor between EKho output "load" and your load. I use 2.2 uF. You may need a much smaller capacitor if you use a large resistor. Keep in mind the RC constant and whether it is reasonable for your application.
6- Although the board has a 470 ohm sense resistor, but you might need to remove it and replaced it by a resistor that suit your recorded harvester. Also you have to change the variable "RESISTANCE" value in "emulate.cpp" to reflect the resistance that you are using. You can place a resistor between the P$1 (LOAD_V) and P$2 (AMPOUT) pins to interchange.

How to choose a sense resistor for a certain harvester?

you need to know what the maximum voltage and current your harvester can produce ( by plotting the recorded data).
Suppose that your harvester produces maximum 4.15 voltage and maximum 0.00004 Amp current, so subtract maximum voltage of your harvester form the maximum voltage that Ekho recorder can play which is 10.30 and divide the result by the maximum current of your harvester. (10.3-4.15)/0.00004 = 125K resistor.
 Now, you need to see the minimum current can Ekho emulator produces based on the value of the sense resistor (125K).
first, calculate the minimum voltage can be produced by Ekho emulator = 3.3(maximum voltage of Teensy)* 3.2 (opamp gains of Ekho recorder) /4096 (ADC value of 3.3 voltage) = 0.0025 volts. 

second, divide this minimum voltage by the sense resistor= 0.0025V/125K = 0.00000002 Amp. 
So, the minimum current can be played with using 125K sense resistor is 0.00000002 Amp. If your harvester produce less than that you need to pick another resistor value. 

7- flash Teensy 3.6 with EkhoEmulate.ino code (in "Teensy_firmware_Emulator" folder), using Arduino-teensy.
8-  After compiling emulate.cpp file using make command, and connecting the teensy
    to your computer, open command line and type:

	For MAC user:

 	./emulate /dev/cu.<portName> <surface_file_name.ivs>
 	<portName> type us and press tab for port name auto completion

	For Linux user
 			./emulate /dev/ttyACM0  <surface_file_name.ivs>

NOTE: If connecting the ekho record to the load of the emulator, the emulator and recorder should be connected to different laptops that are not plugged into the charger. In addition, the RC constant is especially important when using the recorder as the load. The load changes fast, so a smaller capacitor may be necessary.