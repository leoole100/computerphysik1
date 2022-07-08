// solves f''(x) + k(x)^2 f(x) = F(x) for Poisson eqn using Numerov method and fix

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define H 0.001
#define XMAX 20.0
#define N (XMAX/H)

double F(double x) {
	return -1/(8*M_PI)*exp(-x);	// Poisson
	//return -x/2.0*exp(-x);
}

double k2(double x) {
	// 0.0	// Poisson
	return 0.0;
}

int main() {
	// cos: fnm1=1.0, fn=fnm1
	// sin: fnm1=0.0, fn=H
	// Poisson: fnm1=0.0, fn = 1-(H+2.0)/2.0*exp(-H)	(aus analytischer Loesung!)
	double x, fnm1=0.0, fn=H, fnp1;
	double *v=(double *)malloc(N*sizeof(double));

	FILE *f;
	f=fopen("poisson2.dat","w");

	int i=1;
	for(x=H;x <= XMAX;x += H) {
		fnp1 = (H*H/12.0*(F(x+H)+10.0*F(x)+F(x-H)) + fn*(2.0-5.0/6.0*H*H*k2(x))-fnm1*(1.0+H*H/12.0*k2(x-H)))/(1.0+H*H/12.0*k2(x+H));

		v[i++]=fn;

		fnm1=fn;
		fn=fnp1;
	}
	// remove linear part of solution
	i=0;
	for(x=H;x < XMAX;x += H)
		fprintf(f,"%g %g\n",x,v[i++]-x*(v[(int)N-1]-v[(int)N-2])/H);

	return 0;
}