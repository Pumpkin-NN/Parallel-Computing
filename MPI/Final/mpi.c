/***********************************************************
* Program: 
    Final Project: Pi estimation
*************************************************************/
#include <mpi.h>  
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long long toss_darts (long long tosses, int rank){
	long long toss, number_in_circle = 0;        
	double x,y;
	unsigned int seed = (unsigned) time(NULL);
	srand(seed + rank);
	for (toss = 0; toss < tosses; toss++) {
	   x = rand_r(&seed)/(double)RAND_MAX;
	   y = rand_r(&seed)/(double)RAND_MAX;
	   if( (x*x+y*y) <= 1.0 ){
           number_in_circle++;
        }
    }
    return number_in_circle;
}

int main(int argc, char *argv[]) {
   int rank, nprocs;
   long long number_of_tosses, number_of_toss, number_in_circle, circles;
   double pi_estimate;
   double PI25DT = 3.141592653589793238462643;
   
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
   // Get input from the command line
   if (argc!= 2){
        printf("usage: mpirun -np <number_of_processes> ./ProgramName <number_of_tosses> \n");
        MPI_Finalize();
        exit(-1);
    } else {
        number_of_tosses = atoi(argv[1]);
    }

   number_of_toss = number_of_tosses/nprocs; 
   
   MPI_Barrier(MPI_COMM_WORLD);

   number_in_circle = toss_darts(number_of_toss, rank);
 
   MPI_Reduce(&number_in_circle, &circles, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
   
   if (rank == 0) {
	   pi_estimate = (4*circles)/((double) number_of_tosses);
	   printf("Pi is approximately %.16f, Error is %.16f\n", pi_estimate, fabs(pi_estimate - PI25DT));
   }
   MPI_Finalize(); 
   return 0;

}



