#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../timer.h"

const int SIZE = 15*1024;
const int MAX_VALUE = 100;
int thread_count;
int local_n;
double *A;
double *x;
double *y;

/* Does a matrix vector multiplication (A*x = y) using pthreads */
void* MatVecMult (void *rank)
{
    int i, j;
    int start_i, finish_i;
    long my_rank = (long)rank;

    /* Calculate the lines */
    start_i = my_rank*local_n;
    finish_i = start_i + local_n;

    for (i = start_i; i < finish_i; i++)
    {
        for (j = 0; j < SIZE; j++)
            y[i] += A[i*SIZE+j]*x[j];
    }
    
    return NULL;
}

void allocMemory (pthread_t **thread_handles, int thread_count)
{
    *thread_handles = malloc(sizeof(pthread_t*)*thread_count);
    A = malloc(sizeof(double)*SIZE*SIZE);
    x = malloc(sizeof(double)*SIZE);
    y = calloc(SIZE,sizeof(double));
}

double generateNumber ()
{
    int sign = rand() % 2;
    if (sign)
        return rand() % MAX_VALUE;
    else
        return -(rand() % MAX_VALUE);
}

void generateMatrix ()
{
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            A[i*SIZE+j] = generateNumber();
}

void generateVector ()
{
    int i;
    for (i = 0; i < SIZE; i++)
        x[i] = generateNumber();
}

void printSolution ()
{
    int i;
    for (i = 0; i < SIZE; i++)
        printf("%.1lf\n",y[i]);
}   



int main (int argc, char *argv[])
{
    long thread;
    pthread_t *thread_handles;
    double start, finish, elapsed;

    if (argc-1 < 1)
    {
        printf("Usage:> %s <num_threads>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Get number of threads from input and calculate the lines of each thread will work on */
    thread_count = strtol(argv[1],NULL,10);
    local_n = SIZE / thread_count;

    /* Allocate memory for the program */
    allocMemory(&thread_handles,thread_count);

    /* Generate the matrix and the vector */
    generateMatrix();
    generateVector();

    GET_TIME(start);

    /* Create the threads and call the thread function */
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread],NULL,MatVecMult,(void*)thread);

    /* Join the threads */
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread],NULL);

    GET_TIME(finish);
    elapsed = finish - start;

    //printSolution();
    printf("[!] Elapsed time = %.10lf\n",elapsed);

    free(y);
    free(x);
    free(A);
    free(thread_handles);
    return 0;
}
