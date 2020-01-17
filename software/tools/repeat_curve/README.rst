Repeating IV Curves
===================

This directory contains the tool for generating a 10 second long IV surface from one IV curve in an ``.ivs`` file.

To generaye the IV surface, follow below steps

#. Compile ``repeat_curve.cpp`` file using ``make`` or ``make all``.
#. Genrate the IV surface using below command.

``./repeat_curve <path_to_ivs_file> <time_of_curve> <output_file_name>`

An ``*.ivs`` will be generated that can be used for emulation.

Use ``make clean`` or ``make remake`` if needed.