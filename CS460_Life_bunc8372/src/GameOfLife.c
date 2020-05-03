/*****************************************************************************
 File name:   GameOfLife.c
 Author:      Julian Bunch
 Date:        04/22/20
 Class:       CS460
 Assignment:  Game Of Life
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../include/GameOfLife.h"

/****************************************************************************
 Function:    gofCreate
 Description: mallocs 2D array, sets member variables
 Parameters:  psGame  - Game of Life to create
 	 	 	 	 	 	  szInput - path to the input file
 Returned:    None
 ****************************************************************************/
extern void gofCreate(GameOfLife *psGame, char *szInput)
{
	int width, height;
	FILE *inputFile;

	inputFile = fopen(szInput, "r");
	fscanf(inputFile, "%d", &width);
	fscanf(inputFile, "%d", &height);
	fclose(inputFile);

	psGame->pBoard = malloc(sizeof(char*) * width);
	psGame->pTempBoard = malloc(sizeof(char*) * width);
	for (int i = 0; i < width; i++)
	{
		psGame->pBoard[i] = malloc(sizeof(char) * height);
		memset(psGame->pBoard[i], '\0', height);

		psGame->pTempBoard[i] = malloc(sizeof(char) * height);
		memset(psGame->pTempBoard[i], '\0', height);
	}

	psGame->width = width;
	psGame->height = height;
	psGame->births = 0;
	psGame->deaths = 0;
	psGame->totalBirths = 0;
	psGame->totalDeaths = 0;
	psGame->generations = 0;

	pthread_mutex_init(&psGame->lock, NULL);
}

/****************************************************************************
 Function:    gofTerminate
 Description: frees the board in psGame
 Parameters:  psGame - Game of Life to terminate
 Returned:    None
 ****************************************************************************/
extern void gofTerminate(GameOfLife *psGame)
{
	for (int i = 0; i < psGame->width; i++)
	{
		free(psGame->pBoard[i]);
		free(psGame->pTempBoard[i]);
	}
	free(psGame->pBoard);
	free(psGame->pTempBoard);
	pthread_mutex_destroy(&psGame->lock);
}

/****************************************************************************
 Function:    gofLoad
 Description: reads data from the input file and copies it to psGame
 Parameters:  psGame  - Game of Life to load the data into
 	 	 	 	 	 	  szInput - path to the input file
 Returned:    None
 ****************************************************************************/
extern void gofLoad(GameOfLife *psGame, char *szInput)
{
	FILE *inputFile;
	int width, height;
	char throwAway;

	inputFile = fopen(szInput, "r");
	fscanf(inputFile, "%d%d", &width, &height);

	for (int i = 0; i < height; i++)
	{
		fscanf(inputFile, "%c", &throwAway);
		for (int j = 0; j < width; j++)
		{
			fscanf(inputFile, "%c", &psGame->pBoard[i][j]);
			psGame->pTempBoard[i][j] = psGame->pBoard[i][j];
			if (psGame->pBoard[i][j] != DEAD && psGame->pBoard[i][j] != LIVING)
			{
				j--;
			}
		}
	}

	fclose(inputFile);
}

/****************************************************************************
 Function:    gofCycle
 Description: Runs one cycle of life of the game
 Parameters:  pArgs - pointer to struct with Game and which half to compute
 Returned:    0
 ****************************************************************************/
