# This works on OSX with Homebrew
CC=g++
GL=-framework Foundation -framework glut -framework opengl
CFLAGS=-g -Wall -pedantic -lm -Wno-deprecated
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas

plot_surface: plot_surface.cpp
	$(CC) plot_surface.cpp $(CFLAGS) $(BREW) $(GL) $(GSL) -o plot_surface


clean:
	rm -rf plot_surface plot_surface.dSYM *.gnu *.ivs
