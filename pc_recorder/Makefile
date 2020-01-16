SUBDIRS = recorder plot_surface convert_IV_to_txt_file convert_rawData_to_txt_file
CLEAN = make clean
MAKE = make all

all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done 

clean:
	rm -rf *.dSYM *.gnu *.ivs *.raw *.png; \
	for dir in $(SUBDIRS); do \
		$(CLEAN) -C $$dir; \
	done 

remake:
	rm -rf *.gnu *.ivs *.raw *.png; \
	for dir in $(SUBDIRS); do \
		$(CLEAN) -C $$dir; \
		$(MAKE) -C $$dir; \
	done 

