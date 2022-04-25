#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <inttypes.h>



//void add(int[] number1, int[] number2)


int main(){
    //numbers = new int[10000];
	long double f_1=0, f_2=1, f_3;

	for(int i=0; i < 20000; i++){
		//printf("F%d: %Lf		F%d/F%d: %Lf\n\n", i, f_2, i, i-1, f_2/(long double)f_1);
        //double gold =  f_2/(double)f_1;
		//printf("F%i: %llu", i, f_2);
        //printf("        F%d/F%d: %g\n", i, i-1,gold);
        
        f_3 = f_1 + f_2;
		

		if(f_2 < f_1){
			printf("\n\nI maximal: %i\n\n\n",i);
			break;
		}
		
		f_1 = f_2;
		f_2 = f_3;
	}

	long double phi = 1;
	
	int N = 10000;
	
	for(int i = 0; i < N; i++)
		phi = 1 + 1/phi;

	printf("Phi g:     %0.55Lf \nPhi i:     %0.70Lf \nPhi-b:       %0.55Lf\n\n\n\n\n", (long double)(1+sqrt(5))/2, phi, f_2/(long double)f_1);
	
	return 0;

    //1.6180339887498949025257388711906969547271728515625
}
