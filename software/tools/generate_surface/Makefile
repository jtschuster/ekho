# This works on OSX with Homebrew
CC=g++
CFLAGS=-g -Wall -pedantic -lm -Wno-deprecated
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas

all: generate_surface.cpp
	$(CC) generate_surface.cpp $(CFLAGS) $(BREW) $(GL) $(GSL) -o generate_surface

clean:
	rm -rf generate_surface generate_surface.dSYM *.gnu *.png
