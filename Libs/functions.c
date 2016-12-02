/*==========================================================================================
  ==========================================================================================
	File : functions.c

   Descritpion: Contain the fonctions for the main routine 
	
	Authors : Mehmed Blazevic, Buffo Pierre, Da Silva Marques Gabriel
	
	Date : December 2016

   Version: 1.0.0

==========================================================================================*/

#include "functions.h"

/***********************************************************************
 * insert data in the structure gived to threads
 * @param data Structure to fill
 * @param nbrThreads
 * @param frequency Frequncy of screen refresh
 * @param display Semaphore for display synchronization
 * @param workers Semaphore for workers synchronization
 * @param gfxSynchro Semaphore for gfx creation synchronization
 * @param width Width of the array
 * @param height Height of the array
 * @param seed Seed used for randomness
 * @param probability Probability of having a living cell
 * @return none
 **********************************************************************/
void initData(thData* data, int nbrThreads, int frequency, 
					sem_t* display, sem_t** workers, sem_t* gfxSynchro,  int width, int height, 
					double seed, double probability){
	data->nbrThreads = nbrThreads;	
	data->frequency = frequency;
	data->semWorkers = workers;
	data->semDisplay = display;
	data->gfxSynchro = gfxSynchro;
	data->width = width;
	data->height = height;
	data->seed = seed;
	data->probability = probability;
}

/***********************************************************************
 * cancel the threads for a proper exit
 * @param workers to cancel
 * @param nbrWorkers to cancel
 * @param displayer to cancel
 * @return none
 **********************************************************************/
void exitTreads(pthread_t* workers, int nbrWorkers, pthread_t* displayer){
	pthread_cancel(*displayer);	
	for (int i = 0; i < nbrWorkers; i++){
		pthread_cancel(workers[i]);
	}
}

/***********************************************************************
 * show the syntax to throw the game
 * @param : none
 * @return : none
 **********************************************************************/
void showSyntax(){
	printf("syntax : gameoflife <width> <height> <seed> <p> <freq> <#workers>\n");
	printf("\t- <width> and <height> are integers specifying the dimensions of\n\
	the game (>= 4).\n");
	printf("\t- <seed> is an integer used to randomly populate the board.\n");
	printf("\t- <p> is a floating point value (range [0..1]) which is the\n\
	probability of having a live cell during initialization.\n");
	printf("\t- <freq> is an integer specifying the display frequency in Hz(> 0).\n");
	printf("\t- <#workers> is an integer specifying the number of workerthreads (>= 1).\n");
}
