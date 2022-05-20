/*
	solution for part c with the Verlet method
	includes damping and excitation
*/

#include <math.h>
#include <stdio.h>

int main(){
	// define constants
	const double v_0 = 1., x_0=0., D=1, M=1.;
	const double T = 300., einschwingzeit = T*0.6;
	const double H = 0.003;

	// and initialize variables
	double v = v_0, xalt = x_0, x=xalt + H*v, xneu, E;	
    double amplitude, phase,phase2;    
    
	// loop over excitation frequencies with linear step size
    for(double omega_t=0.001; omega_t < 3; omega_t += 0.01+0.005*omega_t){

		// loop over different damping factors
        for(double gamma=0; gamma < 10; gamma += 0.05){
			// reset variables	
            amplitude = 0.0;
            phase = 0.0;
            phase2=0;
            // and initial conditions
            v = v_0;
            xalt = x_0;
            x=xalt + H*v;
            
			// main simulation time loop
            for(double t = 0; t < T; t += H){
                // verlet step
                v = (x-xalt)/H;
                xneu = 2*x - H*H*(D/M*x + gamma/M * v + cos(omega_t * t)) - xalt;
                xalt = x;
                x = xneu;

				// print results (for debugging)
                //printf("%g %g %g %g\n", t, x, v, E);
                
				// test for maximum after einschwingzeit
                if(t > einschwingzeit && x > amplitude){
					amplitude = x;
					phase = ((omega_t*t)/(2*M_PI)-(int)((omega_t*t)/(2*M_PI)))*2*M_PI;
					//phase=fmod(t*omega_t, 2*M_PI);
                    
                }

            }

			// print results
            printf("%g %g %g %g\n", gamma, omega_t, amplitude, phase);

        }

		// print newline after each damping factor
        printf("\n");
    }

	// return ok
	return(0);
}
