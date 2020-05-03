/****************************************************************************
 Milestone 1:
 First Time:
 	 real 153.22
 	 user 153.05
 	 sys 0.04

 Second Time:
 	 real 177.37
 	 user 176.94
 	 sys 0.09
***************************************************************************/

/**************************************************************************
 Milestone 2:
 First time:
 	 real 89.31
	 user 152.47
	 sys 1.13

 Second time:
 	 real 87.15
	 user 149.79
	 sys 0.89
***************************************************************************/


/*****************************************************************************
 File name:   CS460_Life.c
 Author:      Julian Bunch
 Date:        04/22/20
 Class:       CS460
 Assignment:  Game of Life
 Purpose:     Learn to use threads
 ****************************************************************************/

#include <pthread.h>

#include "../include/GameOfLife.h"

/****************************************************************************
 Function:    main
 Description: Driver the Game of Life
 Parameters:  argc - number of command line arguments
 	 	 	 	 	 	  argv - argument values
 Returned:    None
 ****************************************************************************/
int main (int argc, char *argv[])
{
	GameOfLife sGame;
	GameOfLife *psGame = &sGame;
	CycleHalfArgs sArgs1, sArgs2;
	pthread_t tid[2];
	int numGenerations = atoi(argv[3]);
	char *szInput = argv[1];
	char *szOutput = argv[2];

	// Set up arguments for gofCycleHalf
	sArgs1.half = 0;
	sArgs2.half = 1;
	sArgs1.psGame = psGame;
	sArgs2.psGame = psGame;

	gofCreate(psGame, szInput);
	gofLoad(psGame, szInput);

	// Run the game
	for (int i = 0; i < numGenerations; i++)
	{
		gofStartGen(psGame);

		pthread_create(&tid[0], NULL, gofCycleHalf, (void*) &sArgs1);
		pthread_create(&tid[1], NULL, gofCycleHalf, (void*) &sArgs2);

		pthread_join(tid[0], NULL);
		pthread_join(tid[1], NULL);

		gofEndGen(psGame);

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
