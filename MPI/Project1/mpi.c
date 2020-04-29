#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

// Marcos
#define MASTER 0      /* rank of the master */
#define BLOCK_LOW(id,p,n)   ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)  (BLOCK_LOW((id)+1,p,n)-1)

int main(int argc, char *argv[]) {

    int rank;                                           /* rank variable to identify the process */
    int nprocs;                                         /* number of processes */
    int i;
    int a;
    int d;
    int n;

    int start_term;
    int end_term;

    double my_value,                                        /* local value */
           value,                                          /* total value */
           sum;                                         /* sum of area under the curve */

    a = atoi(argv[1]);
    d = atoi(argv[2]);
    n = atoi(argv[3]);
    
    /*Initialize MPI execution environment */
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    start_term = BLOCK_LOW(rank, nprocs, n);
    end_term = BLOCK_HIGH(rank, nprocs, n);

    /* Calculating for each process */
    sum = 0.0;
    for (i = start_term; i <= end_term; i++) {
        sum += (double) a + d * i;
    }

    my_value = sum;

    MPI_Reduce(&my_value, &value, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("The result is %.16f\n", value);
    }

    /* Terminate MPI execution environment */
    MPI_Finalize(); 
    return 0;
}
