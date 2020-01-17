#!/bin/bash

if [ "$1" = "-h" ] || [ $# -lt 2 ]; then
    echo "Emulates IV surface in real-time"
    echo "Format: ./emulate.sh <emulator_port> <path_to_ivs_file>"
else
    ./emulator/emulate "$1" "$2"                                    
fi
