/***********************************************************
* Program: 
    Final Project: Pi estimation
*************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/*------------------------------------------------------------------
 * Function:     toss_darts
 * Purpose:      toss darts randomly to find out the number in circles.
 * Input args:   total_tosses:  number of tosses
 * Output args:  circles:  the total number of circles
 */
long long toss_darts (long long total_tosses, long long* circles){
	long long i, number_in_circle = 0;        
	double x, y;
    int rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

	unsigned int seed = (unsigned) time(NULL);
	srand(seed + rank);

	for (i = rank; i <= total_tosses; i += thread_count) {
	   x = rand_r(&seed)/(double)RAND_MAX;
	   y = rand_r(&seed)/(double)RAND_MAX;
	   if( (x*x+y*y) <= 1.0 ){
           number_in_circle++;
        }
    }

    # pragma omp critical
    *circles += number_in_circle;
}

int main(int argc, char* argv[]){
    int thread_count = strtol(argv[1], NULL, 10);
    long long number_of_tosses, circles;
    double pi_estimate;
    double PI25DT = 3.141592653589793238462643;

   /* Get input from the command line */
   if (argc != 3){
        printf("usage: ./ open_mpi <thread_count> <number_of_tosses> \n");
        exit(-1);
    } else {
        number_of_tosses = atoi(argv[2]);
    }

    # pragma omp parallel num_threads(thread_count)
    toss_darts(number_of_tosses, &circles);

    /* Estimate PI using the formula*/
    pi_estimate = (4*circles)/((double) number_of_tosses);
	printf("Pi is approximately %.16f, Error is %.16f\n", pi_estimate, fabs(pi_estimate - PI25DT));
    return 0;
}/*  main  */