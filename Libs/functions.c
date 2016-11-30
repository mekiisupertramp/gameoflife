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
<<<<<<< HEAD
void initData(thData* data, int ID, int nbrThreads, int frequency, 
					sem_t* display, sem_t** workers, sem_t* gfxSynchro,  int width, int height, 
																				double seed, double probability){
	data->ID = ID;
	data->nbrThreads = nbrThreads;	
	data->frequency = frequency;
	data->semWorkers = workers;
	data->semDisplay = display;
	data->gfxSynchro = gfxSynchro;
=======
void initData(thData* data, struct gfx_context_t* gfx, int ID, int nbrThreads, int frequency, 
					int* nbrWorkers, sem_t* display, sem_t** workers, int width, int height, double seed, double probability){
	data->ID = ID;
	data->nbrThreads = nbrThreads;	
	data->gfx = gfx;
	data->frequency = frequency;
	data->nbrWorkers = nbrWorkers;
	data->semWorkers = workers;
	data->semDisplay = display;
>>>>>>> 2b5b543f5f8929021414bcffd4da37e7118c272f
	data->width = width;
	data->height = height;
	data->seed = seed;
	data->probability = probability;
}
<<<<<<< HEAD
=======



/***********************************************************************
 * initialize the struct to give to each thread
 * @param thData struct to initialize
 * @param id of the thread
 * @param gfx context to share to calculate
 * @return none
 **********************************************************************/
/*void initWorkersStruct(threadsData* thData, int id, struct gfx_context_t* gfx, int nbrThreads, sem_t* semDisplay, sem_t** semWorkers){
	thData->ID = id;
	thData->nbrThreads = nbrThreads;
	thData->gfx = gfx;
	thData->semWorkers = semWorkers;
	thData->semDisplay = semDisplay;
}*/

/***********************************************************************
 * initialize the struct to give to the displayer thread
 * @param displayVar struct to initialize
 * @param gfx context to share to show
 * @param frequency of the display
 * @return none
 **********************************************************************/
/*void initDisplayStruct(displaySt* displayVar, struct gfx_context_t* gfx, int frequency, uint *nbrWorkers, sem_t* semDisplay, sem_t** semWorkers){
	displayVar->gfx = gfx;
	displayVar->frequency = frequency;
	displayVar->nbrWorkers = nbrWorkers;
	displayVar->semWorkers = semWorkers;
	displayVar->semDisplay = semDisplay;
}*/
>>>>>>> 2b5b543f5f8929021414bcffd4da37e7118c272f

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
