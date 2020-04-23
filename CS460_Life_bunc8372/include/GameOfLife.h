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

//****************************************************
// Constants
//****************************************************
#define F_OPTION "[-F]"
#define X_OPTION "[-X]"
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
	unsigned char **pBoard;
	int width, height, totalBirths, totalDeaths, births, deaths, generations;
}GameOfLife;

//****************************************************
// Functions
//****************************************************

extern void gofCreate(GameOfLife *psGame, char *szInput);
extern void gofTerminate(GameOfLife *psGame);
extern void gofLoad(GameOfLife *psGame, char *szInput);
extern void gofCycle(GameOfLife *psGame);
extern void gofPrintTotalStats(GameOfLife *psGame);
extern void gofPrintToFile(GameOfLife *psGame, char *szOutput);
extern void gofPrintGenStats(GameOfLife *psGame);

#endif /* INCLUDE_COMMAND_H_ */

