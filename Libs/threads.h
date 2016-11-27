//
// Created by pierre.buffo on 23.11.16.
//

#ifndef GAMEOFLIFE_THREADS_H
#define GAMEOFLIFE_THREADS_H

#define _GNU_SOURCE
#include <semaphore.h>
#include "gfx.h"
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define ALIVE  COLOR_WHITE
#define DEAD  COLOR_BLACK

typedef struct display_st{
	struct gfx_context_t* gfx;
	uint frequency;
    uint *nbrWorkers;
	sem_t* semDisplay;
	sem_t* semWorkers;
}displaySt;

typedef struct threadsData{
    int ID;
    int nbrThreads;
    struct gfx_context_t* gfx;
    // surement une barrière ou une sémaphore
	sem_t* semDisplay;
	sem_t* semWorkers;
} threadsData;

void* worker(void* threadData);
void lifeIsSad(int cellToTest, struct gfx_context_t* gfx);
uint32_t isAlive(int x, int y, struct gfx_context_t* gfx);
int countNeighboursAlive(int x, int y, struct gfx_context_t* gfx);
void* display(void* displaySt);
void swapPixel(struct gfx_context_t* gfx);
void* escape();


#endif //GAMEOFLIFE_THREADS_H
