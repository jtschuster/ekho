import numpy as np 
import struct
recorded_file = input("What is the file that was recorded?\n")
emulated_file = input("what is the file that was emulated?\n")
with open(recorded_file, 'rb') as rf:
    with open(emulated_file, 'rb') as ef:
        