/**
 * @file gridSearch.c
 * @brief creates a 3D velocity grid and evaluates the error function
 * @version 0.1
 * @date 2022-09-20
 * 
 * @author Leon Oleschko
 */

#include "methods.h"

int main(){

	// create file for results
	FILE * results_file = fopen("data/gridSearchFine.dat", "w+");

	//double dv[3] = {0.000000001 , 0.000000001 , 0.000000001}; //stepsize
	double dv[3] = {0.0001 , 0.0001 , 0.0001}; //stepsize
	double stepsplus[3] = {15 , 15 , 30};
	double stepsminus[3] = {15 , 15 , 30};

	v_start[0]-=stepsminus[0]*dv[0];
	v_start[1]-=stepsminus[1]*dv[1];
	v_start[2]-=stepsminus[2]*dv[2];

	for(int i = 0 ; i < stepsminus[0] + stepsplus[0] ; i++)
	{	
		v_start[0]+=dv[0];

		for(int j = 0 ; j < stepsminus[1] + stepsplus[1] ; j++)
		{
			v_start[1]+=dv[1];
			for(int k = 0 ; k < stepsminus[2] + stepsplus[2] ; k++)
			{
				v_start[2]+=dv[2];
				double currenterr = fast_trajectory(v_start[0],v_start[1],v_start[2]);
				fprintf(results_file, "%.10f %.10f %.10f %g \n", v_start[0], v_start[1], v_start[2], currenterr);
			}
			v_start[2]-=(stepsminus[2] + stepsplus[2])*dv[2];

			// print progress in percent
			printf("%02.2f % \n", (double)(i*(stepsminus[1] + stepsplus[1]) + j)/(stepsminus[0] + stepsplus[0])/(stepsminus[1] + stepsplus[1])*100);

		}
		v_start[1]-=(stepsminus[1] + stepsplus[1])*dv[1];
	}

}