/****************************COPYRIGHTS AND LICENSE****************************

Copyright (c) 2013, Mhd Adel G. Al Qodamni
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:


Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* For testing convience purposes, the functions in here will have a standard
interface, that is, they all return (void*) and take (void*)
The functions will be divided in two groups, atomic and shared-variable

Times may vary from one machine to another, these are being tested on Linux
Kernel 3.2.0; the cpu: i5 3210M, but the amount of time they spend should be
relative to what's pointed out here	
 */
#include <pthread.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define TEN_SEC_SIZE (1024 * 60)
/* begin foo10Secs */
void *foo10Secs(void *data)
{
	assert(data == NULL);
	char *arr = malloc(sizeof(int)* TEN_SEC_SIZE);
	for(int i = 0; i < TEN_SEC_SIZE; i++)
			arr[i] = i;
	for(int i = 0; i < TEN_SEC_SIZE; i++)
		for(int j = 0; j < TEN_SEC_SIZE; j++)
			if(arr[i] == 255)
				printf("bug\n");
	free(arr);
	printf("Thread has finished executing\n");	
	return NULL;
}
/* end foo10Secs */

/* begin fooCounter */
void* fooCounter(void *data)
{
	assert(data == NULL);
	printf("\n");
	sleep((rand()%5));
	for(int i = 0; i < 10; i++)
	{
		/* Note the following line is not portable
		the standard doesn't say that pthread_t should be
		unsigned long or anything at all
		but it's okay since this is a test function and not the 
		pool itself */
		printf("thread: %lu\t counter = %d\n", pthread_self(), i);
		sleep(rand()%2);
	}
	return NULL;
}
/* end fooCounter */

/* begin countTo10000 */
extern pthread_mutex_t mutex;
static int counter;
void *countTo10000(void *data)
{
	sleep(2);
	assert(data == NULL);
	while(true)
	{
		pthread_mutex_lock(&mutex);
		if(counter == 10000)
			break;
		counter++;
		pthread_mutex_unlock(&mutex);
		sleep(1);	
	}
	printf("counter = %d\n", counter);
	pthread_mutex_unlock(&mutex);
	return NULL;
}
/* end countTo10000 */
