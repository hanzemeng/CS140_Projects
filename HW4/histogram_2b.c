/* File:      histogram.c
 * Purpose:   Build a histogram from some random data
 * 
 * Compile:   gcc -g -Wall -o histogram histogram.c
 * Run:       ./histogram <bin_count> <min_meas> <max_meas> <data_count>
 *
 * Input:     None
 * Output:    A histogram with X's showing the number of measurements
 *            in each bin
 *
 * Notes:
 * 1.  Actual measurements y are in the range min_meas <= y < max_meas
 * 2.  bin_counts[i] stores the number of measurements x in the range
 * 3.  bin_maxes[i-1] <= x < bin_maxes[i] (bin_maxes[-1] = min_meas)
 * 4.  DEBUG compile flag gives verbose output
 * 5.  The program will terminate if either the number of command line
 *     arguments is incorrect or if the search for a bin for a 
 *     measurement fails.
 *
 * IPP:  Section 2.7.1 (pp. 66 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

void Usage(char prog_name[]);
double elapsed_seconds();

void Get_args(
      char*    argv[]        /* in  */,
      int*     bin_count_p   /* out */,
      float*   min_meas_p    /* out */,
      float*   max_meas_p    /* out */,
      int*     data_count_p  /* out */,
      int*     thread_count  /* out */);

void Gen_bins(
      float min_meas      /* in  */, 
      float max_meas      /* in  */, 
      float bin_maxes[]   /* out */, 
      int   bin_counts[]  /* out */, 
      int   bin_count     /* in  */);

int Which_bin(
      float    data         /* in */, 
      float    bin_maxes[]  /* in */, 
      int      bin_count    /* in */, 
      float    min_meas     /* in */);

void Print_histo(
      float    bin_maxes[]   /* in */, 
      int      bin_counts[]  /* in */, 
      int      bin_count     /* in */, 
      float    min_meas      /* in */);


int bin_count;
float min_meas, max_meas;
float* bin_maxes;
int* bin_counts;
int data_count;
int thread_count;
pthread_mutex_t* bin_mutexes;
int sampled_data_count;
pthread_barrier_t barrier;

void* gen_and_assign(void* seed)
{
   pthread_barrier_wait(&barrier);
   for(int i=0; i<data_count; i++)
   {
      float data_val = min_meas + (max_meas - min_meas)*rand_r(seed)/((float) RAND_MAX);
      if(data_val>=max_meas) // max_meas doesn't belong in the histogram
      {
         i--;
         continue;
      }
      int data_index = Which_bin(data_val, bin_maxes, bin_count, min_meas);

      pthread_mutex_lock(&bin_mutexes[data_index]);
      bin_counts[data_index]++;
      pthread_mutex_unlock(&bin_mutexes[data_index]);
   }
   return NULL;
}


int main(int argc, char* argv[]) {
   /* Check and get command line args */
   if (argc != 6) Usage(argv[0]); 
   Get_args(argv, &bin_count, &min_meas, &max_meas, &data_count, &thread_count);
   sampled_data_count = 0;
   data_count /= thread_count;

   /* Allocate arrays needed */
   bin_maxes = malloc(bin_count*sizeof(float));
   bin_counts = malloc(bin_count*sizeof(int));

   /* Create bins for storing counts */
   Gen_bins(min_meas, max_meas, bin_maxes, bin_counts, bin_count);
   
   bin_mutexes = malloc(bin_count*sizeof(pthread_mutex_t));
   pthread_barrier_init(&barrier, NULL, thread_count);
   for(int i=0; i<bin_count; i++)
   {
      pthread_mutex_init(&bin_mutexes[i], NULL);
   }
   pthread_t* thread_handles = malloc(thread_count*sizeof(pthread_t));
   unsigned int* seeds = malloc(thread_count*sizeof(unsigned int));

   double begin;
   for (int i = 0; i < thread_count; i++)
   {
      seeds[i] = time(NULL)*(i+1);
      if(i == thread_count-1)
      {
         begin = elapsed_seconds();
      }
      pthread_create(&thread_handles[i], NULL, gen_and_assign, (void*) &seeds[i]);
   }
   for(int i=0; i<thread_count; i++)
   {
      pthread_join(thread_handles[i], NULL);
   }
   double end = elapsed_seconds();

   pthread_barrier_destroy(&barrier);
   for(int i=0; i<bin_count; i++)
   {
      pthread_mutex_destroy(&bin_mutexes[i]);
   }
   free(bin_mutexes);
   free(thread_handles);
   free(seeds);
   

   /* Print the histogram */
   Print_histo(bin_maxes, bin_counts, bin_count, min_meas);
   printf("Time Taken: %f\n", end-begin);
   printf("Sampled points: %d\n", sampled_data_count);
   free(bin_maxes);
   free(bin_counts);
   return 0;

}  /* main */


double elapsed_seconds()
{
   struct timeval tv;
   struct timezone tz;
   gettimeofday(&tv, &tz);
   return (double)tv.tv_sec + (double)tv.tv_usec/1000000.0;
}

/*---------------------------------------------------------------------
 * Function:  Usage 
 * Purpose:   Print a message showing how to run program and quit
 * In arg:    prog_name:  the name of the program from the command line
 */
void Usage(char prog_name[] /* in */) {
   fprintf(stderr, "usage: %s ", prog_name); 
   fprintf(stderr, "<bin_count> <min_meas> <max_meas> <data_count> <thread_count>\n");
   exit(0);
}  /* Usage */


