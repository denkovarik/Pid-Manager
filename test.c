/**
 * test.c
 *
 * Test the implementation of the PID manager.
 *
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pid.h"

#define NUM_THREADS 100
#define ITERATIONS   10
#define SLEEP         2

int in_use[PID_MAX + 1]; 	// allocate array to keep track of pids in use

/**
 * mutex lock used when accessing data structure
 * to ensure there are no duplicate pid's in use.
 */
pthread_mutex_t test_mutex;	// declare mutex lock for access to pids



// allocates pids to the threads. Function used in creation of threads.
void *allocator(void *param)
{
	// local variables
   	int i, pid;



	// tests allocating and releasing a bunch of pids
   	for (i = 0; i < ITERATIONS; i++) {
      	/* sleep for a random period of time */
		sleep((int)(random() % SLEEP));

      	/* allocate a pid */
		pid = allocate_pid();

		// Check if pid is available
      	if (pid == -1)
         	printf("No pid available\n");
      	else 
		{
         	/* mark the pid as in use */
			in_use[pid] = 1;

			// print to screen that pid was allocated		
			printf("allocated %d\n", pid);	

         	/* sleep for a random period of time */
			sleep((int)(random() % SLEEP));
         
         	/* release the pid */
			release_pid(pid);
			in_use[pid] = 0;

			// print to screen that pid was released
			printf("released %d\n", pid);
      	}
   	}
}



// This is the main program which serves as the start of the program
int main(void)
{
	// declare local variables
   	int i, error;

	// allocate array of threads for testing
   	pthread_t tids[NUM_THREADS];
	
	// create mutex lock for access to pid manager
    if(pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\n mutex init failed\n");	// print error message if error
    }

	// Initialize in_use array
   	for (i = 0; i <= PID_MAX; i++) {
      	in_use[i] = 0;
   	}

   	/* allocate the pid map */
   	if (allocate_map() == -1)
   	{
		// print error message if unable to allocate map
      	printf("Unable to allocate map");
      	return -1; 	// return error code
   	}

   	srandom((unsigned)time(NULL));	// seed random number generator

   	/* create the threads */
	i = 0;	
    while (i < NUM_THREADS) 
	{
		// create the new thread
        error = pthread_create(&(tids[i]), NULL, &allocator, NULL);

		// print error message if thread could not be created
        if (error != 0)
            printf("\nThread can't be created\n");
        i++;	// increment loop counter
    }

   	/* join the threads */
	for(i = 0; i < NUM_THREADS; i++)
	{
    	pthread_join(tids[i], NULL);
	}

	// Destroy the mutex lock
    pthread_mutex_destroy(&mutex);
	
   	/* test is finished */

   	return 0;
}
