#include <stdio.h>

// numeric simulation of a 1-d oscillator

#define T 30.0 // simulation time
#define H 0.01 // time step
#define OMEGA 1. // frequency

int main() {

	// initial conditions
    double t, x = 0.0, v = 1.0, xp, vp;

	// simulation loop
    for (t = 0; t <= T; t += H) {
		// print current values
		printf("%g, %g, %g\n",t,x,v);

		// calculate next values
		xp = x + H*v;
		vp = v - OMEGA*OMEGA*H*x; 

		// update values
		x = xp;
		v = vp;
    }
	
	// end of program
	return 0;
}
