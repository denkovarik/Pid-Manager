/**
 * A solution to the pid manager problem. 
 *
 */

#include "pid.h"
#include <pthread.h>
#include <stdio.h>

#include <semaphore.h>
#include <string.h>
#include <errno.h>

// Allocates the pid map.
int allocate_map(void) 
{
	int i;		// Counter



	// Initialize pid_map
	for(i = PID_MIN; i <= PID_MAX; i++)
		pid_map[i] = 0;

	// Initialize last
	last = PID_MIN;

   	return last;
}

// Allocates a pid
int allocate_pid(void)
{
	/* local variables */
	int i;		// Counter

   	/* acquire the mutex lock and warn if unable */
  	if (pthread_mutex_lock(&mutex) != 0)
	{
    	printf("Could not acquire mutex lock %s\n",strerror(errno));
		return -1;	// Error occured
	}
   	
	// Check that the next pid is within range
	if(last < PID_MIN || last >= PID_MAX)
		last = PID_MIN;
	
	/* find the next available pid */
	while(last >= PID_MIN && last <= PID_MAX && pid_map[last])
		last = last + 1;
	
	// Check that next pid is within range
	if(last >= PID_MIN && last <= PID_MAX && !pid_map[last])
		pid_map[last] = 1;		// Allocate the pid
	else
		last = -1;				// Otherwise return error
	
   	/* release and warn if the mutex was not released */
  	if (pthread_mutex_unlock(&mutex) != 0)
	{
    	printf("Could not release mutex lock %s\n",strerror(errno));
		// Give pid back
		if(last >= PID_MIN && last <= PID_MAX)
			pid_map[last] = 0;

		return -1;				// Error occured
	}

   	return last;
}

// Releases a pid
void release_pid(int pid)
{
   	/* acquire the mutex lock and warn if unable */
  	if (pthread_mutex_lock(&mutex) != 0)
	{
		// Error message in acquiring mutex lock
    	printf("Could not acquire mutex lock %s\n",strerror(errno));
		return;		// Error occured
	}

	// Check that pid is within range and then release it
	if(pid >= PID_MIN && pid <= PID_MAX)
		pid_map[pid] = 0;	

   	/* release and warn if the mutex was not released  */
  	if (pthread_mutex_unlock(&mutex) != 0)
	{
		// Error message in releasing mutex lock
    	printf("Could not release mutex lock %s\n",strerror(errno));
	}
}
