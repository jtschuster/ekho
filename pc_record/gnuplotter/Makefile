# This works on OSX with Homebrew
CC=g++
GL=-framework Foundation -framework glut -framework opengl
CFLAGS=-g -Wall -pedantic -lm -Wno-deprecated
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas

gnu_from_txt: gnu_from_txt.cpp
	$(CC) gnu_from_txt.cpp $(CFLAGS) $(BREW) $(GL) $(GSL) -o gnu_from_txt

clean:
	rm -rf gnu_from_txt gnu_from_txt.dSYM