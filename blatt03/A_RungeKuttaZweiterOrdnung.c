/*
Solution for a) with the Runge Kutta method (second order)
*/

#include <stdio.h>
#include <math.h>

int main(){
	// define constants
	const double v_0 = 1., x_0=0., D=1, M=1.;
	const double T = 1000. * M_PI;

	// loop over step size H
	for(double H = 0.001; H < 0.5; H+=0.005){
		// initialize variables
		double v = v_0, x = x_0;
		double k_1_x, k_1_v, k_2_x, k_2_v, E;	

		// main loop
		for(double t = 0; t < T; t += H){
			// Runge Kutta 1. intermediate step
			k_1_v =  -H*D*x/M;
			k_1_x =  H*v;
			// Runge Kutta 2. intermediate step
			k_2_v = -H*D*(x + k_1_x/2)/M;
			k_2_x = H*(v + k_1_v/2);
			// Runge Kutta main step
			v = v + k_2_v;
			x = x + k_2_x;

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