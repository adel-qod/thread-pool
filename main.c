#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "testFunctions.h"
#include "threadPool.h"

int main(void)
{
//	foo10Secs(NULL);
	int err = initThreadPool(10);
	if(err != 100)
	{
		printf("an even bigger fail\n");
	}
	printf("%lu\n", sizeof(pthread_t));
	sleep(10);
	return 0;
}
