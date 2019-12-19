CC=clang++
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas
CFLAGS=-g -Wall -pedantic -lm




emulate:
	$(CC) emulatortest.cpp $(CFLAGS) $(GSL) $(BREW) -o emulatortest

clean:
	rm -rf emulatortest emulatortest.dSYM
