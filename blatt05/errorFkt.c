#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define H 1.e-3
#define XMAX 10

void integ(double E);

void numerov(double *y1, double *y2, double x, double E);

double V(double x);

FILE *file;

int main(){
    
    file = fopen("errorFkt.dat", "w+");
    
    
    for(double E = 0.0; E < 20; E += 0.1){
        integ(E);
    }
    
    //integ(5.0);
    
    
    
    fclose(file);
    
    return 0;
}


void integ(double E){
    
    double y1 = 0, y2 = H;
    for(double x = X_MAX; x > 0; x -= H){
        numerov(&y1, &y2, x, E);
        //fprintf(file, "%f %f \n", x, y1);
    }
    fprintf(file, "%f %f \n", E, y2);
}

void numerov(double *y1, double *y2, double x, double E){
    
    double y = 2* *y2*(1 - H*H*5/12*(E-V(x))) - *y1*(1 - H*H/12*(E-V(x-H)));
    *y1 = *y2;
    *y2 = y;
}


double V(double x){
    return x;
}
