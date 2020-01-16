if [ $# -lt 2 ]; then
    echo "Converts the .sdraw file from the SD card to .raw, .ivs, a raw .gnu, and a regressed .gnu"
    echo "Usage: ./convert.sh <raw file name> <output file name>"
else 
    ./converter/convert "$2" "$1"                                           # gives "$2".ivs and "$2".raw
    ../pc_recorder/plot_surface/plot_surface "$2".raw "$2"       #gives another "$2".ivs and a "$2".gnu                
    gnuplot "$2".gnu 
    gnuplot "$2"_raw.gnu
    open "$2"_render.png "$2"_render_raw.png
fi