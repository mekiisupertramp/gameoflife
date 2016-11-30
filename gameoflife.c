//
// Created by pierre.buffo on 23.11.16.
//

#include "Libs/header.h"


int main(int argc, char** argv){
	if(argc != 7){
		showSyntax();
	}else{
		sem_t semDisplay;
		sem_t* semWorkers;
<<<<<<< HEAD
		sem_t gfxSynchro;

		uint width = atoi(argv[1]);
		uint height = atoi(argv[2]);
		double seed = atof(argv[3]);
		double probability = atof(argv[4]);
		uint frequency = atoi(argv[5]);
		uint nbrWorkers = atoi(argv[6]);
		thData data[nbrWorkers];

		sem_init(&semDisplay, 0, nbrWorkers);
		sem_init(&gfxSynchro, 0, 0);
		semWorkers = malloc(sizeof(sem_t)*nbrWorkers);
		int i;
		for (i = 0; i < nbrWorkers; ++i) {
			sem_init(&(semWorkers[i]), 1, 0);
		}

		pthread_t displayer;
		pthread_t workers[nbrWorkers];					
		pthread_t escaper;

		
		// initialisation of the structures with data		
		for(int i=0 ; i<nbrWorkers ; i++){
			initData(&data[i],i,nbrWorkers,frequency,&semDisplay,
															&semWorkers,&gfxSynchro, width, height, seed, 
																										probability);
		}

=======

		int width = atoi(argv[1]);
		int height = atoi(argv[2]);
		double seed = atoi(argv[3]);
		double probability = atof(argv[4]);
		int frequency = atoi(argv[5]);
		int nbrWorkers = atoi(argv[6]);
	
		thData data[nbrWorkers];

		sem_init(&semDisplay, 1, nbrWorkers);
		semWorkers = malloc(sizeof(sem_t)*nbrWorkers);
		int i;
		for (i = 0; i < nbrWorkers; ++i) {
			sem_init(&(semWorkers[i]), 1, 0);
		}

		pthread_t displayer;
		pthread_t workers[nbrWorkers];					
		pthread_t escaper;
		struct gfx_context_t gfx;
		
		// init datas	
		for(int i=0; i<nbrWorkers; i++){
			initData(&data[i],&gfx,i,nbrWorkers,frequency,&nbrWorkers,&semDisplay,&semWorkers, width, height, seed, probability);
		}
		
>>>>>>> 2b5b543f5f8929021414bcffd4da37e7118c272f
		// display thread
		if(pthread_create(&displayer,NULL,display,&data[0]) != 0){
			fprintf(stderr, "display pthread_create failed !\n");
			return EXIT_FAILURE;
<<<<<<< HEAD
		}

		sem_wait(&gfxSynchro);
		
		for(int i = 0; i < nbrWorkers; i++){
			data[i].gfx = data[0].gfx;
=======
		}										
		
		usleep(10000000);
		

		// workers thread
		for(int i = 0; i < nbrWorkers; i++){
>>>>>>> 2b5b543f5f8929021414bcffd4da37e7118c272f
			if(pthread_create(&workers[i],NULL,worker,&data[i]) != 0){
				fprintf(stderr, "workers pthread_create failed !\n");
				return EXIT_FAILURE;
			}
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

<<<<<<< HEAD
//		gfx_destroy(gfx);
=======
		gfx_destroy(&gfx);
>>>>>>> 2b5b543f5f8929021414bcffd4da37e7118c272f

		printf("finish programm\n");
	}
	return 0;
}
