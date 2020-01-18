CC=g++
CFLAGS=-g -Wall -pedantic -lm -Wno-deprecated
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    GL=-lGL -lGLU -lglut
else
    GL=-framework Foundation -framework glut -framework opengl
endif

all: record.cpp
	$(CC) record.cpp $(CFLAGS) $(BREW) $(GL) $(GSL) -o record

clean:
	rm -rf record record.dSYM *.raw *.ivs
