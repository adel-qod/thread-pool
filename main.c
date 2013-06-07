#include <unistd.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#include "testFunctions.h"
#include "threadPool.h"

pthread_mutex_t mutex;/* we'll use this in the tests */

int main(void)
{
	pthread_mutex_init(&mutex, NULL);
//	foo10Secs(NULL);
	int err = initThreadPool(10);
	if(err != INIT_SUCCESS)
	{
		printf("an even bigger fail; error = %d\n", err);
		return 1;
	}
	/*
	for(int i = 0; i < 10; i++)
	{
		err = startJob(fooCounter, NULL);
		if(err != START_JOB_SUCCESS)
		{
			printf("couldn't start a job, error = %d\n", err);
		}
	}
	sleep(15);*/
	for(int i = 0; i < 15; i++)
	{
		err = startJob(fooCounter, NULL);
		if(err != START_JOB_SUCCESS)
		{
			printf("couldn't start a job, error = %d\n", err);
		}
	}
	sleep(40);
	for(int i = 0; i < 6; i++)
	{
		err = startJob(fooCounter, NULL);
		if(err != START_JOB_SUCCESS)
		{
			printf("couldn't start a job, error = %d\n", err);
		}
	}
	sleep(100);
	return 0;
}
