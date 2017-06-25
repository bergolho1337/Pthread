#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "../timer.h"

const int MAX_THREADS = 1024;

long thread_count;
long long n;
double total_int;
double a, b, h;
pthread_mutex_t mutex;

void* Trap (void* rank);
double f (double x);

/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);

int main(int argc, char* argv[]) 
{
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   double start, finish, elapsed;

   /* Get number of threads from command line and other arguments */
   Get_args(argc, argv);

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   pthread_mutex_init(&mutex, NULL);
   h = (b-a) / n;
   total_int = 0.0;

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Trap, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   GET_TIME(finish);
   elapsed = finish - start;

   printf("With n = %lld,\n", n);
   printf("   Our estimate of the integral = %.15f\n", total_int);
   printf("The elapsed time is %.10lf seconds\n", elapsed);
   
   pthread_mutex_destroy(&mutex);
   free(thread_handles);
   return 0;
}  /* main */

/*------------------------------------------------------------------*/
void* Trap (void* rank) 
{
   long my_rank = (long) rank;
   long i, local_n;
   double local_a, local_b;
   double x, local_int;

   local_n = n / thread_count;
   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   local_int = (f(local_a) + f(local_b)) / 2.0;

   for (i = 1; i < local_n; i++)
   {
        x = local_a + i*h;
        local_int += f(x);
   }
   local_int *= h;

   //printf("Thread %ld:> local_int = %lf\n",my_rank,local_int);
   //printf("Thread %ld:> local_a = %lf || local_b = %lf\n",my_rank,local_a,local_b);

   pthread_mutex_lock(&mutex);
   total_int += local_int;
   pthread_mutex_unlock(&mutex);

   return NULL;
}  /* Thread_sum */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */
void Get_args(int argc, char* argv[]) 
{
   if (argc != 5) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
   n = strtoll(argv[2], NULL, 10);
   a = atof(argv[3]);
   b = atof(argv[4]);
   if (n <= 0) Usage(argv[0]);
}  /* Get_args */


/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) 
{
   fprintf(stderr, "usage: %s <number of threads> <n> <a> <b>\n", prog_name);
   fprintf(stderr, "   n is the total number of intervals for the integration\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   fprintf(stderr, "   a is the lower limit for the integration\n");
   fprintf(stderr, "   b is the upper limit for the integration\n");
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:  Integral function
 */
double f (double x)
{
    return sin(x)*exp(x);
}