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
	// gestion barrier
	gfx_present(struct gfx_context_t *ctxt);
	// gestion barrier
	return NULL;
}

void* escape(){
	return NULL;
}
