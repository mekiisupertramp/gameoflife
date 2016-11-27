//
// Created by pierre.buffo on 23.11.16.
//

#include "Libs/header.h"


int main(int argc, char** argv){
    if(argc != 7){
		showSyntax();
	}else{
		sem_t semDisplay;
		sem_t semWorkers;


		uint width = atoi(argv[1]);
		uint height = atoi(argv[2]);
		int seed = atoi(argv[3]);
		double probability = atof(argv[4]);
		uint frequency = atoi(argv[5]);
		uint nbrWorkers = atoi(argv[6]);
		threadsData thData[nbrWorkers];

		sem_init(&semDisplay, 1, nbrWorkers);
		sem_init(&semWorkers, 1, 0);
		pthread_t displayer;
		pthread_t workers[nbrWorkers];					
		pthread_t escaper;
		displaySt displayVar;
		struct gfx_context_t* gfx = gfx_create("Game of life bitches", width, height);
		
		// initialisation of the structures with data
		initGfx(gfx,seed,probability);
		initDisplayStruct(&displayVar, gfx, frequency, &nbrWorkers, &semDisplay, &semWorkers);

		// initialisation of the semaphores
/*		sem_t mutex1;
		sem_init(&mutex1, 0, 0);
		displayVar.sem1 = &mutex1;
		thData[0].sem1 = &mutex1;
		sem_t mutex2;
		sem_init(&mutex2, 0, 1);
		displayVar.sem2 = &mutex2;
		thData[0].sem2 = &mutex2;		*/
		
		// workers thread
		for(int i = 0; i < nbrWorkers; i++){
			initWorkersStruct(&thData[i], i, gfx, nbrWorkers, &semDisplay, &semWorkers);
			if(pthread_create(&workers[i],NULL,worker,&thData[i]) != 0){
				fprintf(stderr, "workers pthread_create failed !\n");
				return EXIT_FAILURE;
			}
		}	
		// display thread
		if(pthread_create(&displayer,NULL,display,&displayVar) != 0){
			fprintf(stderr, "display pthread_create failed !\n");
			return EXIT_FAILURE;
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
		



		printf("finish programm\n");
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
		
		// ici, il faut attendre que les threads soient killés(workers & displayer), avant de destroy gfx 
		// sinon, je sais pas ce qui se passe, mais le programme refuse de s'arrêter (BLAZEVIC)
		gfx_destroy(gfx);
	}
	return 0;
}
