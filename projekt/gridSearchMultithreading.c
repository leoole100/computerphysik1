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

#define NUM_THREADS 10

void *thread(void *arg);

double dv = 1e-5; //stepsize
double step_plus[3] = {50 , 30 , 10};
//double step_plus[3] = {20 , 20 , 10};

double step_minus[3];

int main(){

	step_minus[0] = step_plus[0];
	step_minus[1] = step_plus[1];
	step_minus[2] = step_plus[2];

	printf("%g grid points in %d threads\n", (step_minus[0] + step_plus[0])*(step_minus[1] + step_plus[1])*(step_minus[2] + step_plus[2]), NUM_THREADS);

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
	printf("	saving in: %s\n", filename);
	FILE * results_file = fopen(filename, "w+");

	double v[3];
	v[0] = v_start[0] - step_minus[0]*dv;
	v[1] = v_start[1] - step_minus[1]*dv;
	v[2] = v_start[2] - step_minus[2]*dv;

	for(
		int i = (step_minus[0] + step_plus[0])/NUM_THREADS * n; 
		i < (step_minus[0] + step_plus[0])/NUM_THREADS * (n+1) - 1; 
	i++){	
		
		v[0] = v_start[0] - step_minus[0]*dv + i*dv;

		for(int j = 0 ; j < step_minus[1] + step_plus[1] ; j++)
		{
			v[1]+=dv;
			for(int k = 0 ; k < step_minus[2] + step_plus[2] ; k++)
			{
				v[2]+=dv;
				double err = trajectory(&v, false);
				fprintf(results_file, "%.10f %.10f %.10f %g \n", v[0], v[1], v[2], err);
			}
			v[2]-=(step_minus[2] + step_plus[2])*dv;

		}
		v[1]-=(step_minus[1] + step_plus[1])*dv;
	}

	fclose(results_file);

	printf("	thread %d finished\n", n);
	return 0;
}