//
// Created by pierre.buffo on 23.11.16.
//

#include "Libs/header.h"
int main(int argc, char** argv){
    if(argc != 7){
		printf("syntax : gameoflife <width> <height> <seed> <p> <freq> <#workers>\n");
		printf("\t- <width> and <height> are integers specifying the dimensions of\n\
	  the game (>= 4).\n");
		printf("\t- <seed> is an integer used to randomly populate the board.\n");
		printf("\t- <p> is a floating point value (range [0..1]) which is the\n\
  	  probability of having a live cell during initialization.\n");
		printf("\t- <freq> is an integer specifying the display frequency in Hz(> 0).\n");
		printf("\t- <#workers> is an integer specifying the number of workerthreads (>= 1).\n");
	}else{
		
		uint width = atoi(argv[1]);
		uint height = atoi(argv[2]);
		int seed = atoi(argv[3]);
		double probability = atof(argv[4]);
		uint frequency = atoi(argv[5]);
		uint nbrWorkers = atoi(argv[6]);
		threadsData thData[nbrWorkers];
		uint scope = (width*height)/nbrWorkers; // à refaire
		
		pthread_t displayer;
		pthread_t workers[nbrWorkers];
		pthread_t escaper;
		struct gfx_context_t* gfx = gfx_create("Game of life bitches", width, height);
		
		initGfx(gfx,seed,probability);
		
		displaySt displayVar;
		displayVar.gfx = gfx;
		displayVar.frequency = frequency;
		
		// workers thread
		for(int i = 0; i < nbrWorkers; i++){
			initStruct(&thData[i], i, gfx, scope);
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
		
		if (pthread_join(escaper,NULL) != 0) {
			perror("escape pthread_join");
			return EXIT_FAILURE;
		}
		
	}
	return 0;
}
