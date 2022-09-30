/*
Solution for a) with the explicit euler method
*/

#include <stdio.h>
#include <math.h>

int main(){
	// define constants
	const double v_0 = 1., x_0=0., D=1, M=1.;
	const double T = 1000. * M_PI;

	// loop over step size H
	for(double H = 0.001; H < 0.5; H+=0.0005){
		// initialize variables
		double v = v_0, x = x_0, xneu, vneu, E;	
		
		// simulation loop
		for(double t = 0; t < T; t += H){
			// euler step
			vneu = v - H*D*x/M;
			xneu = x + H*v;

			// update variables
			x = xneu;
			v = vneu;

			// calculate energy
			E = (M*v*v + D*x*x)/2;

			// print results for debugging
			//printf("%g %g %g %g\n", t, x, v, E);

		}

		// print difference from analytical solution
		printf("%g %g %g %g\n", H, x-x_0, v-v_0, E - (M*v_0*v_0 + D*x_0*x_0)/2);
	}

	// exit ok
	return(0);
}
