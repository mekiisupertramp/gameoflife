//
// Created by pierre.buffo on 23.11.16.
//

#include "Libs/header.h"


int main(int argc, char** argv){
	if(argc != 7){
		showSyntax();
	}else{
		printf("program started\n");
		
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

		sem_init(&semDisplay, 0, nbrWorkers);
		sem_init(&gfxSynchro, 0, 0);
		semWorkers = malloc(sizeof(sem_t)*nbrWorkers);
		int i;
		for (i = 0; i < nbrWorkers; ++i) {
			sem_init(&(semWorkers[i]), 0, 0);
		}
		
		// initialisation of the structure with data		
		initData(&data,0,nbrWorkers,frequency,&semDisplay, &semWorkers,&gfxSynchro,
								width, height, seed, probability);

		// display thread
		if(pthread_create(&displayer,NULL,display,&data) != 0){
			fprintf(stderr, "display pthread_create failed !\n");
			return EXIT_FAILURE;
		}

		sem_wait(&gfxSynchro);
		
		for(int i = 0; i < nbrWorkers; i++){
			data.ID = i;
			if(pthread_create(&workers[i],NULL,worker,&data) != 0){
				fprintf(stderr, "workers pthread_create failed !\n");
				return EXIT_FAILURE;
			}
			sem_wait(&gfxSynchro);
		}
		
		// escape thread
		if(pthread_create(&escaper,NULL,escape,NULL) != 0){
			fprintf(stderr, "escape pthread_create failed !\n");
			return EXIT_FAILURE;
		}
		
		// join thread escaper
		if (pthread_join(escaper,NULL) != 0) {
			perror("escape pthread_join");
			return EXIT_FAILURE;
		}
				
		// proper exit
		exitTreads(workers, nbrWorkers, &displayer);

		// join thread workers
		for(int i = 0; i < nbrWorkers; i++){
			if(pthread_join(workers[i],NULL) != 0){
				perror("workers pthread_join");
				return EXIT_FAILURE;
			}
		}
		// join thread displayer
		if (pthread_join(displayer,NULL) != 0) {
			perror("displayer pthread_join");
			return EXIT_FAILURE;
		}

		gfx_destroy(data.gfx);

		printf("program finished\n");
	}
	return 0;
}
