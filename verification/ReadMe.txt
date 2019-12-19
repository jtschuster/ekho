The mse program compares a recorded surface to an emulated surface. This is only for use when a recorder is set up as the load to an emulator. This assumes the recording is longer than the emulation and the emulation begins after the recording has started.

Usage:
    ./mse <emulated .ivs file> <recorded .ivs file> <time emulation begins>

The program will print the mean square error, percent error, and average deviation for each timestamp and average over all timestamps. The program disregards points on the IV curve where the emulated current is under 1e-8.