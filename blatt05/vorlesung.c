// solves Schroedinger eqn. for HO using shooting method and Numerov

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define H 0.001
#define XMAX 10.0
#define N (XMAX/H)

double *phi;

double F(double x) {
	return 0.0;
}

double k2(double x, double E) {
	return 2.0*E-x*x;
}

double numerov(double E, int save, int sign) {
	double x, fnm1=0.0, fn=H, fnp1;
	
	// left
	int i=0;
	for(x=-XMAX;x <= 0;x += H) {
		fnp1 = (H*H/12.0*(F(x+H)+10.0*F(x)+F(x-H)) + fn*(2.0-5.0/6.0*H*H*k2(x,E))-fnm1*(1.0+H*H/12.0*k2(x-H,E)))/(1.0+H*H/12.0*k2(x+H,E));

		fnm1=fn;
		fn=fnp1;
		if(save)
			phi[i++]=fn;
	}
	double df1=(fn-fnm1)/fn;

	// right
	fnm1=0.0, fn=H;
	i=2*(double)N-1;
	for(x=XMAX;x > 0;x -= H) {
		fnp1 = (H*H/12.0*(F(x+H)+10.0*F(x)+F(x-H)) + fn*(2.0-5.0/6.0*H*H*k2(x,E))-fnm1*(1.0+H*H/12.0*k2(x-H,E)))/(1.0+H*H/12.0*k2(x+H,E));

		fnm1=fn;
		fn=fnp1;
		if(save)
			phi[i--]=sign*fn;
	}
	double df2=(fnm1-fn)/fn;

	return df2-df1;
}

int main() {
	phi = (double *)malloc(2*(int)N*sizeof(double));

	double E0=7.0, E1=8.0, Ex, tmp;
	do {
		Ex = (E1+E0)/2.0;
		// bisection (non continous !)
		tmp = numerov(Ex,0,0);
	
		if(tmp*numerov(E0,0,0)>0)
			E0=Ex;
		else
			E1=Ex;
	} while (fabs((E1-E0)/E0)>1.e-14);

	// flip sign if slope jumps
	if(fabs(numerov(E1,0,0))>1.0)
		numerov(E1,1,-1);
	else
		numerov(E1,1,1);

	// Normierung
	int i;
	double sum=0.0;
	for(i=0;i< 2*(int)N;i++) 
		sum += phi[i]*phi[i]*H;
	
	// save it
	FILE *f=fopen("ho2.dat","w");
	for(i=0;i< 2*(int)N;i++) 
		fprintf(f,"%g %g %g\n",-XMAX+i*H,phi[i]/sqrt(sum),E1);
	
	return 0;
}