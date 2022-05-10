#include <stdio.h>

// DIES GEHoeRT ZU AUFGABENTEIL A)

int main(){
	double v_0 = 1., x_0=0., D=1, M=1.;
	
	double T = 10.;

	int N = 1000;

	double H = T / N;

	double v = v_0, x = x_0, xneu, vneu, E;	


	for(double t = 0; t < T; t += H){
		
		vneu = v - H*D*x/M;
		xneu = x + H*v;

		x = xneu;
		v = vneu;

		E = (M*v*v + D*x*x)/2;

		printf("%g %g %g %g\n", t, x, v, E);

	}

	return(0);

}
