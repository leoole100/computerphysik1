#include <stdio.h>
#include <math.h>

#define Nmag 4
#define N 10000
#define H 0.01
#define GAMMA 1
#define K 0.1
#define h 1



double rmag[Nmag][3]; // X- und Y-Koordinaten und Magnetstärke
double T = N * H;
double r[2], v[2];
double kraft[2], diff[2];

int main(){
    
    for(int i = 0; i < Nmag; i++){
        double winkel = 2*M_PI*i/Nmag;
        rmag[i][0] = cos(winkel);
        rmag[i][1] = sin(winkel);
        rmag[i][2] = 1.;
        //printf("x: %g \t\t y: %g \t\t Stärke: %g \n", rmag[i][0], rmag[i][1], rmag[i][2]);
    }
    
    v[0] = 0;
    v[1] = 0;
    
    r[0] = 0.6;
    r[1] = 0.3;
    
    
    for(double t = 0; t < T; t += H){
        
        // JETZT RECHNEN WIR DIE KRAFT AUS =)
        
        kraft[0] = 0;
        kraft[1] = 0;
        
        for(int i = 0; i < Nmag; i++){
            
            diff[0] = rmag[i][0] - r[0];
            diff[1] = rmag[i][1] - r[1];
            
            kraft[0] += rmag[i][2] * diff[0]/pow(diff[0]*diff[0] + diff[1]*diff[1] + h*h, 1.5);
            kraft[1] += rmag[i][2] * diff[1]/pow(diff[0]*diff[0] + diff[1]*diff[1] + h*h, 1.5);
            
        }
        
        
        kraft[0] += - K * r[0] - GAMMA * v[0];
        kraft[1] += - K * r[1] - GAMMA * v[1];
        
        // Leap Frog
        v[0] += H * kraft[0];
        v[1] += H * kraft[1];
        
        r[0] += H * v[0];
        r[1] += H * v[1];
        
        printf("%g %g %g\n", t, r[0], r[1]);
        
    }
    
    
    
    return 0;
}

