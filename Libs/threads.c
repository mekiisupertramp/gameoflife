/*==========================================================================================
  ==========================================================================================
	File : threads.c

   Descritpion: implementation of thread functions
	
	Authors : Mehmed Blazevic, Buffo Pierre, Da Silva Marques Gabriel
	
	Date : December 2016

   Version: 1.0.0

==========================================================================================*/
#include "threads.h"
#include "gfx.h"

/***********************************************************************
 * function given to the thread wich will calculate cell's states
 * @param threadData Structure with all the informations
 * @return NULL
 **********************************************************************/
void* worker(void* threadData){
	
	thData* tdata = (thData*)threadData;
	int id = tdata->ID;
	
	// waiting for gfx creation
	sem_post(tdata->gfxSynchro);
	int scope = (tdata->gfx->height-2)*(tdata->gfx->width-2);
	int cellToTest = id;

	while(1){
		// waiting for display liberation
		sem_wait(&(tdata->semWorkers[0][id]));
		if(tdata->finish == FINISH) return NULL;

		int i = 0;
		
		while(cellToTest <= scope){
			lifeIsSad(cellToTest, tdata->gfx);
			cellToTest = ++i * tdata->nbrThreads + id;			
		}
		
		cellToTest = id;
		// liberation of display
		sem_post(tdata->semDisplay);
	}
	return NULL;
}

/***********************************************************************
 * kill or give life to a cell by editing his colour 
 * @param cellToTest the number of the cell to check
 * @param gfx Context
 * @return none
 **********************************************************************/
void lifeIsSad(int cellToTest, struct gfx_context_t* gfx){
	
	int x = (cellToTest%(gfx->width-2)) + 1;
	int y = (cellToTest/(gfx->width-2)) + 1;
	
	// writing in the second array
	gfx_putpixel2(gfx,x,y,isAlive(x,y,gfx));
}

/***********************************************************************
 * say if the cell will survive or not
 * @param x Col of the cell to check
 * @param y Line of the cell to check
 * @param gfx Context
 * @return the next colour of cell
 **********************************************************************/
uint32_t isAlive(int x, int y, struct gfx_context_t* gfx){
	int neighboursAlive = countNeighboursAlive(x,y,gfx);
	
	if(gfx->pixels[gfx->width*y+x] == ALIVE){
		if((neighboursAlive == 2) || (neighboursAlive == 3)){
			return ALIVE;
		}else{
			return DEAD;
		}
	}
	else if(neighboursAlive == 3){
		return ALIVE;
	}else{
		return DEAD;	
	}
}

/***********************************************************************
 * Count the number of neighbours of a cell alive
 * @param x Col to check the neigbours
 * @param y Line to check the neigbours
 * @param gfx Context
 * @return int The number of neighbourgs alive
 **********************************************************************/
int countNeighboursAlive(int x, int y, struct gfx_context_t* gfx){
	int neighboursAlive = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int e = -1; e <= 1; e++)
		{
			if(!((i == 0) && (e == 0))){
				if((gfx->pixels[(gfx->width*(y+e))+(x+i)]) == ALIVE)
					neighboursAlive++;
			}			
		}
	}
	return neighboursAlive;	
}

/***********************************************************************
 * displaying thread
 * also create, present, and destroy gfx
 * @param gfx Context to show
 * @return NULL
 **********************************************************************/
void* display(void* gfx){

	thData* displayVar = (thData*) gfx;
	// create and initialise the gfx context
	displayVar->gfx = gfx_create("Game of life bitches", displayVar->width, displayVar->height);
	initGfx(displayVar->gfx,displayVar->seed,displayVar->probability);
	
	// workers first liberation
	sem_post(displayVar->gfxSynchro);

	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC, &start);
	while(1){
		// wait for workers end process
		for (int i = 0; i < displayVar->nbrThreads; ++i) {
			sem_wait(displayVar->semDisplay);
		}	
		swapPixel(displayVar->gfx);
		// workers liberation for process
		for (int i = 0; i < displayVar->nbrThreads; ++i) {
			sem_post(&(displayVar->semWorkers[0][i]));
		}
		if(displayVar->finish == FINISH){
			gfx_destroy(displayVar->gfx);
			return NULL;
		}
		
		usleep(waitAMoment(&start, &finish, displayVar->frequency));
		gfx_present(displayVar->gfx);					
	}
	return NULL;
}

/***********************************************************************
 * calculate the time to wait before the next refresh with the time of process
 * @param start Start struct with the timer's begin
 * @param finish Finish struct with the timer's end
 * @param frequency freq of screen refresh
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
 * @param gfx Context with arrays
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
 * check every 20ms, if a escape touch is pressed. the programm stops
 * properly if escape is pressed
 * @param none
 * @return NULL
 **********************************************************************/
void* escape(void* data){	
	thData* dataVar = (thData*) data;
	while(keypress() != SDLK_ESCAPE){
		usleep(20000);
	}	
	pthread_mutex_lock(dataVar->m);
	dataVar->finish = FINISH;
	pthread_mutex_unlock(dataVar->m);
	return NULL;
}	

/***********************************************************************
 * initialize the window context with random live cells and dead extrems
 * @param gfx context to share to calculate
 * @param seed to give to the srand function
 * @param probability of having a cell alive
 * @return none
 **********************************************************************/
void initGfx(struct gfx_context_t* gfx, double seed, double probability){
	srand(seed);
	double val;
	gfx_clear(gfx, COLOR_BLACK);
	for(int x = 1; x < (gfx->width-1); x++){
		for(int y = 1; y < (gfx->height-1); y++){
			val = (rand()/(double)RAND_MAX);
			if(val <= probability){
				// fill the two tabs
 				gfx_putpixel(gfx,x,y,ALIVE); 
				gfx_putpixel2(gfx,x,y,ALIVE);
			}else{ 
				gfx_putpixel(gfx,x,y,DEAD); 
				gfx_putpixel2(gfx,x,y,DEAD);
			}
		}
	}
}	
