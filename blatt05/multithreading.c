#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

void *perform_work(void *arguments){
  int index = *((int *)arguments);
  int sleep_time = 1 + rand() % NUM_THREADS;
  printf("THREAD %d: Started.\n", index);  
  sleep(sleep_time); // doing some work
  printf("THREAD %d: Ended.\n", index);
  return NULL;
}

int main(void) {
  pthread_t threads[NUM_THREADS];
  int thread_args[NUM_THREADS];
  int i;
  int result_code;
  
  //create all threads one by one
  for (i = 0; i < NUM_THREADS; i++) {
    printf("IN MAIN: Creating thread %d.\n", i);
    thread_args[i] = i;
    result_code = pthread_create(&threads[i], NULL, perform_work, &thread_args[i]);
    //printf("result code: %d\n", result_code);
  }

  printf("\nIN MAIN: All threads are created.\n\n");

  //wait for each thread to complete
  for (i = 0; i < NUM_THREADS; i++) {
    result_code = pthread_join(threads[i], NULL);
    //printf("result code: %d\n", result_code);
    printf("IN MAIN: Thread %d has ended.\n", i);
  }

  printf("\nMAIN program has ended.\n");
  return 0;
}