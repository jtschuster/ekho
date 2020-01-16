SUBDIRS = recorder
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