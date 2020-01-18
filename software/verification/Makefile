CC=g++
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas
CFLAGS=-g -Wall -pedantic -lm




all:
	$(CC) mse.cpp $(CFLAGS) $(GSL) $(BREW) -o mse

clean:
	rm -rf mse mse.dSYM
