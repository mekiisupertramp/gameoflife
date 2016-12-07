/*==========================================================================================
  ==========================================================================================
	File : threads.c

   Descritpion: implementation of thread functions
	
	Authors : Mehmed Blazevic, Buffo Pierre, Da Silva Marques Gabriel
	
	Date : December 2016

   Version: 1.0.0

==========================================================================================*/

#include "threads.h"


/***********************************************************************
 * function given to the thread wich will calculate cell's states
 * @param threadData - structure with all the informations
 * @return NULL
 **********************************************************************/
void* worker(void* threadData){

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	thData* tdata = (thData*)threadData;
	int id = tdata->ID;
	sem_post(tdata->gfxSynchro); // liberates main for the next worker 
	int scope = (tdata->gfx->height-2)*(tdata->gfx->width-2);
	int cellToTest = id;	

	while(1){
		sem_wait(&(tdata->semWorkers[0][id])); // waiting for display 
	
		int i = 0;
		while(cellToTest <= scope){
			lifeIsSad(cellToTest, tdata->gfx);
			cellToTest = ++i * tdata->nbrThreads + id;			
		}
		
		cellToTest = id;
		sem_post(tdata->semDisplay);	// liberates display sem.
	}
	return NULL;
}

/***********************************************************************
 * kill or give life to a cell by editing his colour 
 * @param cellToTest - the number of the cell to check
 * @param gfx 			- context
 * @return none
 **********************************************************************/
void lifeIsSad(int cellToTest, struct gfx_context_t* gfx){
	int x = (cellToTest%(gfx->width-2)) + 1;
	int y = (cellToTest/(gfx->width-2)) + 1;
	// writing in the next pixels state array 
	gfx_putpixel2(gfx,x,y,isAlive(x,y,gfx));
}

/***********************************************************************
 * say if the cell will survive or not
 * @param x   - col of the cell to check
 * @param y   - line of the cell to check
 * @param gfx - context
 * @return the next colour of cell
 **********************************************************************/
uint32_t isAlive(int x, int y, struct gfx_context_t* gfx){
	int neighboursAlive = countNeighboursAlive(x,y,gfx);
	// check the present pixels state array 
	if(gfx->pixels[gfx->width*y+x] == ALIVE){
		if((neighboursAlive == 2) || (neighboursAlive == 3)){
			return ALIVE;
		}else{
			return DEAD;
		}
	}
	else{
		if(neighboursAlive == 3){
			return ALIVE;
		}else{
			return DEAD;	
		}
	}
}

/***********************************************************************
 * count the number of neighbours alive of a cell
 * @param line - line to check the neigbours
 * @param col  - col to check the neigbours
 * @param gfx  - context
 * @return the number of neighbourgs alive
 **********************************************************************/
int countNeighboursAlive(int x, int y, struct gfx_context_t* gfx){
	int neighboursAlive = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int e = -1; e <= 1; e++)
		{
			if(!((i == 0) && (e == 0))){
				if((gfx->pixels[(gfx->width*(y+i))+(x+e)]) == ALIVE)
					neighboursAlive++;
			}			
		}
	}
	return neighboursAlive;	
}

/***********************************************************************
 * displaying thread
 * also create, present, and destroy gfx
 * @param gfx - structure with all the informations including the 
 *					 context to display 
 * @return NULL
 **********************************************************************/
void* display(void* gfx){

	thData* displayVar = (thData*) gfx;
	// create and initialise the gfx contaxt 
	displayVar->gfx = gfx_create("Game of life bitches", 
											displayVar->width, displayVar->height);
	initGfx(displayVar->gfx,displayVar->seed,displayVar->probability);
	
	// workers first liberation 
	sem_post(displayVar->gfxSynchro);

	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC, &start);	
	while(1){
		// probability is also used for destroy the context properly
		if(displayVar->probability != -1.0){
			// wait for every workers end 
			for (int i = 0; i < displayVar->nbrThreads; ++i) {
				sem_wait(displayVar->semDisplay);
			}							
			swapPixel(displayVar->gfx);
			// liberates all workers for process
			for (int i = 0; i < displayVar->nbrThreads; ++i) {
				sem_post(&(displayVar->semWorkers[0][i]));
			}
			usleep(waitAMoment(&start, &finish, displayVar->frequency));
		 	gfx_present(displayVar->gfx);		 			
		}else{
			gfx_destroy(displayVar->gfx);
			return NULL;
		}	
	}
	return NULL;
}

/***********************************************************************
 * calculate the time to wait before the next refresh with the process 
 *	time 
 * @param start 	  - start struct with the timer's begin
 * @param finish 	  - finish struct with the timer's end
 * @param frequency - freq of screen refresh
 * @return time to wait in nanoseconds or 0 if work process too long
 **********************************************************************/
double waitAMoment(struct timespec* start, struct timespec* finish, int frequency){

	double sleepTime, deltaT = 0;
	clock_gettime(CLOCK_MONOTONIC, finish);
	deltaT = (*finish).tv_sec - (*start).tv_sec;
	deltaT += ((*finish).tv_nsec - (*start).tv_nsec)/1000000000.0;
	sleepTime = (1000000.0/frequency) - deltaT;
	clock_gettime(CLOCK_MONOTONIC, start);
	
	return (sleepTime > 0) ? sleepTime : 0;
}

/***********************************************************************
 * swap the two pointers of the gfx context to restart a process
 * @param gfx - context with arrays
 * @return none
 **********************************************************************/
void swapPixel(struct gfx_context_t* gfx){
	uint32_t *temp = gfx->pixels;
	gfx->pixels = gfx->pixelsNextState;
	gfx->pixelsNextState = temp;
}

/// If a key was pressed, returns its key code (non blocking call).
/// List of key codes: https://wiki.libsdl.org/SDL_Keycode
/// @return the key that was pressed or 0 if none was pressed.
SDL_Keycode keypress() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN)
			return event.key.keysym.sym;
	}
	return 0;
}	

/***********************************************************************
 * check every 20ms, if a escape touch is pressed
 * @param data - structure with all the informations 
 * @return none
 **********************************************************************/
void* escape(void* data){
	thData* datas = (thData*) data;	
	while(keypress() != SDLK_ESCAPE){
		usleep(20000);
	}	
	// probability is used a second time for destroy the context properly
	datas->probability = -1.0;
	return NULL;
}	

/***********************************************************************
 * initialize the window context with random live cells and dead extrems
 * @param gfx  		 - context to share to calculate
 * @param seed 		 - to give to the srand function
 * @param probability - of having a cell alive
 * @return none
 **********************************************************************/
void initGfx(struct gfx_context_t* gfx, double seed, double probability){
	srand(seed);
	double val;
	gfx_clear(gfx, DEAD);
	for(int x = 1; x < (gfx->width-1); x++){
		for(int y = 1; y < (gfx->height-1); y++){
			val = (rand()/(double)RAND_MAX);
			if(val < probability){
				// fill the present and the next state of pixels 
 				gfx_putpixel(gfx,x,y,ALIVE); 
				gfx_putpixel2(gfx,x,y,ALIVE);
			}else{ 
				gfx_putpixel(gfx,x,y,DEAD); 
				gfx_putpixel2(gfx,x,y,DEAD);
			}
		}
	}
}	
