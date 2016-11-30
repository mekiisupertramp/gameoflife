//
// Created by pierre.buffo on 23.11.16.
//

#ifndef GAMEOFLIFE_FUNCTIONS_H
#define GAMEOFLIFE_FUNCTIONS_H
#include "gfx.h"
#include "threads.h" 

void initData(thData* data, struct gfx_context_t* gfx, int ID, int nbrThreads, int frequency, 
					 sem_t* display, int width, int height, double seed, double probability);


void exitTreads(pthread_t* workers, int nbrWorkers, pthread_t* displayer);
void showSyntax();




// a supprimer

/*
void initDisplayStruct(displaySt* displayVar, struct gfx_context_t* gfx, int frequency, uint *nbrWorkers, sem_t* semDisplay, sem_t** semWorkers);
void initWorkersStruct(threadsData* thData, int id, struct gfx_context_t* gfx, int nbrThreads, sem_t* semDisplay, sem_t** semWorkers);
*/

#endif //GAMEOFLIFE_FUNCTIONS_H
