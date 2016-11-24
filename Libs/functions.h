//
// Created by pierre.buffo on 23.11.16.
//

#ifndef GAMEOFLIFE_FUNCTIONS_H
#define GAMEOFLIFE_FUNCTIONS_H
#include "gfx.h"
#include "threads.h" 

typedef struct display_st{
	struct gfx_context_t* gfx;
	uint frequency;
}displaySt;

void initGfx(struct gfx_context_t* gfx, int seed, double probability);
void initStruct(threadsData* thData, uint id, struct gfx_context_t* gfx);
double myRand();
void showSyntax();

#endif //GAMEOFLIFE_FUNCTIONS_H
