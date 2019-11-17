# This works on OSX with Homebrew
CC=gcc
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas
PAUDIO=-lportaudio
CFLAGS=-g -Wall -pedantic -lm

gen_surface: gen_surface.c
	$(CC) gen_surface.c $(CFLAGS) $(BREW) $(GSL) -o gen_surface

clean:
	rm -rf sine logic_counter ivptocsv *.dSYM *.o compare_surface gen_surface
