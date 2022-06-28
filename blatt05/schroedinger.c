#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define H 1.e-3
#define XMAX 10

#define UNCERT_END 1.e-10

#define UNCERT_GUESS 0.2
double guess[] = {2.5, 4.5, 6, 7.5, 8.75, 10, 12, 14, 16, 18.5};

double integ(double E, bool save);
void numerov(double *y1, double *y2, double x, double E);
void findAndExport(int n);
double V(double x);

FILE *file;

int main(){
    
    file = fopen("schroedinger.dat", "w+");
    
    
	findAndExport(0);
    
    
    
    fclose(file);
    
    return 0;
}

void findAndExport(int n){

	// bisection to find the root
	double E0 = guess[n]-UNCERT_GUESS;
	double E1 = guess[n]+UNCERT_GUESS;
	double Ex, tmp;
	do {
		Ex = (E1+E0)/2.0;
		
		tmp = integ(Ex,false);
	
		if(tmp*integ(E0,false)>0)
			E0=Ex;
		else
			E1=Ex;
	} while (fabs((E1-E0)/E0)>UNCERT_END);

	printf("guess: %.2f E: %f\n", guess[n], E1);

}

double integ(double E, bool save){
    
    double y1 = 0, y2 = H;
    for(double x = XMAX; x > 0; x -= H){
        numerov(&y1, &y2, x, E);
        
		if(save){
			fprintf(file, "%f %f \n", x, y1);
		}
    }
	return y2;
}

void numerov(double *y1, double *y2, double x, double E){
    
    double y = 2* *y2*(1 - H*H*5/12*(E-V(x))) - *y1*(1 - H*H/12*(E-V(x-H)));
    *y1 = *y2;
    *y2 = y;
}


double V(double x){
    return x;
}
