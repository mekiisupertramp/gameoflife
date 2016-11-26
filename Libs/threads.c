//
// Created by pierre.buffo on 23.11.16.
//

#include "threads.h"

void* worker(void* threadData){
	threadsData* tdata = (threadsData*) threadData;
	int scope = (tdata->gfx->height-2)*(tdata->gfx->width-2);
	int i = 0;
	int cellToTest = tdata->ID;
	
	while(cellToTest <= scope){
		//work(var);
		cellToTest = ++i * tdata->nbrThreads + tdata->ID;
	}
	// barrière
	return NULL;
}    

void* display(void* gfx){
	//threadsData* display = (threadsData*) gfx;
	//gfx_present(display->gfx);
	return NULL;
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
 * params: non
 * return: none
 **********************************************************************/
void* escape(){	
	while(keypress() != SDLK_ESCAPE){
		usleep(20000);
		printf("non finish!\n");
	}	
	printf("finish!\n");
	return NULL;
}		
