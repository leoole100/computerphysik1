/**
 * @file gridSearchMultithreading.c
 * @brief creates a 3D velocity grid and evaluates the error function
 * @version 1.0
 * @date 2022-09-21
 * 
 * @author Leon Oleschko
 * 
 */

#include "methods.h"
#include <pthread.h>

#define NUM_THREADS 4

void *thread(void *arg);

double dv = 0.0001; //stepsize
double step_plus[3] = {30 , 30 , 30};
double step_minus[3];

int main(){

	step_minus[0] = step_plus[0];
	step_minus[1] = step_plus[1];
	step_minus[2] = step_plus[2];

	v_start[0]-=step_minus[0]*dv;
	v_start[1]-=step_minus[1]*dv;
	v_start[2]-=step_minus[2]*dv;

	int thread_args[NUM_THREADS];
	pthread_t threads[NUM_THREADS];

	// create threads
	for (size_t i = 0; i < NUM_THREADS; i++){
    	thread_args[i] = i;
		pthread_create(&threads[i], NULL, thread, &thread_args[i]);
	}
		
	//wait for each thread to complete
	for (size_t i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}

void *thread(void *arg){
	// get the index of the thread
	const int n = *((int *)arg);
	// exporting
	char filename[210];
	snprintf(filename, 22, "data/gridSearch%02d.dat", n);
	printf("saving in: %s\n", filename);
	FILE * results_file = fopen(filename, "w+");

	for(
		int i = (step_minus[0] + step_plus[0])/NUM_THREADS * n; 
		i < (step_minus[0] + step_plus[0])/NUM_THREADS * (n+1); 
	i++){	
		v_start[0]+=dv;

		for(int j = 0 ; j < step_minus[1] + step_plus[1] ; j++)
		{
			v_start[1]+=dv;
			for(int k = 0 ; k < step_minus[2] + step_plus[2] ; k++)
			{
				v_start[2]+=dv;
				trajectory(v_start[0],v_start[1],v_start[2], false);
				double currenterr = errfunction();
				fprintf(results_file, "%.10f %.10f %.10f %g \n", v_start[0], v_start[1], v_start[2], currenterr);
			}
			v_start[2]-=(step_minus[2] + step_plus[2])*dv;

		}
		v_start[1]-=(step_minus[1] + step_plus[1])*dv;
	}

	fclose(results_file);
	return 0;
}