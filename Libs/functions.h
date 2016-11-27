//
// Created by pierre.buffo on 23.11.16.
//

#ifndef GAMEOFLIFE_FUNCTIONS_H
#define GAMEOFLIFE_FUNCTIONS_H
#include "gfx.h"
#include "threads.h" 

void initGfx(struct gfx_context_t* gfx, int seed, double probability);
void initDisplayStruct(displaySt* displayVar, struct gfx_context_t* gfx, int frequency, int *nbrWorkers, sem_t* semDisplay, sem_t* semWorkers);
void initWorkersStruct(threadsData* thData, int id, struct gfx_context_t* gfx, int nbrThreads, sem_t* semDisplay, sem_t* semWorkers);
void exitTreads(pthread_t* workers, int nbrWorkers, pthread_t* displayer);
void showSyntax();

#endif //GAMEOFLIFE_FUNCTIONS_H
