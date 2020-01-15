# CC=g++
# CFLAGS=-g -Wall -pedantic -lm -Wno-deprecated
# BREW=-I/usr/local/include -L/usr/local/lib
# GSL=-I/usr/local/opt/gsl/include -lgsl -lgslcblas

# record: jacksonrecord.cpp
# 	$(CC) jacksonrecord.cpp $(CFLAGS) $(GSL) -o record
CC=g++
GL=-framework Foundation -framework glut -framework opengl
CFLAGS=-g -Wall -pedantic -lm -Wno-deprecated
BREW=-I/usr/local/include -L/usr/local/lib
GSL=-I/usr/local/Cellar/gsl/1.16/include  -lgsl -lgslcblas

record: pc_record.cpp
	$(CC) pc_record.cpp $(CFLAGS) $(BREW) $(GL) $(GSL) -o record

clean:
	rm -rf record pc_record.dSYM
