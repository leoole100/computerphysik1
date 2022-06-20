#include <stdio.h>
#include <math.h>


#define H 0.001
#define rstart 100


double rho(double r);
void numerov(double* y1, double* y2, double r);


int main(){
    
    double y1=0, y2 = 0;
    
    for(double r = rstart; r > 0; r -= H){
        numerov(&y1, &y2, r);
        printf("%f\n", y2);
    }
    
    // Ergebnis:                        -0.039789
    // Analytisch: -1/(8 pi) \approx    -0.0397887
    
    return 0;
}


double rho(double r){
    double wert = exp(-r)/(8 * M_PI);
    return wert;
}

void numerov(double* y1, double* y2, double r){
    double y = 2 * *y2 - *y1 - H*H/12*(rho(r+H)+10*rho(r)+rho(r-H));
    *y1 = *y2;
    *y2 = y;
}
