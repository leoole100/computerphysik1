/*
 * Exercise 5a) with substitution of r
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


const double H = 1.e-3;
FILE *file;

double rho(double x);
double dr(double x);
void numerov(double* y1, double* y2, double* x);


int main(){
	double y1=0, y2 = 0;
	file = fopen("./poisson2.dat", "w+");
	
	// substitute r to x = x/(x-1)^1 => dr = - (1+x)/(x-1)^3 
	for(double x = 0; x < 1; x += H){ // and reversed direction
		numerov(&y1, &y2, &x);
	}
	
	// Ergebnis:                        -0.039789
	// Analytisch: -1/(8 pi) \approx    -0.0397887
	
	fclose(file);
	return 0;
}


double rho(double x){
	return exp(- x/powf(x-1, 2.))/(8 * M_PI);
}

double dr(double x){
	return -(1 + x)/powf(x-1, 3.);
}


void numerov(double *y1, double *y2, double *x){
	double y = 2 * *y2 - *y1 - powf(H, 2.)/12 * (dr(*x+H)*rho(*x+H) + 10*dr(*x)*rho(*x) + dr(*x-H)*rho(*x-H));			// -0.027310
	//double y = 2 * *y2 - *y1 - dr(*x) * powf(H, 2.)/12 * (rho(*x+H) + 10*rho(*x) + rho(*x-H));							// -0.027310
	*y1 = *y2;
	*y2 = y;

	printf("%f %f %f \n", *x, *y2, dr(*x));
	fprintf(file, "%f %f %f \n", *x, *y2, dr(*x));
}
