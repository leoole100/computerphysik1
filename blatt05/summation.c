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
	double y=0;
	file = fopen("./summation.dat", "w+");
	
	// substitute r to x = x/(x-1)^1 => dr = - (1+x)/(x-1)^3 
	for(double x = 0; x < 1; x += H){ // and reversed direction
		y += dr(x) * rho(x) * H;
		fprintf(file, "%f %f\n", x, y);
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
