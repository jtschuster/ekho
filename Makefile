SUBDIRS = emulate pc_record mobile_record verification
MAKE = make all
CLEAN = make clean

all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done 

clean:
	for dir in $(SUBDIRS); do \
		$(CLEAN) -C $$dir; \
	done 

remake:
	for dir in $(SUBDIRS); do \
		$(CLEAN) -C $$dir; \
		$(MAKE) -C $$dir; \
	done 