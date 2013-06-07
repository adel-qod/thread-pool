#ifndef THREAD_POOL_POOL_H
#define THREAD_POOL_POOL_H

/* creates a pool with numberOfThreads of threads, it returns */
#define INIT_SUCCESS 100 
#define INIT_OUT_OF_MEMORY 200
#define INIT_CANT_CREATE_THREAD 300
#define INIT_SYNC_FAILURE 400
int initThreadPool(unsigned int numberOfThreads);
/*Returns :
INIT_SUCCESS for success
INIT_OUT_OF_MEMORY for failure of allocating enough memory to use internally
INIT_CANT_CREATE_THREAD if the OS didn't create of at least one thread
INIT_SYNC_FAILURE if the pool had an error in controlling its state*/



/* dispatches a thread from the pool to do jobFunction and gives it parameter */
#define START_JOB_SUCCESS 100
#define NO_AVAILABLE_THREADS 200
#define START_JOB_SYNC_FAILURE 300
int startJob(void* (*jobFunction)(void *data), void *parameter);
/* Returns:
START_JOB_SUCCESS on success
NO_AVAILABLE_THREADS if the pool has no idle threads to dispatch
START_JOB_SYNC_FAILURE if pool had error in controlling its state */
#endif
