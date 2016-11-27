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
	
	threadsData* tdata = (threadsData*) threadData;
	int scope = (tdata->gfx->height-2)*(tdata->gfx->width-2);
	int cellToTest = tdata->ID;

	while(1){
		sem_wait(tdata->semWorkers);
		int i = 0;
		
		while(cellToTest <= scope){
			lifeIsSad(cellToTest, tdata->gfx);
			cellToTest = ++i * tdata->nbrThreads + tdata->ID;			
		}
		
		cellToTest = tdata->ID;
		
		swapPixel(tdata->gfx);
		gfx_present(tdata->gfx);

		usleep(100000);
		sem_post(tdata->semDisplay);
	}
	return NULL;
}
	/*	printf("worker is waiting\n");
		sem_wait(tdata->sem2);
		printf("worker is processing\n");
		printf("worker finish processing\n");
		sem_post(tdata->sem1);
		printf("worker free affichage\n");		*/

/***********************************************************************
 * kill or give birth to a cell by editing his colour 
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
 * @param x Col of the cell to check
 * @param y Line of the cell to check
 * @param gfx Context
 * @return int The new colour of the cell
 **********************************************************************/
uint32_t isAlive(int x, int y, struct gfx_context_t* gfx){
	int neighboursAlive = countNeighboursAlive(x,y,gfx);
	
	if(gfx->pixels[gfx->width*y+x] == ALIVE){
		if((neighboursAlive >= 2) && (neighboursAlive <= 3)){
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
			if(!(i == 0 && e == 0)){
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
	displaySt* displayVar = (displaySt*) gfx;
	
	while(1){
	
		for (int i = 0; i < *displayVar->nbrWorkers; ++i) {
			sem_wait(displayVar->semDisplay);
		}	
		usleep(10000);		

		//swapPixel(gfx);

		for (int i = 0; i < *displayVar->nbrWorkers; ++i) {
			sem_post(displayVar->semWorkers);
		}
		//gfx_present(displayVar->gfx);
	}
	
	/*	
	while(1){
		for (int i = 0; i < *displayVar->nbrWorkers; ++i) {
			sem_wait(displayVar->semDisplay);
		}
	//	printf("display wait\n");
	//	sem_wait(displayVar->sem1);
	//	printf("display is displaying\n");		
		usleep(10000);		
		gfx_present(displayVar->gfx);
		swapPixel(gfx);
	//	sem_post(displayVar->sem2);
		for (int i = 0; i < *displayVar->nbrWorkers; ++i) {
			sem_post(displayVar->semWorkers);
		}
	}*/
	return NULL;
}

/***********************************************************************
 * Swap pointers of pixels and pixelsNextState 
 * @param gfx Context
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
	if (SDL_PollEvent(&event)) {
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
