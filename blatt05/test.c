#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>


double test(double *arg){
	double res = *arg * *arg; 
	return res;
}

int main(void) {
	double arg = 2.;

	printf("%.4f\n", test(&arg));

	return 0;
}