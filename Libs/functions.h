//
// Created by pierre.buffo on 23.11.16.
//

#ifndef GAMEOFLIFE_FUNCTIONS_H
#define GAMEOFLIFE_FUNCTIONS_H
#include "gfx.h"
#include "threads.h" 

void initData(thData* data, int ID, int nbrThreads, int frequency, 
						sem_t* display, sem_t** workers, sem_t* gfxSynchro, int width, int height, 
																				double seed, double probability);
void exitTreads(pthread_t* workers, int nbrWorkers, pthread_t* displayer);
void showSyntax();

#endif //GAMEOFLIFE_FUNCTIONS_H
