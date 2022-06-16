#include <stdio.h>
#include <math.h>

#define NMag 3 // Number of magnet
#define Nmax 20000 // Maximal number of simulation steps
#define H 0.001
#define GAMMA 0.1
#define K 1. // Schwerkraftstärke
#define h 0.1 // Höhe des Pendels

// parameters for the plot
const double start[2] = {1.5, 1.5};
const double epsilon = 1e-2;



double rMag[NMag][3]; // X- und Y-Koordinaten und Magnetstärke
const double Tmax = Nmax * H;
double r[2], v[2];
double kraft[2], diff[2], E; // temporary variables
FILE *file;

int main(){
    // calculate positions of the magnets and save to file "magnete.dat"
    file = fopen("./magnete.dat", "w+");
    // loop over all magnets
    for(int i = 0; i < NMag; i++){
        const double temp = 2*M_PI*i/NMag;

        rMag[i][0] = cos(temp);
        rMag[i][1] = sin(temp);
        rMag[i][2] = 1.;

        fprintf(file, "%g %g\n", rMag[i][0], rMag[i][1]); // plot "magnetpendel.dat" using 2:3 w l, "magnete.dat" lw 5
        //printf("x: %g \t\t y: %g \t\t Stärke: %g \n", rmag[i][0], rmag[i][1], rmag[i][2]);
    }
    fclose(file);    
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	// open output file
	file = fopen("./path.dat", "w+");

    // loop over x and y
    for( double x = start[0]-epsilon; x <= start[0]+epsilon; x += epsilon){
        for( double y = start[1]-epsilon; y < start[1]+epsilon; y += epsilon){
            
			// set initial conditions
            v[0] = 0;
            v[1] = 0;
            r[0] = x;
            r[1] = y;
    
			// loop over simulation Time
            for(double t = 0; t < Tmax; t += H){
            
				// reset the force
                kraft[0] = 0;
                kraft[1] = 0;
				E = 0;
                
				// calculate the Force from all Magnets 
				for(int i = 0; i < NMag; i++){
                    diff[0] = rMag[i][0] - r[0];
                    diff[1] = rMag[i][1] - r[1];
                    
                    kraft[0] += rMag[i][2] * diff[0]/pow(diff[0]*diff[0] + diff[1]*diff[1] + h*h, 1.5);
                    kraft[1] += rMag[i][2] * diff[1]/pow(diff[0]*diff[0] + diff[1]*diff[1] + h*h, 1.5);
					
					E -= 1* rMag[i][2]/pow(diff[0]*diff[0] + diff[1]*diff[1] + h*h, 0.5);
                }
				double Emag = E;
                
				// add the force due to gravity
                kraft[0] += - K * r[0] - GAMMA * v[0];
                kraft[1] += - K * r[1] - GAMMA * v[1];

				E -= 1/2 * K * (r[0]*r[0] + r[1]*r[1]);
                E += 1/2 * (v[0]*v[0] + v[1]*v[1]);

                // Leap Frog step
                v[0] += H * kraft[0];
                v[1] += H * kraft[1];
                r[0] += H * v[0];
                r[1] += H * v[1];
                
				// print path
				fprintf(file, "%g %g %g %g %g \n", t, r[0], r[1], E, Emag);   
			}
        }// end of y scan
        fprintf(file, "\n");
        
    }// end of x scan

    fclose(file);
    return 0;
}

