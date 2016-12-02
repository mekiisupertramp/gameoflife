/*==========================================================================================
  ==========================================================================================
	File : threads.h

   Descritpion: header of threads functions 
	
	Authors : Mehmed Blazevic, Buffo Pierre, Da Silva Marques Gabriel
	
	Date : December 2016

   Version: 1.0.0

==========================================================================================*/

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
#define FINISH	1
#define NONFINISH 0

typedef struct thData{
	int ID;
	int nbrThreads;	
	int width;
	int height;
	double seed;
	double probability;
	int frequency;	
	struct gfx_context_t* gfx;	
	
	int finish;
	pthread_mutex_t* m;
	
	sem_t* semDisplay;
	sem_t** semWorkers;
	sem_t* gfxSynchro;
}thData;

void* worker(void* threadData);
void* escape(void* data);
void* display(void* displaySt);

void lifeIsSad(int cellToTest, struct gfx_context_t* gfx);
uint32_t isAlive(int x, int y, struct gfx_context_t* gfx);
int countNeighboursAlive(int x, int y, struct gfx_context_t* gfx);
double waitAMoment(struct timespec* start, struct timespec* finish, int frequency);
void swapPixel(struct gfx_context_t* gfx);
void initGfx(struct gfx_context_t* gfx, double seed, double probability);


#endif //GAMEOFLIFE_THREADS_H
