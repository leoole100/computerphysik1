
#include <math.h>
#include <stdio.h>


#define GAMMA 2.5

// DIES GEHoeRT ZU AUFGABENTEIL C)

// Verwenden VERLET-Verfahren, N I C H T Velocity-Verlet!!!!!!!!!!!!!!

int main(){
	double v_0 = 1., x_0=0., D=1, M=1.;
	
	double T = 300., EINSCHWINGZEIT = T*0.6;

	int N = 1000000;

	double H = T / N;

	double v = v_0, xalt = x_0, x=xalt + H*v, xneu, E;	
    
    double amplitude, phase,phase2;

    double omega_t = 1.5, gamma = 7;

        
        
            
            amplitude = 0.0;
            
            phase = 0.0;
            phase2=0;
            
            v = v_0;
            xalt = x_0;
            x=xalt + H*v;
            
            for(double t = 0; t < T; t += H){
                
                v = (x-xalt)/H;

                xneu = 2*x - H*H*(D/M*x + gamma/M * v + cos(omega_t * t)) - xalt;

                xalt = x;

                x = xneu;

                E = (M*v*v + D*x*x)/2;

                printf("%g %g %g %g\n", t, x, v, E);
                
                /*if(t > EINSCHWINGZEIT){
                    
                    if(x > amplitude){
                        amplitude = x;
                        phase = ((omega_t*t)/(2*M_PI)-(int)((omega_t*t)/(2*M_PI)))*2*M_PI;//fmod(t*omega_t, 2*M_PI);
                        //phase2=fmod(t*omega_t, 2*M_PI);
                    }
                    
                }*/

            }
            /*if(amplitude>6)
                amplitude = 6;
            printf("%g %g %g\n", gamma, omega_t, amplitude);*/

    
    
	
	return(0);

}
