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
	int i = 0;
	int cellToTest = tdata->ID;
	
	while(cellToTest <= scope){
		lifeIsSad(cellToTest, tdata->gfx);
		cellToTest = ++i * tdata->nbrThreads + tdata->ID;
	}
	// barrière
	return NULL;
}    

/***********************************************************************
 * kill or give life to a cell by editing his colour 
 * @param cellToTest the number of the cell to check
 * @param gfx Context
 * @return none
 **********************************************************************/
void lifeIsSad(int cellToTest, struct gfx_context_t* gfx){
	
	int line = (cellToTest/(gfx->width-2)) + 1;
	int col = (cellToTest%(gfx->width-2)) + 1;
	
	if(isAlive(line,col,gfx))
		gfx_putpixel2(gfx,line,col,ALIVE);
	else
		gfx_putpixel2(gfx,line,col,DEAD);
}

/***********************************************************************
 * say if the cell will survive or not
 * @param line Line of the cell to check
 * @param col Col of the cell to check
 * @param gfx Context
 * @return bool true if survive else false
 **********************************************************************/
bool isAlive(int line, int col, struct gfx_context_t* gfx){
	int neighboursAlive = countNeighboursAlive(line,col,gfx);
	if(gfx->pixels[gfx->width*col+line] == ALIVE){
		if(neighboursAlive >= 2 && neighboursAlive <= 3)
			return true;
		else
			return false;
	}
	else {
		if(neighboursAlive == 3)
			return true;
		else
			return false;	
	}
}

/***********************************************************************
 * Count the number of neighbours alive of a cell
 * @param line Line to check the neigbours
 * @param col Col to check the neigbours
 * @param gfx Context
 * @return int The number of neighbourgs alive
 **********************************************************************/
int countNeighboursAlive(int line, int col, struct gfx_context_t* gfx){
	int neighboursAlive = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int e = -1; e < 1; e++)
		{
			if(i != 0 && e != 0 && (gfx->pixels[gfx->width*(col+i)+(line+e)] == ALIVE) )
				neighboursAlive++;
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
	swapPixel(gfx);
	
	while(1){		
		gfx_present(displayVar->gfx);
		usleep(1000);
	}
	return NULL;
}

void swapPixel(struct gfx_context_t* gfx){
	uint32_t* temp = gfx->pixels;
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
