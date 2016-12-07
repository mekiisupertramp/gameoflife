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
#include <time.h>

#define ALIVE  COLOR_YELLOW
#define DEAD  COLOR_BLACK

typedef struct thData{
	int ID;
	int nbrThreads;
	struct gfx_context_t* gfx;
	int frequency;
	sem_t* semDisplay;
	sem_t** semWorkers;
	sem_t* gfxSynchro;
	int width;
	int height;
	double seed;
	double probability;
}thData;

void* worker(void* threadData);
void* escape(void* data);
void* display(void* displaySt);

void lifeIsSad(int cellToTest, struct gfx_context_t* gfx);
uint32_t isAlive(int x, int y, struct gfx_context_t* gfx);
int countNeighboursAlive(int x, int y, struct gfx_context_t* gfx);
void swapPixel(struct gfx_context_t* gfx);
void initGfx(struct gfx_context_t* gfx, double seed, double probability);
double waitAMoment(struct timespec* start, struct timespec* finish, int frequency);

#endif //GAMEOFLIFE_THREADS_H
