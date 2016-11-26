//
// Created by pierre.buffo on 23.11.16.
//

#include "Libs/header.h"

int main(int argc, char** argv){
    if(argc != 7){
		showSyntax();
	}else{
		
	uint width = atoi(argv[1]);
		uint height = atoi(argv[2]);
		int seed = atoi(argv[3]);
		double probability = atof(argv[4]);
		uint frequency = atoi(argv[5]);
		uint nbrWorkers = atoi(argv[6]);
		threadsData thData[nbrWorkers];		
		
		pthread_t displayer;
		pthread_t workers[nbrWorkers];					
		pthread_t escaper;
		displaySt displayVar;
		struct gfx_context_t* gfx = gfx_create("Game of life bitches", width, height);
		
		// initialisation of the structures with data
		initGfx(gfx,seed,probability);
		initDisplayStruct(&displayVar, gfx, frequency);
		
		// workers thread
		for(int i = 0; i < nbrWorkers; i++){
			initWorkersStruct(&thData[i], i, gfx, nbrWorkers);
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
		
		// join thread
		if (pthread_join(escaper,NULL) != 0) {
			perror("escape pthread_join");
			return EXIT_FAILURE;
		}
		
		// proper exit
		//exitTreads(workers, nbrWorkers, &displayer);
		
		gfx_destroy(gfx);
		//free(thData);
	}
	return 0;
}