/*---------------------------------------------------------------------
 * Function:  Get_args
 * Purpose:   Get the command line arguments
 * In arg:    argv:  strings from command line
 * Out args:  bin_count_p:   number of bins
 *            min_meas_p:    minimum measurement
 *            max_meas_p:    maximum measurement
 *            data_count_p:  number of measurements
 */
void Get_args(
      char*    argv[]        /* in  */,
      int*     bin_count_p   /* out */,
      float*   min_meas_p    /* out */,
      float*   max_meas_p    /* out */,
      int*     data_count_p  /* out */,
      int*     thread_count  /* out */) {

   *bin_count_p = strtol(argv[1], NULL, 10);
   *min_meas_p = strtof(argv[2], NULL);
   *max_meas_p = strtof(argv[3], NULL);
   *data_count_p = strtol(argv[4], NULL, 10);
   *thread_count = strtol(argv[5], NULL, 10);

#  ifdef DEBUG
   printf("bin_count = %d\n", *bin_count_p);
   printf("min_meas = %f, max_meas = %f\n", *min_meas_p, *max_meas_p);
   printf("data_count = %d\n", *data_count_p);
#  endif
}  /* Get_args */

/*---------------------------------------------------------------------
 * Function:  Gen_bins
 * Purpose:   Compute max value for each bin, and store 0 as the
 *            number of values in each bin
 * In args:   min_meas:   the minimum possible measurement
 *            max_meas:   the maximum possible measurement
 *            bin_count:  the number of bins
 * Out args:  bin_maxes:  the maximum possible value for each bin
 *            bin_counts: the number of data values in each bin
 */
void Gen_bins(
      float min_meas      /* in  */, 
      float max_meas      /* in  */, 
      float bin_maxes[]   /* out */, 
      int   bin_counts[]  /* out */, 
      int   bin_count     /* in  */) {
   float bin_width;
   int   i;

   bin_width = (max_meas - min_meas)/bin_count;

   for (i = 0; i < bin_count; i++) {
      bin_maxes[i] = min_meas + (i+1)*bin_width;
      bin_counts[i] = 0;
   }

#  ifdef DEBUG
   printf("bin_maxes = ");
   for (i = 0; i < bin_count; i++)
      printf("%4.3f ", bin_maxes[i]);
   printf("\n");
#  endif
}  /* Gen_bins */


/*---------------------------------------------------------------------
 * Function:  Which_bin
 * Purpose:   Use binary search to determine which bin a measurement 
 *            belongs to
 * In args:   data:       the current measurement
 *            bin_maxes:  list of max bin values
 *            bin_count:  number of bins
 *            min_meas:   the minimum possible measurement
 * Return:    the number of the bin to which data belongs
 * Notes:      
 * 1.  The bin to which data belongs satisfies
 *
 *            bin_maxes[i-1] <= data < bin_maxes[i] 
 *
 *     where, bin_maxes[-1] = min_meas
 * 2.  If the search fails, the function prints a message and exits
 */
int Which_bin(
      float   data          /* in */, 
      float   bin_maxes[]   /* in */, 
      int     bin_count     /* in */, 
      float   min_meas      /* in */) {
   int bottom = 0, top = bin_count-1;
   int mid;
   float bin_max, bin_min;

   while (bottom <= top) {
      mid = (bottom + top)/2;
      bin_max = bin_maxes[mid];
      bin_min = (mid == 0) ? min_meas: bin_maxes[mid-1];
      if (data >= bin_max) 
         bottom = mid+1;
      else if (data < bin_min)
         top = mid-1;
      else
         return mid;
   }
   /* Whoops! */
   fprintf(stderr, "Data = %f doesn't belong to a bin!\n", data);
   fprintf(stderr, "Quitting\n");
   exit(-1);
}  /* Which_bin */


/*---------------------------------------------------------------------
 * Function:  Print_histo
 * Purpose:   Print a histogram.  The number of elements in each
 *            bin is shown by an array of X's.
 * In args:   bin_maxes:   the max value for each bin
 *            bin_counts:  the number of elements in each bin
 *            bin_count:   the number of bins
 *            min_meas:    the minimum possible measurment
 */
void Print_histo(
        float  bin_maxes[]   /* in */, 
        int    bin_counts[]  /* in */, 
        int    bin_count     /* in */, 
        float  min_meas      /* in */) {
   int i, j;
   float bin_max, bin_min;
   sampled_data_count = bin_counts[0];
   // normalizing
   int normal_factor = 100;
   int largest_bin_count = bin_counts[0];
   for(i=1; i<bin_count; i++)
   {
      sampled_data_count += bin_counts[i];
      largest_bin_count = largest_bin_count<bin_counts[i] ? bin_counts[i] : largest_bin_count;
   }
   for(i=0; i<bin_count; i++)
   {
      double ratio = (double) bin_counts[i] / (double) largest_bin_count;
      bin_counts[i] = round((double)normal_factor*ratio);
   }
   
   for (i = 0; i < bin_count; i++) {
      bin_max = bin_maxes[i];
      bin_min = (i == 0) ? min_meas: bin_maxes[i-1];
      printf("%.3f-%.3f:\t", bin_min, bin_max);
      for (j = 0; j < bin_counts[i]; j++)
         printf("X");
      printf("\n");
   }
}  /* Print_histo */
