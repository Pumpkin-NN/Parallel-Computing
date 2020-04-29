#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sum_func(int a, int d, int n, double* result){
    int i;
    double sum = 0.0;
    int rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    for (i = rank; i < n; i += thread_count) {
        sum += (double) a + d * i;
    }

    printf("The sum is %.0f\n", sum);
    # pragma omp critical
    *result += sum;
    printf("The result is %.0f\n", *result);
}


int main(int argc, char* argv[]){
    double result = 0.0;
    int thread_count = strtol(argv[1], NULL, 10);
    int a, d, n;

    // Get input a, d and n
    a = atoi(argv[2]);
    d = atoi(argv[3]);
    n = atoi(argv[4]);

    # pragma omp parallel num_threads(thread_count)
    sum_func(a, d, n, &result);

    printf("The result is %.16f\n", result);
    return 0;
}