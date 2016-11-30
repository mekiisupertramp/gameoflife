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
		
		// display thread
		if(pthread_create(&displayer,NULL,display,&data[0]) != 0){
			fprintf(stderr, "display pthread_create failed !\n");
			return EXIT_FAILURE;
		}										
		
		usleep(10000000);
		

		// workers thread
		for(int i = 0; i < nbrWorkers; i++){
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

		gfx_destroy(&gfx);

		printf("finish programm\n");
	}
	return 0;
}
