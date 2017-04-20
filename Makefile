SHELL = /bin/sh
FLAGS = -g -Wall
CC = g++

.PHONY: all clean

all:    TwoPipesTwoChildren TwoPipesThreeChildren dynpipe

debug:	DynPipe.cpp
	$(CC) $(FLAGS) -g -c DynPipe.cpp
	$(CC) $(FLAGS) -g -o dynpipe DynPipe.o

TwoPipesTwoChildren: TwoPipesTwoChildren.cpp
	$(CC) $(FLAGS) -c TwoPipesTwoChildren.cpp
	$(CC) $(FLAGS) -o TwoPipesTwoChildren TwoPipesTwoChildren.o

TwoPipesThreeChildren: TwoPipesThreeChildren.cpp
	$(CC) $(FLAGS) -c TwoPipesThreeChildren.cpp
	$(CC) $(FLAGS) -o TwoPipesThreeChildren TwoPipesThreeChildren.o

dynpipe: DynPipe.cpp
	$(CC) $(FLAGS) -c DynPipe.cpp
	$(CC) $(FLAGS) -o dynpipe DynPipe.o

clean:
	rm dynpipe TwoPipes*Children *.o
