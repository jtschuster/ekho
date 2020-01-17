Ekho Tools
==========

This folder contains Tools for visualizing and converting binary data to other readable formats. The converted data is used to emulate and display IV surfaces on the screen.

Directories
-----------

- ``generate_surface`` Converts ``.raw`` data file into ``.ivs`` and ``.gnu`` files for visualizing recorded and regressed IV surfaces.
- ``convert_IV_to_txt_file`` Converts an ``.ivs`` file into a ``.txt`` file with format ``<timestamp, voltage, current>``.
- ``convert_rawData_to_txt_file: Converts a ``.raw`` file into a ``.txt`` file with format ``<timestamp, voltage, current>``.

Usage
-----

- Run ``make clean`` to remove executable from all sub-directories.
- Run ``make`` or ``make all`` to compile and generate execuatables.
- Run ``remake`` to clean, compile and generate execuatables.
- Follow instructions in desired directories to proceed.
