/*****************************************************************************
 File name:   GameOfLife.h
 Author:      Julian Bunch
 Date:        04/22/20
 Class:       CS460
 Assignment:  Game Of Life
 ****************************************************************************/

#ifndef INCLUDE_GAMEOFLIFE_H_
#define INCLUDE_GAMEOFLIFE_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

//****************************************************
// Constants and global variables
//****************************************************
#define F_OPTION "-F"
#define X_OPTION "-X"
#define DEAD '0'
#define LIVING '1'
#define MIN_TO_LIVE 2
#define MAX_TO_LIVE 3
#define NUM_TO_RESURRECT 3



//****************************************************
// User-defined types
//****************************************************
typedef struct GameOfLife
{
	char **pBoard, **pTempBoard;
	int width, height, births, deaths, generations;
	unsigned long long totalBirths, totalDeaths;

	pthread_mutex_t lock;
}GameOfLife;

typedef struct CycleHalfArgs
{
	GameOfLife *psGame;
	int half;
} CycleHalfArgs;
//****************************************************
// Functions
//****************************************************

extern void gofCreate(GameOfLife *psGame, char *szInput);
extern void gofTerminate(GameOfLife *psGame);
extern void gofLoad(GameOfLife *psGame, char *szInput);
extern void gofStartGen(GameOfLife *psGame);
extern void gofEndGen(GameOfLife *psGame);
extern void *gofCycleHalf(void *psArgs); // TODO
extern void gofPrintTotalStats(GameOfLife *psGame);
extern void gofPrintToFile(GameOfLife *psGame, char *szOutput);
extern void gofPrintGenStats(GameOfLife *psGame);

#endif /* INCLUDE_COMMAND_H_ */

