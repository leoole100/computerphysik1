#include <stdio.h>

// DIES GEHoeRT ZU AUFGABENTEIL A)

int main(){
	double v_0 = 1., x_0=0., D=1, M=1.;
	
	double T = 10.;

	int N = 1000;

	double H = T / N;

	double v = v_0, x = x_0, k_1_x, k_1_v, k_2_x, k_2_v, E;	


	for(double t = 0; t < T; t += H){
		
		k_1_v =  -H*D*x/M;
		k_1_x =  H*v;

		k_2_v = -H*D*(x + k_1_x/2)/M;
		k_2_x = H*(v + k_1_v/2);

		v = v + k_2_v;
		x = x + k_2_x;

		E = (M*v*v + D*x*x)/2;

		printf("%g %g %g %g\n", t, x, v, E);

	}

	return(0);

}
