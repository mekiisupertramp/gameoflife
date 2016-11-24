//
// Created by pierre.buffo on 23.11.16.
//

#include "functions.h"

/***********************************************************************
 * initialize the window context with random live cells and dead extrems
 * params: gfx context to share to calculate
 * 		   seed to give to the srand function
 * 		   probability of having a cell alive
 * return: double between 0 and 1
 **********************************************************************/
void initGfx(struct gfx_context_t* gfx, int seed, double probability){
	srand(seed);
	double val;
	gfx_clear(gfx, COLOR_BLACK);
	for(int x = 1; x < (gfx->height-1); x++){
		for(int y = 1; y < (gfx->width-1); y++){
			val = myRand(seed);
			if(val <= probability) gfx_putpixel(gfx,x,y,COLOR_WHITE);
			else gfx_putpixel(gfx,x,y,COLOR_BLACK);
		}
	}
}

/***********************************************************************
 * initialize the struct to give to each thread
 * params: thData struct to initialize
 * 		   id of the thread
 * 		   gfx context to share to calculate
 * return: none
 **********************************************************************/
void initWorkersStruct(threadsData* thData, int id, struct gfx_context_t* gfx, int nbrThreads){
	thData->ID = id;
	thData->nbrThreads = nbrThreads;
	thData->gfx = gfx;
}

/***********************************************************************
 * initialize the struct to give to the displayer thread
 * params: displayVar struct to initialize
 * 		   gfx context to share to show
 * 		   frequency of the display
 * return: none
 **********************************************************************/
void initDisplayStruct(displaySt* displayVar, struct gfx_context_t* gfx, int frequency){
	displayVar->gfx = gfx;
	displayVar->frequency = frequency;
}

/***********************************************************************
 * generates a rondom nomber between 0 and 1
 * params: seed for the randomness
 * return: double between 0 and 1
 **********************************************************************/
double myRand(int seed){
	srand(seed);
	return (rand()/(double)RAND_MAX);
}

/***********************************************************************
 * show the syntax to throw the game
 * params: none
 * return: none
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
