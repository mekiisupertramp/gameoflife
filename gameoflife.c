//
// Created by pierre.buffo on 23.11.16.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
		
		int width = atoi(argv[1]);
		int height = atoi(argv[2]);
		int seed = atoi(argv[3]);
		double probability = atof(argv[4]);
		int frequency = atoi(argv[5]);
		int workers = atoi(argv[6]);
		
		
		
	}
}
