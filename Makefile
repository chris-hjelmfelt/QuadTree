# Chris Hjelmfelt and Cheldon Coughlen
# CSC-300 Data Structures, Fall 2015
# Usage: 


CC=g++

all:
	$(CC) -o quadTree globals.cpp BMPdisplay.cpp BMPload.cpp quadTree.cpp -lglut -lGLU -lGL -lm -std=c++11 -g -pg

clean:
	rm -f *.o *~
