//
// Created by pierre.buffo on 23.11.16.
//

#ifndef GAMEOFLIFE_THREADS_H
#define GAMEOFLIFE_THREADS_H

#include "gfx.h"
#include <pthread.h>
#include <unistd.h>
//#include "functions.h"

typedef struct display_st{
	struct gfx_context_t* gfx;
	uint frequency;
}displaySt;

typedef struct threadsData{
    int ID;
    int nbrThreads;
    struct gfx_context_t* gfx;
    // surement une barrière ou une sémaphore
} threadsData;

void* worker(void* threadData);
void* display(void* displaySt);
void* escape();


#endif //GAMEOFLIFE_THREADS_H
