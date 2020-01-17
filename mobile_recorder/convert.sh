if [ $# -lt 2 ]; then
    echo "Converts the data from .sdraw file stored in SD card after recording in the wild"
    echo "Genrates .raw, .ivs, raw .gnu and .png, and regressed .gnu and .png files"
    echo "Usage: ./convert.sh <sdraw file name> <output file name>"
else 
    ./converter/convert "$2" "$1"                                           # record to .raw data
    ../tools/generate_surface/generate_surface "$2".raw "$2"                # convert .raw to .ivs and .gnu (1 raw and 1 regression)             
    gnuplot "$2".gnu                                                        # generate png for no regression plot
    gnuplot "$2"_raw.gnu                                                    # generage png for regression plot
    open "$2"_render.png "$2"_render_raw.png                                # open plots       
fi