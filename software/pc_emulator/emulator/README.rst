Emulating IV Surfaces
=====================

This folder contains tool for emulating IV surfaces.

To start emulation, follow

To emulate IV surfaces when connected, follow below steps

#. Connect Ekho emulator board to Teensy 3.6 ``A2 -> VSAMP``, ``GND -> GND``, ``A21 -> DAC``.
#. Flash Teensy 3.6 with ``pc_emulate_revE.ino`` code in ``Teensy_firmware_pc_emulate_revE`` directory, using Arduino-teensy.
#. Make sure emulator is powered by a battery.
#. To start emulating, and run following command
    
``./emulate.sh <emulator_port> <path_to_ivs_file>``

Keep following the output on the terminal

| Note
- Run above command after compiling emulate.cpp. This can be done by compiling it in ``emulator`` directory or in Ekho software directory. 
- Run ``make`` or ``make all`` to compile in any of the above directory of your choice.
- Use ``make clean`` or ``make remake`` if needed.