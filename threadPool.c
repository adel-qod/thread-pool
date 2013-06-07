/****************************COPYRIGHTS AND LICENSE****************************

Copyright (c) 2013, Mhd Adel G. Al Qodamni
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:


Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <pthread.h>
#include <semaphore.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Note: To fully understand how the pool works, refer to doc.txt */

/* begin function prototypes */
/* this is the waiting function, all threads spend their idle time here
it returns void* and takes void* just to make calling pthread_create easier
but neither are used */
static void* waitingFunction(void*);
/* end function prototypes */

/* begin pool related variables */
static pthread_t *threadsArray;/* our threads of the pool */
static unsigned int threadPoolSize;
static unsigned int busyThreadsCount;/* counts how many threads are busy */
static sem_t poolControlSemaphore;/* Used to make threads idle or do a job */
static pthread_mutex_t *busyThreadsCountMutex;/* protects busyThreadsCount */

/* a function pointer to a function that returns void* and takes *void
this function pointer is used by threads to start the job they're given*/
static void* (*jobFunctionPointer) (void *data);
static void *data;/* the data which the user sends as a parameter to thread */
/* end pool related variables */


/* begin initThreadPool */
/* The function creates the semaphore, the mutex, creates an array of threads
starts them all and sends them to wait on the semaphore
It may return any of the following  */
#define INIT_SUCCESS 100
#define INIT_OUT_OF_MEMORY 200
#define INIT_CANT_CREATE_THREAD 300
#define INIT_SYNC_FAILURE 400
int initThreadPool(unsigned int numberOfThreads)
{
	threadPoolSize = numberOfThreads;
	
	threadsArray = malloc(numberOfThreads * sizeof(pthread_t));
	if(threadsArray == NULL)
		return INIT_OUT_OF_MEMORY;
	
	/* semaphore starts with 0, meaning all threads will initially block */
	if(sem_init(&poolControlSemaphore, 0, 0) < 0)
	{/* Failed to create a semaphore */
		free(threadsArray);
		return INIT_SYNC_FAILURE;
	}

	for(unsigned int i = 0; i < numberOfThreads; i++)
	{/* start all threads and send them to the waiting function */
		if(pthread_create(&threadsArray[i], NULL, waitingFunction,
					NULL) < 0)
		{/* Failed to create a thread for some reason */
			/* kill already created threads */
			for(unsigned j = 0; j < i; j++)
				pthread_cancel(threadsArray[j]);
			sem_destroy(&poolControlSemaphore);
			free(threadsArray);
			return INIT_CANT_CREATE_THREAD;
		}	
	}
	return INIT_SUCCESS;
}
/* End initThreadPool */

/* begin startJob */
#define START_JOB_SUCCESS 100
#define NO_AVAILABLE_THREADS 200
#define START_JOB_SYNC_FAILURE 300
int startJob(void* (*jobFunction)(void *data), void *parameter)
{
	pthread_mutex_lock(busyThreadsCountMutex);
	if(busyThreadsCount == threadPoolSize)
	{
		pthread_mutex_unlock(busyThreadsCountMutex);
		return NO_AVAILABLE_THREADS;
	}
	pthread_mutex_unlock(busyThreadsCountMutex);
	jobFunctionPointer = jobFunction;
	data = parameter;
	if(sem_post(&poolControlSemaphore) < 0)
		return START_JOB_SYNC_FAILURE;
	return START_JOB_SUCCESS;
}
/* end startJob */

/* begin waitingFunction */
static void* waitingFunction(void *par)
{
	par = NULL;/* just to avoid an ugly compiler warning */
	while(true)
	{
		/* The only likely failure that can happen here is if the call
		 	was interrupted by a signal handler in which case we
			simply attempt to wait again */
		if(sem_wait(&poolControlSemaphore) < 0)
			continue;
		/* Check the man pages for the pthread_mutex_lock
		there's (ALMOST) 0 chance these functions may fail so 
		there's no sense in checking their error codes */	

		pthread_mutex_lock(busyThreadsCountMutex);
		busyThreadsCount++;
		pthread_mutex_unlock(busyThreadsCountMutex);

		jobFunctionPointer(data);/* finish job and get back to pool */

		pthread_mutex_lock(busyThreadsCountMutex);
		busyThreadsCount--;
		pthread_mutex_unlock(busyThreadsCountMutex);
		
	}
	return NULL;/* To avoid warnings, but this is not reachable */
}
/* end waitingFunction */
