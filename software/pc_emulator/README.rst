PC Emulation
============

This directory contains Teensy firmware and tool for emulating IV surfaces.

Sub-Directories
---------------

- ``emulator`` Tool for emulating recorded IV surfaces from ``.ivs`` files.
- ``Teensy_firmware_pc_emulate_revE`` Teensy firmware to emulate IV surfaces.

Usage
-----

To emulate IV surfaces when connected to a PC, you need to follow below steps

#. Connect Ekho emulator board to Teensy 3.6 ``A2 -> VSAMP``, ``GND -> GND``, ``A21 -> DAC``.
#. ensure the dacamp.dat file in emulate.cpp is the correct map for the emulate board.
#. Flash Teensy 3.6 with ``pc_emulate_revE.ino`` code in ``Teensy_firmware_pc_emulate_revE`` directory, using Arduino-teensy.
#. Make sure emulator is powered by a 12V battery or power supply.
#. Place a capacitor between EKho output "load" and your load. I use 2.2 uF. You may need a much smaller capacitor if you use a large resistor. Keep in mind the RC constant and whether it is reasonable for your application.
#. Although the board has a 470 ohm sense resistor, you might need to remove it and replace it with a resistor that suits your recordeding. You should also change the variable "RESISTANCE" value in "emulate.cpp" to reflect the resistance that you are using. You can place a resistor between the P$1 (LOAD_V) and P$2 (AMPOUT) pins.
    
    How to choose a sense resistor for a certain harvester?
    You need to know what the maximum voltage and current your harvester can produce ( by plotting the recorded data).
    Suppose that your harvester produces maximum 4.15 voltage and maximum 0.00004 Amp current, so subtract maximum voltage of your harvester form the maximum voltage that Ekho recorder can play which is 10.30 and divide the result by the maximum current of your harvester. (10.3-4.15)/0.00004 = 125K resistor.
    Now, you need to see the minimum current can Ekho emulator produces based on the value of the sense resistor (125K). First, calculate the minimum voltage can be produced by Ekho emulator = 3.3(maximum voltage of Teensy)* 3.2 (opamp gains of Ekho recorder) /4096 (ADC value of 3.3 voltage) = 0.0025 volts. Second, divide this minimum voltage by the sense resistor= 0.0025V/125K = 0.00000002 Amp. 
    So, the minimum current can be played with using 125K sense resistor is 0.00000002 Amp. If your harvester produce less than that you need to pick another resistor value. 
#. To start emulating, and run following command
    
``./emulate.sh <emulator_port> <path_to_ivs_file>``

Keep following the output on the terminal

| Note
- Run above command after compiling emulate.cpp. This can be done by compiling it in ``emulator`` directory or in Ekho software directory. 
- The emulation program has a one second delay before beginning emulation.
- Run ``make`` or ``make all`` to compile in any of the above directory of your choice.
- Use ``make clean`` or ``make remake`` if needed.
