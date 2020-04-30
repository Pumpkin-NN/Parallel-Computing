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

// Marcos
#define MASTER 0      /* rank of the master */
#define BLOCK_LOW(id,p,n)   ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)  (BLOCK_LOW((id)+1,p,n)-1)

int main(int argc, char *argv[]) {
    int rank, nprocs;
    long long i, number_of_tosses, number_in_circle = 0, circles;
    double pi_estimate;
    double x, y;
    double PI25DT = 3.141592653589793238462643;

    /* Start up MPI */
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Get input <number_of_tosses> from the command line */
    if (argc!= 2){
        printf("usage: mpirun -np <number_of_processes> ./ProgramName <number_of_tosses> \n");
        MPI_Finalize();
        exit(-1);
    } else {
        number_of_tosses = atoi(argv[1]);
    }

    /* Random number generator */
    unsigned int seed = (unsigned) time(NULL);
	srand(seed + rank);

    /* Use Block Partitioning */
    for (i = BLOCK_LOW(rank, nprocs, number_of_tosses); i <= BLOCK_HIGH(rank, nprocs, number_of_tosses); i++) {
	   x = rand_r(&seed)/(double)RAND_MAX;
	   y = rand_r(&seed)/(double)RAND_MAX;
	   if( (x*x+y*y) <= 1.0 ){
           number_in_circle++;
        }
    }

    /* Add up the integrals calculated by each process */
    MPI_Reduce(&number_in_circle, &circles, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Print the result */
    if (rank == 0) {
        pi_estimate = (4*circles)/((double) number_of_tosses);
        printf("Pi is approximately %.16f, Error is %.16f\n", pi_estimate, fabs(pi_estimate - PI25DT));
    }
    /* Shut down MPI */
    MPI_Finalize(); 
    return 0;
}/*  main  */



