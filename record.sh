#!/bin/bash

if [ "$1" = "-h" ]; then
    echo "Runs the recording demo"
    echo "Format: ./record.sh <recording file name> <time in ms> <usb port>"
else
    echo "Making file name" $1
    ./record/record "$3" "$2" "$1"
    ./Plot\ Surface/plot_surface ./"$1".raw "$1"_from_raw
    ./convert_IV_to_txt_file/convert ./"$1"_from_raw.ivs
    mv ./IVdata.txt ./IVdata_from_raw.txt
    ./convert_IV_to_txt_file/convert ./"$1".ivs
    # mv ./convert_IV_to_txt_file/IVdata.txt ./IVdata.txt
    # mv ./convert_IV_to_txt_file/IVdata_from_raw.txt ./IVdata_from_raw.txt
    open IVdata_from_raw.txt
    open IVdata.txt
fi
