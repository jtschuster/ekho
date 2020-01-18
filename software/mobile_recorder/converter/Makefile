CC=g++
CFLAGS=-g -Wall -pedantic -lm -Wno-deprecated
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas

all: convert.cpp
	$(CC) convert.cpp $(CFLAGS) $(GSL) -o convert


clean:
	rm -rf convert convert.dSYM
