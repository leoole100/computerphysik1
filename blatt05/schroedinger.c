#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

#define H 1.e-4
#define X_MAX 30.
//#define X_MAX 14.
#define N ((int) (X_MAX/H))
#define EXPORT_STEPS ((int) (0.1/H))
#define UNCERT_END 1.e-12

#define UNCERT_GUESS 0.25
double guess[] = {2.5, 4.5, 6, 7.5, 8.5, 10, 11, 12, 13, 14, 15, 16, 17, 18., 18.5, 19.5, 20., 21, 22, 22.5, 23.5, 24, 25.,25.5, 26.};
//double guess[] = {2.5, 4.5, 6, 7.5, 8.5, 10};

void integ(double E, double* phi);
void numerov(double *y1, double *y2, double x, double E);
void *findAndExport(void *argument);
double V(double x);


int main(){
	const uint NUM_THREADS = sizeof(guess)/sizeof(double);
	int thread_args[NUM_THREADS];
	pthread_t threads[NUM_THREADS];

	// create a thread for each guess
	for (int i = 0; i < NUM_THREADS; i++){
    	thread_args[i] = i;
		pthread_create(&threads[i], NULL, findAndExport, &thread_args[i]);
	}
		
	//wait for each thread to complete
	for (size_t i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}


	return 0;
}

// find the root of the error function near the n-th guess and save the generated wave function
void *findAndExport(void *argument){
	// get the index of the thread
	const int n = *((int *)argument);

	// create array for the wave Function
	// in the heap because the stack is too small
	double* phi = malloc(N*sizeof(double));

	// bisection to find the root
	double E0 = guess[n]-UNCERT_GUESS;
	double E1 = guess[n]+UNCERT_GUESS;
	double Ex, tmp;
	do {
		Ex = (E1+E0)/2.0;
		
		integ(Ex, phi);
		tmp = phi[0];
		integ(E0, phi);

		if(tmp*phi[0]>0)
			E0=Ex;
		else
			E1=Ex;
	} while (fabs((E1-E0)/E0)>UNCERT_END);

	printf("guess: %.2f E: %f\n", guess[n], E1);

	// normalizing
	double norm = 0;
	for(int i=0; i<N; i++){
		norm += phi[i]*phi[i]*H;
	}
	norm = sqrt(norm);
	for(int i=0; i<N; i++){
		phi[i] /= norm;
	}

	// exporting
	FILE *file;
	char filename[210];
	snprintf(filename, 10, "out%02d.dat", n);
	printf("saving in: %s\n", filename);
	file = fopen(filename, "w+");
	fprintf(file, "%f\n", E1);
	for(int i=0; i<N; i+=EXPORT_STEPS){
		fprintf(file, "%f %f\n", i*H, phi[i]);
	}
	fclose(file);

	// end the task successfully
	return 0;
}

// integrating the wave function
void integ(double E, double *phi){

	double y1 = 0, y2 = 1e-10;
	for(int i=0; i <= N; i++){
		numerov(&y1, &y2, (N-i)*H, E);
		phi[N-i] = y2;
	}
}

// numerov step
void numerov(double *y1, double *y2, double x, double E){
	
	double y = 2* *y2*(1 - H*H*5/12*(E-V(x))) - *y1*(1 - H*H/12*(E-V(x-H)));
	*y1 = *y2;
	*y2 = y;
}


double V(double x){
	return x;
}
