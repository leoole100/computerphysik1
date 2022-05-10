#include <stdio.h>

#define GAMMA 2.5

// DIES GEHoeRT ZU AUFGABENTEIL B)

// Verwenden VERLET-Verfahren, N I C H T Velocity-Verlet!!!!!!!!!!!!!!

int main(){
	double v_0 = 1., x_0=0., D=1, M=1.;
	
	double T = 30.;

	int N = 1000;

	double H = T / N;

	double v = v_0, xalt = x_0, x=xalt + H*v, xneu, E;	


	for(double t = 0; t < T; t += H){
		
		v = (x-xalt)/H;

		xneu = 2*x - H*H*(D/M*x + GAMMA/M * v) - xalt;

		xalt = x;

		x = xneu;

		E = (M*v*v + D*x*x)/2;

		printf("%g %g %g %g\n", t, x, v, E);

	}

	return(0);

}
