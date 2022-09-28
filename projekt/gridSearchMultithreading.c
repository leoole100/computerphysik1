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

const double dv = 1e-8; //stepsize
const double steps[3] = {20, 20, 10};

/*const double coordinateSystem[3][3] = {
	{-0.919601, 0.36784, -0.13794},
	{0,0,1},
	{0.36784, 0.919601, 0.} // cross product of the above
};*/

const double coordinateSystem[3][3] = {
	{1,0,0},
	{0,1,0},
	{0,0,1}
};


int main(){
	printf("%g grid points in %d threads\n", 8*steps[0]*steps[1]*steps[2], NUM_THREADS);

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
	printf("	saving in: %s \n", filename);
	FILE * results_file = fopen(filename, "w+");

	double v[3];

	for(
		int i = 2*steps[0]/NUM_THREADS * n; 
		i < 2*steps[0]/NUM_THREADS * (n+1); 
	i++){
		
		for(int j = 0 ; j < 2*steps[1] ; j++)
		{
			for(int k = 0 ; k < 2*steps[2] ; k++)
			{
				// set v to v_start
				v[0] = v_start[0];
				v[1] = v_start[1];
				v[2] = v_start[2];

				// calculate velocity in cartesian coordinates
				v[0] += coordinateSystem[0][0] * (i - steps[0]) * dv + coordinateSystem[1][0] * (j - steps[1]) * dv + coordinateSystem[2][0] * (k - steps[2]) * dv;
				v[1] += coordinateSystem[0][1] * (i - steps[0]) * dv + coordinateSystem[1][1] * (j - steps[1]) * dv + coordinateSystem[2][1] * (k - steps[2]) * dv;
				v[2] += coordinateSystem[0][2] * (i - steps[0]) * dv + coordinateSystem[1][2] * (j - steps[1]) * dv + coordinateSystem[2][2] * (k - steps[2]) * dv;
				
				double err = trajectory(&v, false);
				fprintf(results_file, "%d %d %d %.10f %.10f %.10f %g \n", i, j, k, v[0], v[1], v[2], err);

				// print progress of the thread 0
				if(n==0){
					// print progress in percent
					printf("	%.01f %%\n", 100. * i/(2*steps[0]/NUM_THREADS) + 100. * j/(2*steps[0]/NUM_THREADS * 2*steps[1]) + 100. * k/(2*steps[0]/NUM_THREADS * 2*steps[1] * 2*steps[2]));
				}
			}
		}
	}

	fclose(results_file);

	printf("	thread %d finished\n", n);
	return 0;
}
