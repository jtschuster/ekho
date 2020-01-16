This folder contains tools for recording while connected to a computer. 


Run 
    make clean
Run record.sh to record an IV surface and convert it to .ivs and .gnu files.

Folders:
    - record: contains the code for recording on a laptop with a realtime plot of the IV curve. Outputs a .raw file. 
    - plot_surface: takes a .raw file from record and creates an IVS file and .gnu files for visualizing the recorded data and ivs curve.
    - convert_IV_to_txt_file: Self explanatory. Takes a .ivs file and makes a file of (timestamp, voltage, current).
    - convert_rawData_to_txt_file: Takes a .raw file and makes a .txt file of (timestamp, voltage, current).
    - teensy-firmware_record_revf_AA: contains the teensy code for recording on a computer. 
