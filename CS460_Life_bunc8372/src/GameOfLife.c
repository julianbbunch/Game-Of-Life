/*****************************************************************************
 File name:   GameOfLife.c
 Author:      Julian Bunch
 Date:        04/22/20
 Class:       CS460
 Assignment:  Game Of Life
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

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

	psGame->pBoard = malloc(sizeof(unsigned char*) * width);
	for (int i = 0; i < width; i++)
	{
		psGame->pBoard[i] = malloc(sizeof(unsigned char) * height);
		memset(psGame->pBoard[i], '\0', height);
	}

	psGame->width = width;
	psGame->height = height;
	psGame->births = 0;
	psGame->deaths = 0;
	psGame->totalBirths = 0;
	psGame->totalDeaths = 0;
	psGame->generations = 0;
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
	}
	free(psGame->pBoard);
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
		}
	}


	fclose(inputFile);
}

/****************************************************************************
 Function:    gofCycle
 Description: Runs one cycle of life of the game
 Parameters:  psGame - Game of Life to run
 Returned:    None
 ****************************************************************************/
extern void gofCycle(GameOfLife *psGame)
{
	unsigned char tempBoard[psGame->height][psGame->width];
	psGame->births = 0;
	psGame->deaths = 0;

	// Copy psGame's board to tempBoard
	for (int row = 0; row < psGame->height; row++)
	{
		for (int col = 0; col < psGame->width; col++)
		{
			tempBoard[row][col] = psGame->pBoard[row][col];
		}
	}

	// Process each cell, count living neighbors
	for (int row = 0; row < psGame->height; row++)
	{
		for (int col = 0; col < psGame->width; col++)
		{
			int livingNeighbors = 0;

			// Top left neighbor
			if (row > 0 && col > 0 && psGame->pBoard[row - 1][col - 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Top neighbor
			if (row > 0 && psGame->pBoard[row - 1][col] == LIVING)
			{
				livingNeighbors++;
			}

			// Top right neighbor
			if (row > 0 && col < psGame->width - 1
					&& psGame->pBoard[row - 1][col + 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Right neighbor
			if (col < psGame->width - 1 && psGame->pBoard[row][col + 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Bottom right neighbor
			if (row < psGame->height - 1 && col < psGame->width - 1
					&& psGame->pBoard[row + 1][col + 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Bottom neighbor
			if (row < psGame->height - 1 && psGame->pBoard[row + 1][col] == LIVING)
			{
				livingNeighbors++;
			}

			// Bottom left neighbor
			if (row < psGame->height - 1 && col > 0
					&& psGame->pBoard[row + 1][col - 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Left neighbor
			if (col > 0 && psGame->pBoard[row][col - 1] == LIVING)
			{
				livingNeighbors++;
			}

			// Kills cell if lonely or overcrowded
			if ((livingNeighbors < MIN_TO_LIVE || livingNeighbors > MAX_TO_LIVE)
					&& tempBoard[row][col] == LIVING)
			{
				psGame->deaths++;
				psGame->totalDeaths++;
				tempBoard[row][col] = DEAD;
			}
			// ... or brings cell back to life
			else if (livingNeighbors == NUM_TO_RESURRECT
					&& tempBoard[row][col] == DEAD)
			{
				psGame->births++;
				psGame->totalBirths++;
				tempBoard[row][col] = LIVING;
			}
		}
	}

	// Copy tempBoard to psGame
	for (int row = 0; row < psGame->height; row++)
	{
		for (int col = 0; col < psGame->width; col++)
		{
			psGame->pBoard[row][col] = tempBoard[row][col];
		}
	}

	psGame->generations++;
}

/****************************************************************************
 Function:    gofPrintTotalStats
 Description: Prints the total deaths and births for the whole game
 Parameters:  psGame  - Game of Life to print data from
 Returned:    None
 ****************************************************************************/
extern void gofPrintTotalStats(GameOfLife *psGame)
{
	printf("\nTOTAL DEATHS: %d\tTOTAL BIRTHS: %d\n",
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
			fprintf(outputFile, "%c", psGame->pBoard[i][j]);
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


