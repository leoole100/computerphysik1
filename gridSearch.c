/**
 * @file gridSearch.c
 * @brief creates a 3D velocity grid and evaluates the error function
 * @version 1.0
 * @date 2022-09-20
 * 
 * @author Leon Oleschko
 */

#include "methods.h"

int main(){
	// create file for results
	FILE * results_file = fopen("data/gridSearch.dat", "w+");


	// track max
	double v_best[3];
	double best_error = 1e6;

	//double d = 0.000000001; //stepsize
	double dv = 0.00003; //stepsize
	double step_plus[3] = {15 , 15 , 15};

	double step_minus[3];
	step_minus[0] = step_plus[0];
	step_minus[1] = step_plus[1];
	step_minus[2] = step_plus[2];

	v_start[0]-=step_minus[0]*dv;
	v_start[1]-=step_minus[1]*dv;
	v_start[2]-=step_minus[2]*dv;

	for(int i = 0 ; i < step_minus[0] + step_plus[0] ; i++)
	{	
		v_start[0]+=dv;

		for(int j = 0 ; j < step_minus[1] + step_plus[1] ; j++)
		{
			v_start[1]+=dv;
			for(int k = 0 ; k < step_minus[2] + step_plus[2] ; k++)
			{
				v_start[2]+=dv;
				trajectory(v_start[0],v_start[1],v_start[2], false);
				double currenterr = errfunction();
				fprintf(results_file, "%.10f %.10f %.10f %g \n", v_start[0], v_start[1], v_start[2], currenterr);
				if(currenterr < best_error){
					v_best[0] = v_start[0];
					v_best[1] = v_start[1];
					v_best[2] = v_start[2];
					best_error = currenterr;
				}
			}
			v_start[2]-=(step_minus[2] + step_plus[2])*dv;

			// print progress in percent
			printf("	%02.2f % \n", (double)(i*(step_minus[1] + step_plus[1]) + j)/(step_minus[0] + step_plus[0])/(step_minus[1] + step_plus[1])*100);

		}
		v_start[1]-=(step_minus[1] + step_plus[1])*dv;
	}

	printf("v = { %f, %f, %f } @ %g\n", v_best[0], v_best[1], v_best[2], best_error);

	fclose(results_file);

	printf("creating trajectory\n");
	trajectory( v_best[0], v_best[1], v_best[2], true);

	return 0;
}