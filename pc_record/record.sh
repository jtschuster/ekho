#!/bin/bash

if [ "$1" = "-h" ] || [ $# -lt 3 ]; then
    echo "Runs the recording demo"
    echo "Format: ./record.sh <recording file name> <time in ms> <usb port>"
else
    echo "Making file name" $1
    ./record/record "$3" "$2" "$1"                                  # record the ivs data
    ./Plot\ Surface/plot_surface ./"$1".raw "$1"                    # convert .raw to .ivs and .gnu (regression)
    ./convert_rawData_to_txt_file/convert ./"$1".raw "$1"_raw      # convert .raw to .txt file
    ./gnuplotter/gnu_from_txt ./"$1"_raw.txt "$1"_raw                        # convert the raw .txt to .gnu (no regression)
    gnuplot ./"$1"_raw.gnu                                            # generate png for no regression plot
    gnuplot ./"$1".gnu                                                # generage png for regression plot
    open "$1"_render.png "$1"_raw_render.png 
                                                                      # open plots

fi
