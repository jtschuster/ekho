CC=g++
CFLAGS=-g -Wall -pedantic -lm -Wno-deprecated
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas

repeat_curve: repeat_curve.cpp
	$(CC) repeat_curve.cpp $(CFLAGS) $(GSL) -o repeat_curve

clean:
	rm -rf repeat_curve repeat_curve.dSYM
