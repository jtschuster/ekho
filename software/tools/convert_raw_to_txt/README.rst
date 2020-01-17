Binary to Text
==============

This directory contains the tool for converting ``.raw`` (binary) data file into a ``.txt`` file with format ``<timestamp, voltage, current>``.

To convert the data, follow below steps

#. Compile ``convert.c`` file using ``make`` or ``make all``.
#. Convert the data using below command.

``./convert <path_to_raw_file>``

Use ``make clean`` or ``make remake`` if needed.

Following file(s) will be generated after running ``convert``

**rawData.txt**
    A human readable IV surface with each line representing a point on raw IV curve.