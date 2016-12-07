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
 * insert datas in the structure gived to threads
 * @param data 		- structure to fill
 * @param nbrThreads - number of threads 
 * @param frequency 	- frequency of screen refresh
 * @param display 	- semaphore for display synchronization
 * @param workers 	- semaphores for workers synchronization
 * @param gfxSynchro - semaphore for gfx creation synchronization
 * @param width 		- width of the array 
 * @param height 		- height of the array
 * @param seed 		- seed used for randomness
 * @param probability- probability of having a living cell
 * @return none
 **********************************************************************/
void initData(thData* data, int nbrThreads, int frequency, 
			sem_t* display, sem_t** workers, sem_t* gfxSynchro, int width, 
									int height, double seed, double probability){
	data->ID = 0;
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
 * show the syntax to throw the game
 * @param none
 * @return none
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
