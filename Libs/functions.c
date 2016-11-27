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
void initGfx(struct gfx_context_t* gfx, int seed, double probability){
	srand(seed);
	double val;
	gfx_clear(gfx, COLOR_BLACK);
	for(int x = 1; x < (gfx->height-1); x++){
		for(int y = 1; y < (gfx->width-1); y++){
			val = (rand()/(double)RAND_MAX);
			if(val <= probability){
 				gfx_putpixel(gfx,x,y,ALIVE); 
				gfx_putpixel2(gfx,x,y,ALIVE);
			}else{ 
				gfx_putpixel(gfx,x,y,DEAD); 
				gfx_putpixel2(gfx,x,y,DEAD);
			}
		}
	}
}

/***********************************************************************
 * initialize the struct to give to each thread
 * @param thData struct to initialize
 * @param id of the thread
 * @param gfx context to share to calculate
 * @return none
 **********************************************************************/
void initWorkersStruct(threadsData* thData, int id, struct gfx_context_t* gfx, int nbrThreads, sem_t* semDisplay, sem_t** semWorkers){
	thData->ID = id;
	thData->nbrThreads = nbrThreads;
	thData->gfx = gfx;
	thData->semWorkers = semWorkers;
	thData->semDisplay = semDisplay;
}

/***********************************************************************
 * initialize the struct to give to the displayer thread
 * @param displayVar struct to initialize
 * @param gfx context to share to show
 * @param frequency of the display
 * @return none
 **********************************************************************/
void initDisplayStruct(displaySt* displayVar, struct gfx_context_t* gfx, int frequency, uint *nbrWorkers, sem_t* semDisplay, sem_t** semWorkers){
	displayVar->gfx = gfx;
	displayVar->frequency = frequency;
	displayVar->nbrWorkers = nbrWorkers;
	displayVar->semWorkers = semWorkers;
	displayVar->semDisplay = semDisplay;
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
