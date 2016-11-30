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
<<<<<<< HEAD

typedef struct thData{
	int ID;
	int nbrThreads;
=======
/*
typedef struct display_st{
>>>>>>> 2b5b543f5f8929021414bcffd4da37e7118c272f
	struct gfx_context_t* gfx;
	int frequency;
	int *nbrWorkers;
	sem_t* semDisplay;
	sem_t** semWorkers;
<<<<<<< HEAD
	sem_t* gfxSynchro;
=======
}displaySt;**/
/*
typedef struct threadsData{
    int ID;
    int nbrThreads;
    struct gfx_context_t* gfx;
    // surement une barrière ou une sémaphore
	sem_t* semDisplay;
	sem_t** semWorkers;
} threadsData;
*/
typedef struct thData{
	int ID;
	int nbrThreads;
	struct gfx_context_t* gfx;
	int frequency;
	int *nbrWorkers;
	sem_t* semDisplay;
	sem_t** semWorkers;
>>>>>>> 2b5b543f5f8929021414bcffd4da37e7118c272f
	int width;
	int height;
	double seed;
	double probability;
}thData;

void* worker(void* threadData);
<<<<<<< HEAD
void* escape();
void* display(void* displaySt);
=======
void* display(void* displaySt);
void* escape();
>>>>>>> 2b5b543f5f8929021414bcffd4da37e7118c272f

void lifeIsSad(int cellToTest, struct gfx_context_t* gfx);
uint32_t isAlive(int x, int y, struct gfx_context_t* gfx);
int countNeighboursAlive(int x, int y, struct gfx_context_t* gfx);
void swapPixel(struct gfx_context_t* gfx);
<<<<<<< HEAD
void initGfx(struct gfx_context_t* gfx, double seed, double probability);

=======
void initGfx(struct gfx_context_t* gfx, int seed, double probability);
>>>>>>> 2b5b543f5f8929021414bcffd4da37e7118c272f

#endif //GAMEOFLIFE_THREADS_H
