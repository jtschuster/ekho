CC=g++
GL=-framework Foundation -framework glut -framework opengl
CFLAGS=-g -Wall -pedantic -lm -Wno-deprecated
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas

all: record.cpp
	$(CC) record.cpp $(CFLAGS) $(BREW) $(GL) $(GSL) -o record

clean:
	rm -rf record record.dSYM *.raw *.ivs
