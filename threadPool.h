#ifndef THREAD_POOL_POOL_H
#define THREAD_POOL_POOL_H

/* creates a pool with numberOfThreads of threads, it returns */
#define INIT_SUCCESS 100 
#define INIT_OUT_OF_MEMORY 200
#define INIT_CANT_CREATE_THREAD 300
#define INIT_SYNC_FAILURE 400
#define INIT_POOL_ALREADY_EXISTS 500
int initThreadPool(unsigned int numberOfThreads);
/*Returns :
INIT_SUCCESS for success
INIT_OUT_OF_MEMORY for failure of allocating enough memory to use internally
INIT_CANT_CREATE_THREAD if the OS didn't create of at least one thread
INIT_SYNC_FAILURE if the pool had an error in controlling its state
INIT_POOL_ALREADY_EXISTS if the pool is already initiated, if you want a 
			different pool, destroy it and init a new one */



/* dispatches a thread from the pool to do jobFunction and gives it parameter */
#define START_JOB_SUCCESS 100
#define NO_AVAILABLE_THREADS 200
#define START_JOB_SYNC_FAILURE 300
int startJob(void* (*jobFunction)(void *data), void *parameter);
/* Returns:
START_JOB_SUCCESS on success
NO_AVAILABLE_THREADS if the pool has no idle threads to dispatch
START_JOB_SYNC_FAILURE if pool had error in controlling its state */

/* destroyThreadPool destroys an already created pool
	You're strongly adviced to call this when you're sure all the threads */
#define DESTROY_SUCCESS 100
#define POOL_ISNT_CREATED 200
#define MUTEX_DESTROY_FAILED 300
#define SEM_DESTROY_FAILED 400
int destroyThreadPool(void);
/* returns DESTROY_SUCCESS if pool was destroyed or one of the errors 
	defined above */

#endif
