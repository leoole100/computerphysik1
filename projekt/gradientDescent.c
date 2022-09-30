/**
 * @file main.c
 * @brief 
 * @version 0.1
 * @date 2022-09-28
 * 
 * @author Leon Oleschko
 * 
 */

#include "methods.h"
#include <omp.h>

double gradient(double (*v_p)[3], double (*gradient)[3]);

int main(){
	
	// adding symmetric random noise to the initial velocity
	/*double randomInterval[3] = {0.001, 0.01, 0.0001};
	v_start[0] += randomInterval[0] * (2.0 * rand() / RAND_MAX - 1.0);
	v_start[1] += randomInterval[1] * (2.0 * rand() / RAND_MAX - 1.0);
	v_start[2] += randomInterval[2] * (2.0 * rand() / RAND_MAX - 1.0);

	printf("v_start = {%.15f, %.15f, %.15f}\n\n", v_start[0],v_start[1],v_start[2]);*/


	double err = 100;
	unsigned int iter = 1;

	double v_smallest[3];
	double err_smallest = err;

	do{
		double grad[3];
		err = gradient(&v_start, &grad);

		//printf("gradient = {%.15f, %.15f, %.15f}\n", grad[0], grad[1], grad[2]);

		printf("%4d. iteration: v_start = {%.10f, %.10f, %.10f}, err = %e / %e\n", iter,v_start[0],v_start[1],v_start[2], err, err_smallest);

		if(err<err_smallest){
			err_smallest = err;
			v_smallest[0] = v_start[0];
			v_smallest[1] = v_start[1];
			v_smallest[2] = v_start[2];
		}

		double gain = 1e-13;
		v_start[0] -= gain * grad[0];
		v_start[1] -= gain * grad[1];
		v_start[2] -= gain * grad[2];
		
		iter++;
		
	}while(err > NEWTON_PRECISION && iter < NEWTON_STEPS);

	// save the trajectory
	err = trajectory(&v_smallest, true);
	printf("\n optimal: v_start = {%.15f, %.15f, %.15f}, err = %e\n", v_smallest[0],v_smallest[1],v_smallest[2], err_smallest);	
	
	return(0);
}

/**
 * @brief calculates the gradient of the error function
 * 
 * @param v_p pointer to the start velocity
 * @param gradient pointer to the resulting gradient vector
 * @return double error at velocity vector
 *
 * @author Leon Oleschko
 *
 */
double gradient(double (*v_p)[3], double (*gradient)[3]){

	double error[4];

	#pragma omp parallel for
	for(int i = 0 ; i < 4 ; i++){
		double v[3];
		v[0] = (*v_p)[0];
		v[1] = (*v_p)[1];
		v[2] = (*v_p)[2];
		if(i<3){
			v[i] += h;
		}
		error[i] = trajectory(&v, false);
	}

	(*gradient)[0] = (error[0] - error[3])/h;
	(*gradient)[1] = (error[1] - error[3])/h;
	(*gradient)[2] = (error[2] - error[3])/h;

	return error[3];
}