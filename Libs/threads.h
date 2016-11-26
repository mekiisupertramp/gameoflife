//
// Created by pierre.buffo on 23.11.16.
//

#ifndef GAMEOFLIFE_THREADS_H
#define GAMEOFLIFE_THREADS_H

#include "gfx.h"
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define ALIVE  COLOR_WHITE
#define DEAD  COLOR_BLACK

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
void lifeIsSad(int cellToTest, struct gfx_context_t* gfx);
bool isAlive(int line, int col, struct gfx_context_t* gfx);
int countNeighboursAlive(int line, int col, struct gfx_context_t* gfx);
void* display(void* displaySt);
void swapPixel(struct gfx_context_t* gfx);
void* escape();


#endif //GAMEOFLIFE_THREADS_H
