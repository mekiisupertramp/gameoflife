//
// Created by pierre.buffo on 23.11.16.
//

#ifndef GAMEOFLIFE_THREADS_H
#define GAMEOFLIFE_THREADS_H

#include "gfx.h"

typedef struct threadsData{
    int ID;
    int* lastState;
    int* nextState;
    struct gfx_context_t* gfx;
    int start,stop;
} threadsData;

void* worker(void* threadData);

#endif //GAMEOFLIFE_THREADS_H
