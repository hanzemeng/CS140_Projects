#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>

#define ull unsigned long long int 

double rand01();
double dist_to_origin(double x, double y);
double elapsed_seconds();

int main(int argc, char** argv)
{
    //setting up MPI and broadcasting parameter
    ull sample_size;
    int p_size, my_rank;
    double begin;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &p_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if(0 == my_rank)
    {
        if(2 != argc)
        {
            printf("Wrong number of arguments\n");
            exit(1);
        }
        sample_size = strtoull(argv[1], NULL, 10);
    }
    MPI_Bcast(&sample_size, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    begin = elapsed_seconds();
    // sampling
    unsigned int seed = time(NULL)*(my_rank+1);
    ull in_circle_count = 0;
    for(ull i=0; i<sample_size; i++)
    {
        double x = rand01(&seed);
        double y = rand01(&seed);
        if(dist_to_origin(x, y) <= (double)1)
        {
            in_circle_count++;
        }
    }
    // sending result to core 0 and printing the final result
    if(0 == my_rank)
    {
        ull total_in_circle_count = in_circle_count;
        for(int i=1; i<p_size; i++)
        {
            ull temp;
            MPI_Recv(&temp, 1, MPI_UNSIGNED_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_in_circle_count += temp;
        }
        double pi_estimate = (double) 4 * (double) total_in_circle_count / (double) p_size / (double) sample_size;
        double accuracy = fabs((M_PI-pi_estimate)/M_PI);
        double end = elapsed_seconds();
        printf("Estimate of pi: %f\n", pi_estimate);
        printf("Accuracy of estimation: %e\n", accuracy);
        printf("Time taken: %f\n", end-begin);
    }
    else
    {
        MPI_Send(&in_circle_count, 1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}

double rand01(unsigned int* seed)
{
    return ((double) rand_r(seed)) / ((double) RAND_MAX);
}
double dist_to_origin(double x, double y)
{
    return sqrt(x*x + y*y);
}

double elapsed_seconds()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (double)tv.tv_sec + (double)tv.tv_usec/1000000.0;
}