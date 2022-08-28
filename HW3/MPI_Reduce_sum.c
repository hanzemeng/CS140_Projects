#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <mpi.h>

double elapsed_seconds();
void test_result(double* vector_sum, int vector_count, int vector_size);

int main(int argc, char** argv)
{
    // setting up MPI and broadcasting parameters
    int vector_count, vector_size;
    int p_size, my_rank;
    int start_i, end_i;
    double begin;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &p_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if(0 == my_rank)
    {
        if(3 != argc)
        {
            printf("Wrong number of arguments\n");
            exit(1);
        }
        vector_count = strtol(argv[1], NULL, 10);
        vector_size = strtol(argv[2], NULL, 10);
    }
    MPI_Bcast(&vector_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&vector_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // distributing vectors to add
    if(my_rank < vector_count%p_size)
    {
        int temp = vector_count/p_size;
        start_i = my_rank * temp + my_rank;
        end_i = start_i + temp + 1;
    }
    else
    {
        int temp = vector_count/p_size;
        start_i = my_rank * temp + vector_count%p_size;
        end_i = start_i + temp;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    begin = elapsed_seconds();
    // adding vectors
    double* vector_sum;
    vector_sum = (double*) malloc(vector_size*sizeof(double));
    memset(vector_sum, 0 ,vector_size*sizeof(double));
    for(int i=start_i; i<end_i; i++)
    {
        for(int j=0; j<vector_size; j++)
        {
            vector_sum[j] += (double)i*(double)vector_size + (double)j;
        }
    }
    // sending results to core 0
    double* res_sum;
    res_sum = (double*) malloc(vector_size*sizeof(double));
    MPI_Reduce(vector_sum, res_sum, vector_size, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    // testing and printing out results
    if(0 == my_rank)
    {
        double end = elapsed_seconds();
        printf("Time taken: %f\n", end-begin);
        test_result(res_sum, vector_count, vector_size); // test result
    }

    free(res_sum);
    free(vector_sum);
    MPI_Finalize();
    return 0;
}

double elapsed_seconds()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (double)tv.tv_sec + (double)tv.tv_usec/1000000.0;
}

void test_result(double* vector_sum, int vector_count, int vector_size)
{
    double res = (double)vector_size*(double)vector_count*(double)(vector_count-1)/(double)2;
    for(int i=0; i<vector_size; i++)
    {
        double expected = res+(double)i*(double)vector_count;
        if(vector_sum[i] != expected)
        {
            printf("Wrong value at %d\n", i);
            printf("Expected: %.0f\n", expected);
            printf("Actual: %.0f\n", vector_sum[i]);
            exit(1);
        }
    }
    int iterSize = vector_size<30 ? vector_size : 30;
    for(int i=0; i<iterSize; i++)
    {
        printf("%.0f, ", vector_sum[i]);
    }
    printf("\n");
}