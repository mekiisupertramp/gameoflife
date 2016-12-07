/*==========================================================================================
  ==========================================================================================
	File : gameoflife.c

   Descritpion: The programm run the game of life, which is a cellulare automaton. 
	
	Authors : Mehmed Blazevic, Buffo Pierre, Da Silva Marques Gabriel
	
	Date : December 2016

   Version: 1.0.0

==========================================================================================*/

#define _GNU_SOURCE
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <pthread.h>
#include "Libs/gfx.h"
#include "Libs/threads.h"
#include "Libs/functions.h"
#include "Libs/threads.h"

int main(int argc, char** argv){
	if(argc != 7){
		showSyntax();
	}else{
		// the variables declaration
		sem_t semDisplay;
		sem_t* semWorkers;
		sem_t gfxSynchro;

		uint width = atoi(argv[1]);
		uint height = atoi(argv[2]);
		double seed = atof(argv[3]);
		double probability = atof(argv[4]);
		uint frequency = atoi(argv[5]);
		uint nbrWorkers = atoi(argv[6]);
		
		thData data;
		pthread_t displayer;
		pthread_t workers[nbrWorkers];					
		pthread_t escaper;
		
		// init semaphores
		sem_init(&semDisplay, 0, nbrWorkers);
		sem_init(&gfxSynchro, 0, 0);
		semWorkers = malloc(sizeof(sem_t)*nbrWorkers);
		for (int i = 0; i < nbrWorkers; ++i) {
			sem_init(&(semWorkers[i]), 1, 0);
		}
				
		// initialisation of the structure with datas		
		initData(&data,nbrWorkers,frequency,&semDisplay, &semWorkers,&gfxSynchro,
																			width, height, seed, probability);

		// create display thread
		if(pthread_create(&displayer,NULL,display,&data) != 0){
			fprintf(stderr, "display pthread_create failed !\n");
			return EXIT_FAILURE;
		}

		// wait for creating the gfx context in display thread 
		sem_wait(&gfxSynchro);
		
		// create the workers threads 
		for(int i = 0; i < nbrWorkers; i++){
			data.ID = i;
			if(pthread_create(&workers[i],NULL,worker,&data) != 0){
				fprintf(stderr, "workers pthread_create failed !\n");
				return EXIT_FAILURE;
			}
			// wait for the id init 
			sem_wait(&gfxSynchro);
		}
		
		// create escape thread
		if(pthread_create(&escaper,NULL,escape,&data) != 0){
			fprintf(stderr, "escape pthread_create failed !\n");
			return EXIT_FAILURE;
		}
		
		// join thread escaper
		if (pthread_join(escaper,NULL) != 0) {
			perror("escape pthread_join");
			return EXIT_FAILURE;
		}
				
		// cancel workers
		for (int i = 0; i < nbrWorkers; i++){
			pthread_cancel(workers[i]);
		}

		// join thread workers
		for(int i = 0; i < nbrWorkers; i++){
			if(pthread_join(workers[i],NULL) != 0){
				perror("workers pthread_join");
				return EXIT_FAILURE;
			}
			// post display semaphore for display thread 
			sem_post(data.semDisplay);
		}
		
		// join thread displayer
		if (pthread_join(displayer,NULL) != 0) {
			perror("displayer pthread_join");
			return EXIT_FAILURE;
		}
		
		// finish the programm properly
		sem_destroy(&semDisplay);
		for(int i=0 ; i <nbrWorkers ; i++) sem_destroy(&semWorkers[i]);
		free(semWorkers);
		sem_destroy(&gfxSynchro);
	}
	return 0;
}
