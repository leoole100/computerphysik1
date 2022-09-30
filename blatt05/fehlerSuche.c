#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define H 1.e-4
#define XMAX 15.
#define EXPORT_RES 0.1

void integ(double E);

void numerov(double *y1, double *y2, double x, double E);

double V(double x);

FILE *file;

int main(){
    
    file = fopen("fehlerSuche.dat", "w+");
    
    
	integ(2.57333);

    
    fclose(file);
    
    return 0;
}


void integ(double E){
    
    double y1 = 0, y2 = H;
    for(double x = XMAX; x > 0; x -= H){
        numerov(&y1, &y2, x, E);
		fprintf(file, "%f %f \n", x, y1);
	}
}

void numerov(double *y1, double *y2, double x, double E){
    
    double y = 2* *y2*(1 - H*H*5/12*(E-V(x))) - *y1*(1 - H*H/12*(E-V(x-H)));
    *y1 = *y2;
    *y2 = y;
}


double V(double x){
    return x;
}
