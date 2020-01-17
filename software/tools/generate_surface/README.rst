Generate IV Surface
===================

This directory contains the tool for converting ``.raw`` data file into an ``.ivs`` file and two .gnu files for visualizing the recorded and regressed IV surfaces.


To convert the data, follow below steps

#. Compile ``generate_surface.cpp`` file using ``make`` or ``make all``.
#. Convert the data using below command.

``./generate_surface <path_to_raw_file> <output_file_name>``

Use ``make clean`` or ``make remake`` if needed.

Following file(s) will be generated after running ``generate_surface``

***.ivs**
    Raw IV surface in unreadable binary format.

***.gnu**
    GNU plot code for plotting regressed IV surface.

***_raw.gnu**
    GNU plot code for plotting the raw(recorded) IV surface.

Use below commands to plot the IV surfaces

#. ``gnuplot *.gnu`` for plotting regressed IV surface.
#. ``gnuplot _raw.gnu`` for plotting raw IV surface.