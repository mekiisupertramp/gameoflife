/*==========================================================================================
  ==========================================================================================
	Fichier : timing.c

   Descritpion: Allows the user to count the time in secondes, between a start
					and a stop. 
	
	Authors : Mehmed Blazevic, Buffo Pierre, Da Silva Gabriel
	
	Date : November 2016

   Version: 1.0.0

=*========================================================================================*/
#include "timing.h"

// -- globales variables 
struct timespec start, finish;

/*----------------------------------------------------------------------------------------*-
   startTime()
  ------------------------------------------------------------------------------------------
   Description : get the first clock time 
   In	 : --
   Out : --
-*----------------------------------------------------------------------------------------*/
void startTime(){
	clock_gettime(CLOCK_MONOTONIC, &start);
}
/*----------------------------------------------------------------------------------------*-
   stopTime()
  ------------------------------------------------------------------------------------------
   Description : get the second clock time 
   In	 : --
   Out : --
-*----------------------------------------------------------------------------------------*/
void stopTime(){
	clock_gettime(CLOCK_MONOTONIC, &finish);
}
/*----------------------------------------------------------------------------------------*-
   getCntTime()
  ------------------------------------------------------------------------------------------
   Description : get the difference between the first and the second clock time
						in seconds
   In	 : --
   Out : the time between start and stop time 
-*----------------------------------------------------------------------------------------*/
double getCntTime(){
	double elapsed = finish.tv_sec - start.tv_sec;
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	return elapsed;
}
