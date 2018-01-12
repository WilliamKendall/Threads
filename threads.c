/* 
William Kendall
CMSC 412
Week 3 - Processor Scheduling
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define NUMBER_OF_THREADS 3
#define START_NUMBER 1
#define END_NUMBER 5

pthread_t threads [NUMBER_OF_THREADS];
pthread_mutex_t lock;
pthread_cond_t cond;

int runThread = 1;	//thread that should print to screen, others wait

static void * threadFunction(void *arg)
{
	int iterationNo;
	intptr_t threadNumber =(intptr_t) (*((void **) arg));
	free(arg);
	for(iterationNo = START_NUMBER; iterationNo <= END_NUMBER; iterationNo++)
	{
		pthread_mutex_lock(&lock);
		
		while(runThread != threadNumber)
			pthread_cond_wait(&cond, &lock);
	
		printf("Thread %i - iteration no. %i.\n", threadNumber, iterationNo);
		runThread ++;
		if(runThread > NUMBER_OF_THREADS)
			runThread = 1;		
		pthread_cond_broadcast (&cond);
		pthread_mutex_unlock(&lock);
		
	}
}

int main()
{
	//setup
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	
	int i; //iterator
	for(i = 0; i < NUMBER_OF_THREADS; i++)
	{
		intptr_t * arg = malloc(sizeof(*arg));
		*arg=i+1;	//thread numbers start at 1
		pthread_create(&threads[i], NULL,threadFunction, arg);
	}
	
	//wait until finished
	pthread_exit(NULL);
	
	//clean up
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	
}
