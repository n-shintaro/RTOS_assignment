// \* 
// compile with:
// 	g++ threadex3start.cpp -lpthread -o thread3
   
// run with:
// 	.\thread3
// *\

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// initialize mutex to be used to protect conditions and the variable count
// initialize condition
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex1
pthread_mutex_t condition_mutex =PTHREAD_MUTEX_INITIALIZER; // mutex2
pthread_cond_t condition_cond =PTHREAD_COND_INITIALIZER; // condition variable
// declare functionCount1 and functionCount2
void *functionCount1(void *);
void *functionCount2(void *);

int  count = 0;

#define COUNT_DONE  10

int main()
{
   // declare the ids of thread 1 and thread 2
	pthread_t thread1, thread2;
   // create two independent threads each of which will execute functionC
   int rc1,rc2;
	if( (rc1=pthread_create( &thread1, NULL, &functionCount1, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc1);
   }
   if( (rc2=pthread_create( &thread2, NULL, &functionCount2, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc2);
   }
   // Wait till threads are complete before main continues.
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
   printf("Final count: %d\n",count);
   exit(0);
}

void *functionCount1(void*)
{
   for(;;)
   {
	// Lock mutex 
	pthread_mutex_lock(& condition_mutex);
	// Wait while functionCount2() signals the condition
   pthread_cond_wait(&condition_cond,&condition_mutex);
   pthread_mutex_unlock(& condition_mutex);
	count++;

	printf("Counter value functionCount1: %d\n",count);

	// Unlock mutex
   pthread_mutex_unlock(& condition_mutex);

	sleep(1);

	if(count >= COUNT_DONE) pthread_exit(NULL);
    }
}

// Write numbers 4-7
void *functionCount2(void*)
{
    for(;;)
    {
 	// Lock mutex 
   pthread_mutex_lock(&condition_mutex);
        // Signal the condition to functionCount1()
      pthread_cond_signal(&condition_cond);
	count++;

	printf("Counter value functionCount2: %d\n",count);

	// Unlock mutex 
	pthread_mutex_unlock(&condition_mutex);
	sleep(1);

	if(count >= COUNT_DONE) pthread_exit(NULL);
    }
}
