#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define H 1.e-3
#define R_START 20
#define N (R_START/H)

double rho(double r);
void numerov(double* y1, double* y2, double r);

FILE *file;

int main(){
	// create array for results
	double* res = malloc(R_START/H * sizeof(double));

	int i=0; double y1=0, y2 = H;
	for(double r = R_START; r >= 0; r -= H){
		numerov(&y1, &y2, r);
		res[i] = y2;
		i++;
	}

	// remove linear part and save
	file = fopen("poisson.dat","w+");
	i=0; double chi, phi;
	for(double r = R_START; r >= 0; r -= H){
		chi = res[i]-r*(res[0]-res[1])/H;
		phi = chi/r;
		fprintf(file, "%g %g %g\n", r, chi, res[i]);
		i++;
	}

	fclose(file);
	return 0;
}

double rho(double r){
	double wert = -exp(-r)/(8 * M_PI);
	return wert;
}

void numerov(double* y1, double* y2, double r){
	//double y = 2 * *y2 - *y1 - H*H/12*(rho(r+H)+10*rho(r)+rho(r-H));
	double y = H*H/12.0*(rho(r+H)+10.0*rho(r)+rho(r-H)) + *y2 *(2.0)- *y1;
	*y1 = *y2;
	*y2 = y;
}
