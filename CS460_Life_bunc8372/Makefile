#****************************************************
# File name:  Makefile
# Author:     Julian Bunch
# Date:       04/22/20
# Class:      CS460
# Assignment: Game of Life
#****************************************************

CC = gcc
CFLAGS = -g -Wall
OBJECTS = bin/CS460_Life.o \
	bin/GameOfLife.o

.PHONY: all clean tarball

all: bin bin/CS460_Life

bin: 
	mkdir -p bin
	
bin/GameOfLife.o: src/GameOfLife.c include/GameOfLife.h
	${CC} ${CFLAGS} -o bin/GameOfLife.o -c src/GameOfLife.c
	
bin/CS460_Life: ${OBJECTS}
	${CC} ${CFLAGS} -o bin/CS460_Life ${OBJECTS}

bin/CS460_Life.o:	src/CS460_Life.c \
						include/GameOfLife.h
	${CC} ${CFLAGS} -o bin/CS460_Life.o -c src/CS460_Life.c
	
valgrind:	bin/CS460_Life
	valgrind -v --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/CS460_Life
	
debug:	bin/CS460_Life
	bin/CS460_Life -d

valgrind_debug:	bin/CS460_Life
	valgrind -v --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/CS460_Life testcases/smallGame.life testcases/smallGameValgrind.life 4

runTest:	bin/CS460_Life
	bin/CS460_Life

clean:
	rm -f bin/CS460_Life ${OBJECTS}

tarball: clean
	tar czf ../CS460_Life.tar.gz CS460_Life_bunc8372
	
timer:
	time -p bin/CS460_Life testcases/mediumGame.life testcases/out 1000 -X -F

	