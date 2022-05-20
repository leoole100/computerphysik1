/*
	Solution for b) with the Verlet method and damping
*/

#include <stdio.h>

// damping constant 
#define GAMMA 0.3

int main(){
	// define constants
	const double v_0 = 1., x_0=0., D=1, M=1.;
	const double T = 30.;
	const double H = 0.01;

	// initialize variables
	double v = v_0, xalt = x_0, x=xalt + H*v, xneu, E;	

	// main loop
	for(double t = 0; t < T; t += H){
		// Verlet step
		v = (x-xalt)/H;
		xneu = 2*x - H*H*(D/M*x + GAMMA/M * v) - xalt;
		xalt = x;
		x = xneu;

		// calculate energy
		E = (M*v*v + D*x*x)/2;

		// print results
		printf("%g %g %g %g\n", t, x, v, E);

	}

	// exit ok
	return(0);
}
