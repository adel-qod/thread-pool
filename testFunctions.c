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

Atomic functions perform an operation on a local data for a deterministic time

Shared-variable functions perform an operation on a shared variable for a determinstic time


Times may vary from one machine to another, these are being tested on Linux
Kernel 3.2.0; the cpu: i5 3210M, but the amount of time they spend should be
relative to what's pointed out here	
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Atomic group begins */
/*****************************************************************************/
#define TEN_SEC_SIZE (1024 * 60)
/* foo10Secs begins */
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
	return NULL;
}
/* foo10Secs ends */


/*****************************************************************************/
/* Atomic group ends */
