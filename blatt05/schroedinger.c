#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define H 1.e-3
#define XMAX 10.
#define N ((int) (XMAX/H))

#define UNCERT_END 1.e-10

#define UNCERT_GUESS 0.2
double guess[] = {2.5, 4.5, 6, 7.5, 8.75, 10, 12, 14, 16, 18.5};

void integ(double E, double *phi);
void numerov(double *y1, double *y2, double x, double E);
void findAndExport(int n);
double V(double x);



int main(){
    
    for (size_t i = 0; i < sizeof(guess)/ sizeof(guess[0]); i++){
		findAndExport(i);
	}
	
	
    
    
    return 0;
}

void findAndExport(int n){
	double phi[N];

	// bisection to find the root
	double E0 = guess[n]-UNCERT_GUESS;
	double E1 = guess[n]+UNCERT_GUESS;
	double Ex, tmp;
	do {
		Ex = (E1+E0)/2.0;
		
		integ(Ex,phi);
		tmp = phi[N-1];
		integ(E0,phi);

		if(tmp*phi[N-1]>0)
			E0=Ex;
		else
			E1=Ex;
	} while (fabs((E1-E0)/E0)>UNCERT_END);

	printf("guess: %.2f E: %f\n", guess[n], E1);

	// normalizing
	double norm = 0;
	for(int i=0; i<N; i++){
		norm += phi[i]*phi[i]*H;
	}
	norm = sqrt(norm);
	for(int i=0; i<N; i++){
		phi[i] /= norm;
	}

	// exporting
	FILE *file;
	char filename[10];
	snprintf(filename, 10, "out%02d.dat", n);
	printf("saving in: %s\n", filename);
    file = fopen(filename, "w+");
	fprintf(file, "%f\n", E1);
	for(int i=0; i<N; i++){
		fprintf(file, "%f %f\n", i*H, phi[i]);
	}
	fclose(file);
}

void integ(double E, double *phi){
    
    double y1 = 0, y2 = H;
    for(int i=0; i < N; i++){
        numerov(&y1, &y2, i * H, E);
        phi[i] = y2;
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
