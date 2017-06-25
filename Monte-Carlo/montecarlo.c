#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "../timer.h"

const int MAX_THREADS = 1024;

long thread_count;
long long total_tosses;
long long total_in_circle;
pthread_mutex_t mutex;

void* Monte_Carlo (void* rank);

/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);

int main(int argc, char* argv[]) 
{
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   double estimate_pi;
   double start, finish, elapsed;

   /* Get number of threads from command line */
   Get_args(argc, argv);

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   pthread_mutex_init(&mutex, NULL);
   total_in_circle = 0;

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Monte_Carlo, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   GET_TIME(finish);
   elapsed = finish - start;

   estimate_pi = 4*total_in_circle / (double)total_tosses;
   printf("With number_of_tosses = %lld,\n", total_tosses);
   printf("   Our estimate of pi = %.15f\n", estimate_pi);
   printf("The elapsed time is %.10lf seconds\n", elapsed);
   
   pthread_mutex_destroy(&mutex);
   free(thread_handles);
   return 0;
}  /* main */

/*------------------------------------------------------------------*/
void* Monte_Carlo (void* rank) 
{
   long my_rank = (long) rank;
   long long i;
   long long my_tosses = total_tosses / thread_count;
   long long my_in_circle = 0;
   double x, y, dist;
   struct drand48_data drand_buf;                       /* Buffer com os numeros aleatorios */

   /* Gerar os numeros aleatorios de forma Thread-Safe */
   int seed = my_rank;
   srand48_r(seed,&drand_buf);

   for (i = 0; i < my_tosses; i++)
   {
        drand48_r(&drand_buf,&x);
        drand48_r(&drand_buf,&y);
        dist = x*x + y*y;
        if (dist <= 1) my_in_circle++;
   }

   printf("Thread %ld:> my_in_circle = %lld\n",my_rank,my_in_circle);

   pthread_mutex_lock(&mutex);
   total_in_circle += my_in_circle;
   pthread_mutex_unlock(&mutex);

   return NULL;
}  /* Thread_sum */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, total_tosses
 */
void Get_args(int argc, char* argv[]) 
{
   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
   total_tosses = strtoll(argv[2], NULL, 10);
   if (total_tosses <= 0) Usage(argv[0]);
}  /* Get_args */


/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) 
{
   fprintf(stderr, "usage: %s <number of threads> <number of tosses>\n", prog_name);
   fprintf(stderr, "   n is the number of tosses to the dartboard\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   exit(0);
}  /* Usage */
