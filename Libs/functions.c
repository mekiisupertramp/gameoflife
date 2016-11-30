//
// Created by pierre.buffo on 23.11.16.
//

#include "functions.h"
/***********************************************************************
 * initialize the window context with random live cells and dead extrems
 * @param gfx context to share to calculate
 * @param seed to give to the srand function
 * @param probability of having a cell alive
 * @return double between 0 and 1
 **********************************************************************/
void initData(thData* data, struct gfx_context_t* gfx, int ID, int nbrThreads, int frequency, 
					sem_t* display, pthread_barrier_t* b, int width, int height, double seed, double probability){
	data->ID = ID;
	data->nbrThreads = nbrThreads;	
	data->gfx = gfx;
	data->frequency = frequency;
	data->semDisplay = display;
	data->width = width;
	data->height = height;
	data->seed = seed;
	data->probability = probability;
	data->b = b;
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