extern void *gofCycleHalf(void *pArgs)
{
	CycleHalfArgs *psArgs = (CycleHalfArgs*)pArgs;
	unsigned long long deaths = 0, births = 0;

	for (int row = (psArgs->psGame->height / 2) * psArgs->half;
			row < (psArgs->psGame->height / 2) * (psArgs->half + 1); row++)
	{
		for (int col = 0; col < psArgs->psGame->width; col++)
		{
			int livingNeighbors = 0;

			// Top left neighbor
			if (row > 0 && col > 0 &&
					psArgs->psGame->pTempBoard[row - 1][col - 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Top neighbor
			if (row > 0 && psArgs->psGame->pTempBoard[row - 1][col] == LIVING)
			{
				livingNeighbors++;
			}

			// Top right neighbor
			if (row > 0 && col < psArgs->psGame->width - 1
					&& psArgs->psGame->pTempBoard[row - 1][col + 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Right neighbor
			if (col < psArgs->psGame->width - 1 &&
					psArgs->psGame->pTempBoard[row][col + 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Bottom right neighbor
			if (row < psArgs->psGame->height - 1 && col < psArgs->psGame->width - 1
					&& psArgs->psGame->pTempBoard[row + 1][col + 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Bottom neighbor
			if (row < psArgs->psGame->height - 1 &&
					psArgs->psGame->pTempBoard[row + 1][col] == LIVING)
			{
				livingNeighbors++;
			}

			// Bottom left neighbor
			if (row < psArgs->psGame->height - 1 && col > 0
					&& psArgs->psGame->pTempBoard[row + 1][col - 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Left neighbor
			if (col > 0 && psArgs->psGame->pTempBoard[row][col - 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Kills cell if lonely or overcrowded
			if ((livingNeighbors < MIN_TO_LIVE || livingNeighbors > MAX_TO_LIVE)
					&& psArgs->psGame->pTempBoard[row][col] == LIVING)
			{
				deaths++;
				psArgs->psGame->pBoard[row][col] = DEAD;
			}
			// ... or brings cell back to life
			else if (livingNeighbors == NUM_TO_RESURRECT
					&& psArgs->psGame->pTempBoard[row][col] == DEAD)
			{
				births++;
				psArgs->psGame->pBoard[row][col] = LIVING;
			}
		}
	}

	pthread_mutex_lock(&psArgs->psGame->lock);
	psArgs->psGame->deaths += deaths;
	psArgs->psGame->totalDeaths += deaths;
	psArgs->psGame->births += births;
	psArgs->psGame->totalBirths += births;
	pthread_mutex_unlock(&psArgs->psGame->lock);

	return 0;
}

/****************************************************************************
 Function:    gofPrintTotalStats
 Description: Prints the total deaths and births for the whole game
 Parameters:  psGame  - Game of Life to print data from
 Returned:    None
 ****************************************************************************/
extern void gofPrintTotalStats(GameOfLife *psGame)
{
	printf("\nTOTAL DEATHS: %llu\tTOTAL BIRTHS: %llu\n",
			psGame->totalDeaths, psGame->totalBirths);
}

/****************************************************************************
 Function:    gofPrintToFile
 Description: Prints the game's width, height, then board to a file
 Parameters:  psGame  - Game of Life to print data from
 	 	 	 	 	 	  szOutput - path to output file
 Returned:    None
 ****************************************************************************/
extern void gofPrintToFile(GameOfLife *psGame, char *szOutput)
{
	FILE *outputFile;

	outputFile = fopen(szOutput, "w");
	fprintf(outputFile, "%d\n%d", psGame->width, psGame->height);

	for (int i = 0; i < psGame->height; i++)
	{
		fprintf(outputFile, "\n");
		for (int j = 0; j < psGame->width; j++)
		{
			fprintf(outputFile, "%c ", psGame->pBoard[i][j]);
		}
	}

	fclose(outputFile);
}

/****************************************************************************
 Function:    gofPrintGenStats
 Description: Prints the statistics for the last ran generation
 Parameters:  psGame  - Game of Life to print data from
 Returned:    None
 ****************************************************************************/
extern void gofPrintGenStats(GameOfLife *psGame)
{
	printf("Generation %d:\tDEATHS: %d\tBIRTHS: %d\n",
			psGame->generations, psGame->deaths, psGame->births);
}

/****************************************************************************
 Function:    gofStartGen
 Description: Resets generation specific variables like births, deaths, and
 	 	 	 	 	 	  the temporary board
 Parameters:  psGame  - Game of Life to modify
 Returned:    None
 ****************************************************************************/
extern void gofStartGen(GameOfLife *psGame)
{
	psGame->births = 0;
	psGame->deaths = 0;

	// Copy psGame's board to tempBoard
	for (int row = 0; row < psGame->height; row++)
	{
		for (int col = 0; col < psGame->width; col++)
		{
			psGame->pTempBoard[row][col] = psGame->pBoard[row][col];
		}
	}
}

/****************************************************************************
 Function:    gofEndGen
 Description: Adds 1 to generations, to be used after a life cycle
 Parameters:  psGame  - Game of Life to modify
 Returned:    None
 ****************************************************************************/
extern void gofEndGen(GameOfLife *psGame)
{
	psGame->generations++;
}




