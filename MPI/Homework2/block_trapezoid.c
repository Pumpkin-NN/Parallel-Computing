#include <stdio.h>
#include <mpi.h>

#define BLOCK_LOW(id,p,n)   ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)  (BLOCK_LOW((id)+1,p,n)-1)

// Get input
void Get_input(int my_rank, int comm_sz, int* n_p) {

   if (my_rank == 0) {
      printf("Enter the number n (at least 10000000): \n");
      scanf("%d", n_p);
   } 
   MPI_Bcast(n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

// Pi function
double f(double x) {
   return (4.0/(1.0 + x*x));
}

// Trapezoidal Rule
double Trap(
      double left_endpt  /* in */, 
      double right_endpt /* in */, 
      int    trap_count  /* in */, 
      double base_len    /* in */) {
   double estimate, x;
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
}

int main(void) {

    int my_rank, comm_sz, n, local_n;   
    double h;
    double local_a, local_b, local_int, total_int;

    /* Let the system do what it needs to start up MPI */
    MPI_Init(NULL, NULL);
    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out how many processes are being used */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    Get_input(my_rank, comm_sz, &n);

    h = (double) 1/n;

    local_a = BLOCK_LOW(my_rank, comm_sz, n);
    local_b = BLOCK_HIGH(my_rank, comm_sz, n);

    local_int = Trap(local_a, local_b, n, h);

    /* Add up the integrals calculated by each process */
    MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0,
            MPI_COMM_WORLD);

    /* Print the result */
    if (my_rank == 0) {
        printf("With n = %d trapezoids, The estimate of the integral from 0 to 1 = %.15e\n", n, total_int);
    }

    /* Shut down MPI */
    MPI_Finalize();

    return 0;
} /*  main  */
