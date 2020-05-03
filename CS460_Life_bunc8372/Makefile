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
FAST_OBJECTS = bin/CS460_Life_fast.o \
	bin/GameOfLife_fast.o

.PHONY: all clean tarball

all: bin bin/CS460_Life bin/CS460_Life_fast

bin: 
	mkdir -p bin
	
bin/GameOfLife.o: src/GameOfLife.c include/GameOfLife.h
	${CC} ${CFLAGS} -o bin/GameOfLife.o -c src/GameOfLife.c
	
bin/GameOfLife_fast.o: src/GameOfLife.c include/GameOfLife.h
	${CC} ${CFLAGS} -o bin/GameOfLife_fast.o -c src/GameOfLife.c -O3
	
bin/CS460_Life: ${OBJECTS}
	${CC} ${CFLAGS} -o bin/CS460_Life -pthread ${OBJECTS}
	
bin/CS460_Life_fast: ${FAST_OBJECTS}
	${CC} ${CFLAGS} -o bin/CS460_Life_fast -pthread -O3 ${FAST_OBJECTS}

bin/CS460_Life.o:	src/CS460_Life.c \
						include/GameOfLife.h
	${CC} ${CFLAGS} -o bin/CS460_Life.o -c src/CS460_Life.c
	
bin/CS460_Life_fast.o:	src/CS460_Life.c \
						include/GameOfLife.h
	${CC} ${CFLAGS} -o bin/CS460_Life_fast.o -c src/CS460_Life.c -O3
	
valgrind:	bin/CS460_Life
	valgrind -v --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/CS460_Life
	
debug:	bin/CS460_Life
	bin/CS460_Life -d

valgrind_debug:	bin/CS460_Life
	valgrind -v --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/CS460_Life testcases/mediumGame.life testcases/output.txt 20

runTest:	bin/CS460_Life
	bin/CS460_Life

clean:
	rm -f bin/CS460_Life ${OBJECTS} ${FAST_OBJECTS}

tarball: clean
	tar czf ../CS460_Life.tar.gz CS460_Life_bunc8372
	
timer:	bin/CS460_Life
	time -p bin/CS460_Life testcases/mediumGame.life testcases/out 1000 -X -F
	
timer_fast: bin/CS460_Life_fast
	time -p bin/CS460_Life_fast testcases/mediumGame.life testcases/out 1000 -X -F

	