//
// Created by pierre.buffo on 23.11.16.
//

#include "functions.h"

void initGfx(struct gfx_context_t* gfx, int seed, double probability){
	
}

void initStruct(threadsData* thData, uint id, struct gfx_context_t* gfx, uint scope){
	thData->ID = id;
	thData->gfx = gfx;
	thData->start = id*scope; // à refaire
	thData->start = id*scope; // à refaire
	thData->stop = id*scope;  // à refaire
}
