SUBDIRS = recorder
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

