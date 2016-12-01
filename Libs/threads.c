//
// Created by pierre.buffo on 23.11.16.
//

#include "threads.h"
#include "gfx.h"

/***********************************************************************
 * function given to the thread wich will calculate cell's states
 * @param threadData Structure with all the informations
 * @return NULL
 **********************************************************************/
void* worker(void* threadData){

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	thData* tdata = (thData*)threadData;
	int id = tdata->ID;

	sem_post(tdata->gfxSynchro);
	int scope = (tdata->gfx->height-2)*(tdata->gfx->width-2);
	int cellToTest = id;
	
	//printf("data[%d].gfx = %p\n",tdata->ID,tdata->gfx);

	while(1){
		sem_wait(&(tdata->semWorkers[0][id]));

		int i = 0;
		
		while(cellToTest <= scope){
			lifeIsSad(cellToTest, tdata->gfx);
			cellToTest = ++i * tdata->nbrThreads + id;			
		}
		
		cellToTest = id;
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
	
	gfx_putpixel2(gfx,x,y,isAlive(x,y,gfx));
}

/***********************************************************************
 * say if the cell will survive or not
 * @param line Line of the cell to check
 * @param col Col of the cell to check
 * @param gfx Context
 * @return the colour
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
	else{
		if(neighboursAlive == 3){
			return ALIVE;
		}else{
			return DEAD;	
		}
	}
}

/***********************************************************************
 * Count the number of neighbours alive of a cell
 * @param line Line to check the neigbours
 * @param col Col to check the neigbours
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
				if((gfx->pixels[(gfx->width*(y+i))+(x+e)]) == ALIVE)
					neighboursAlive++;
			}			
		}
	}
	return neighboursAlive;	
}

/***********************************************************************
 * fuck it i'm too tired for this shit
 * @param gfx Context to show
 * @return none
 **********************************************************************/
void* display(void* gfx){
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	thData* displayVar = (thData*) gfx;
	displayVar->gfx = gfx_create("Game of life bitches", displayVar->width, 
																								displayVar->height);
	initGfx(displayVar->gfx,displayVar->seed,displayVar->probability);

	struct timespec start, finish;
	double deltaT=0;
	double time = (double)(1.0/displayVar->frequency);
	clock_gettime(CLOCK_MONOTONIC, &start);	
	while(1){
		for (int i = 0; i < displayVar->nbrThreads; ++i) {
			sem_wait(displayVar->semDisplay);
		}	
		clock_gettime(CLOCK_MONOTONIC, &finish);
		deltaT = finish.tv_sec-start.tv_sec;
		deltaT += (finish.tv_nsec-start.tv_nsec)/1000000000.0;		
		if(((time-deltaT)*1000000.0)>0) usleep(((time-deltaT)*1000000.0));	
		swapPixel(displayVar->gfx);
		for (int i = 0; i < displayVar->nbrThreads; ++i) {
			sem_post(&(displayVar->semWorkers[0][i]));
		}
		gfx_present(displayVar->gfx);
		sem_post(displayVar->gfxSynchro);
		clock_gettime(CLOCK_MONOTONIC, &start);
	}
	return NULL;
}

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
 * check every 20ms, if a escape touch is pressed. the programm stop
 * properly if escape is pressed
 * @param none
 * @return none
 **********************************************************************/
void* escape(){	
	while(keypress() != SDLK_ESCAPE){
		usleep(20000);
	}	
	return NULL;
}	

/***********************************************************************
 * initialize the window context with random live cells and dead extrems
 * @param gfx context to share to calculate
 * @param seed to give to the srand function
 * @param probability of having a cell alive
 * @return double between 0 and 1
 **********************************************************************/
void initGfx(struct gfx_context_t* gfx, double seed, double probability){
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
