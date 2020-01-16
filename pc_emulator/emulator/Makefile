CC=clang++
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas
CFLAGS=-g -Wall -pedantic -lm



all:
	$(CC) emulate.cpp $(CFLAGS) $(GSL) $(BREW) -o emulate
	

clean:
	rm -rf emulate emulate.dSYM



