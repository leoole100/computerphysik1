/*
Solution for a) with the Verlet method (second order)
*/

#include <stdio.h>
#include <math.h>

int main(){
	// define constants
	const double v_0 = 1., x_0=0., D=1, M=1.;
	const double T = 2000. * M_PI;

	// loop over step size H
	for(double H = 0.001; H < 0.1; H+=0.001){
		// initialize variables
		double v = v_0, xalt = x_0, x=xalt + H*v, xneu, E;	

		// main loop
		for(double t = 0; t < T; t += H){
			// Verlet step
			v = (x-xalt)/H;
			xneu = 2*x - H*H*(D/M*x * v) - xalt;
			xalt = x;
			x = xneu;

			// calculate energy
			E = (M*v*v + D*x*x)/2;

			// print results
			//printf("%g %g %g %g\n", t, x, v, E);

		}

		// print difference from analytical solution
		printf("%g %g %g %g\n", H, x-x_0, v-v_0, E - (M*v_0*v_0 + D*x_0*x_0)/2);
	}

	// exit ok
	return(0);
}