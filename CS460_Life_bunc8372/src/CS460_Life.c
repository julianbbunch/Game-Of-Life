/*****************************************************************************
 File name:   CS460_Life.c
 Author:      Julian Bunch
 Date:        04/22/20
 Class:       CS460
 Assignment:  Game of Life
 Purpose:     Learn to use threads
 ****************************************************************************/

#include "../include/GameOfLife.h"

int main (int argc, char *argv[])
{
	GameOfLife sGame;
	GameOfLife *psGame = &sGame;
	int numGenerations = atoi(argv[3]);
	char *szInput = argv[1];
	char *szOutput = argv[2];

	gofCreate(psGame, szInput);
	gofLoad(psGame, szInput);

	for (int i = 0; i < numGenerations; i++)
	{
		gofCycle(psGame);

		if (!(argc > 4 && !strcmp(argv[4], X_OPTION))
				&& !(argc > 5 && !strcmp(argv[5], X_OPTION)))
		{
			gofPrintGenStats(psGame);
		}
	}

	gofPrintTotalStats(psGame);

	if (!(argc > 4 && !strcmp(argv[4], F_OPTION))
			&& !(argc > 5 && !strcmp(argv[5], F_OPTION)))
	{
		gofPrintToFile(psGame, szOutput);
	}

	gofTerminate(psGame);

	return 0;
}
