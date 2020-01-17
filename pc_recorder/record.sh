#!/bin/bash

if [ "$1" = "-h" ] || [ $# -lt 3 ]; then
    echo "Records IV curves in real-time"
    echo "Genrates .raw, .ivs, raw .gnu and .png, and regressed .gnu and .png files"
    echo "Format: ./record.sh <output file name> <time in ms> <usb port>"
else
    ./recorder/record "$3" "$2" "$1"                                # record .raw data
    ../tools/generate_surface/generate_surface ./"$1".raw "$1"      # convert .raw to .ivs and .gnu (1 raw and 1 regression)
    gnuplot ./"$1"_raw.gnu                                          # generate png for no regression plot
    gnuplot ./"$1".gnu                                              # generage png for regression plot
    open "$1"_render.png "$1"_render_raw.png                        # open plots                                      
fi
