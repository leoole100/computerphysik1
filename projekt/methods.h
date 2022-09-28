/**
 * @file methods.h
 * @brief Commonly used methods
 * @version 0.1
 * @date 2022-09-20
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <unistd.h>
#include <math.h>
#include <stdbool.h> // why isn't this in c?
#include "setup.h"


// global variables in the heap  ///////////////////////////////////////////////////////////////////////////
//deviation of the numerical solution from the boundary conditions 
double err = 0;
//Jacobian matrix of the errorfunction 
double Jacobian[3][3];
//counter of the newton steps
int newtoniterationnumber = 1;
double errvec[3]; // input of jacobian and input of newtonstep

// structs  ///////////////////////////////////////////////////////////////////////////////////////////////
__uint8_t getPlanetNumber();
void openPlanetFiles(__uint8_t planet_num, FILE ** planet_files);
double errfunction(double (*r)[NUM_ENDS][3], double (*v)[NUM_ENDS][3]);
void makeJacobian();
double newtonstep(int newtoniterationnumber);
double trajectory(double (*v_p)[3],  bool save);

// functions  //////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Get the number of files in the PLANET_PATH
 * 
 * @return __uint8_t 
 * 
 * @author Leon Oleschko
 */
__uint8_t getPlanetNumber(){
	__uint8_t planet_num = 0;

	// count number of planets
	DIR *d = opendir(PLANET_PATH);
	if(d == NULL){
		perror("opendir");
	}
	struct dirent *tmp_ent;
	while ((tmp_ent = readdir(d)) != NULL){
		// check if real file (not /. or /..)
		if (tmp_ent->d_type == DT_REG){
			planet_num++;
		}
	}
	closedir(d);
	//printf("found %i planets:\n", planet_num);

	return planet_num;
}

/**
 * @brief open files to file array
 * 
 * @param planet_num 
 * @param planet_files 
 * 
 * @author Leon Oleschko
 */
void openPlanetFiles(__uint8_t planet_num, FILE ** planet_files){
	// get file names
	DIR *d = opendir(PLANET_PATH);
	__uint8_t i = 0;
	struct dirent *tmp_ent;
	while ((tmp_ent = readdir(d)) != NULL){
		
		// check if regular file (not /. or /..)
		if (tmp_ent->d_type == DT_REG){
			//printf("	%s\n", tmp_ent->d_name);

			// get path to file
			char path[strlen(PLANET_PATH) + strlen(tmp_ent->d_name) + 1];
			strcpy(path, PLANET_PATH);
			strcat(path, tmp_ent->d_name);

			// open file and save pointer in array
			planet_files[i] = fopen(path, "r");
			if (planet_files[i] == NULL){
				perror("fopen");
			}
			
			i++;
		}
	}
	closedir(d);
}	

/**
 * @brief simulates the trajectory from start to TMAX
 * 
 * @param v_x 
 * @param v_y 
 * @param v_z 
 * @param save specify if the trajectory should be saved, will run to TMAX
 * 
 * @author Leon Oleschko
 */

/**
 * @brief simulates the trajectory from start to TMAX
 * 
 * @param v_p 
 * @param save specify if the trajectory should be saved, will run to TMAX
 * @return double error function at TMAX
 * 
 * @author Leon Oleschko
 */
double trajectory(double (*v_p)[3],  bool save)
{	
		FILE * trajectory_file;

		if(save){ trajectory_file = fopen("data/trajectory.dat", "w+"); }

		// count number of planets
		__uint8_t planet_num = getPlanetNumber();

		// create planet array
		FILE * planet_files[planet_num];
		openPlanetFiles(planet_num, planet_files);

		// read planet weights
		//printf("Planet weights:\n");
		double planet_weights[planet_num];
		for (size_t i = 0; i < planet_num; i++){
			if(fscanf(planet_files[i], "%lf", &planet_weights[i])==0){
				perror("fscanf");
				exit(EXIT_FAILURE);
			}
			//printf("	%g\n", planet_weights[i]);
		}

		// read planet positions
		double planet_coords[planet_num][3], planet_coords_current[planet_num][3], planet_coords_next[planet_num][3];
		for (size_t i = 0; i < planet_num; i++){
			for (size_t j = 0; j < 3; j++){
				if(fscanf(planet_files[i], "%lf", &planet_coords_next[i][j])==0){
					perror("fscanf");
					exit(EXIT_FAILURE);
				}
				//printf("	%lf\n", planet_coords[i][j]);
			}
		}

		double r[3], v[3], a[3], tmp[3];

		//initialize boundaryvalues at TMIN
		r[0] = r_start[0];
		r[1] = r_start[1];
		r[2] = r_start[2];
		v[0] = (*v_p)[0];
		v[1] = (*v_p)[1];
		v[2] = (*v_p)[2];

		// for keeping track of the right boundary condition
		double end_min[NUM_ENDS] = {100,100,100};
		double errpoints[NUM_ENDS][3], errvelocitys[NUM_ENDS][3];
		int errdaycounter = 0;

		// open spacecraft file
		FILE * spacecraft_file = fopen("data/spacecraft.csv", "r+");
		double error = 0;

		//leap frog	
		for(int day = 0; day < TMAX; day++){

			// save current planet positions
			for (size_t i = 0; i < planet_num; i++){
				for (size_t j = 0; j < 3; j++){
					planet_coords_current[i][j] = planet_coords_next[i][j];
				}
			}

			// get planet positions 
			for (size_t i = 0; i < planet_num; i++){
				for (size_t j = 0; j < 3; j++){
					if(fscanf(planet_files[i], "%lf", &planet_coords_next[i][j])==0){
						perror("fscanf");
						exit(EXIT_FAILURE);
					}
					//printf("	%lf\n", planet_coords[i][j]);
				}
			}

			// sup steps
			for(size_t sub_step = 0; sub_step < SUB_STEPS; sub_step++){	
				// reset force
				a[0] = 0;
				a[1] = 0;
				a[2] = 0;

				// calculate interpolated position of planets
				for (size_t i = 0; i < planet_num; i++){
					// calculate interpolated position
					for (size_t j = 0; j < 3; j++){
						planet_coords[i][j] = planet_coords_current[i][j] + (planet_coords_next[i][j] - planet_coords_current[i][j]) * (double)sub_step / (double)SUB_STEPS;
					}
				}

				// calculate force on spacecraft
				for (size_t i = 0; i < planet_num; i++){
					// calculate distance
					tmp[0] = planet_coords[i][0] - r[0];
					tmp[1] = planet_coords[i][1] - r[1];
					tmp[2] = planet_coords[i][2] - r[2];

					// calculate force
					double r_norm = sqrt(tmp[0]*tmp[0] + tmp[1]*tmp[1] + tmp[2]*tmp[2]);
					double fac = G*planet_weights[i]/powf(r_norm, 3);
					a[0] += fac*tmp[0];
					a[1] += fac*tmp[1];
					a[2] += fac*tmp[2];
				}
				// print force
				//printf("%lf %lf %lf\n", a[0], a[1], a[2]);

				// Leap Frog step
				v[0] += a[0] / SUB_STEPS;
				v[1] += a[1] / SUB_STEPS;
				v[2] += a[2] / SUB_STEPS;
				r[0] += v[0] / SUB_STEPS;
				r[1] += v[1] / SUB_STEPS;
				r[2] += v[2] / SUB_STEPS;

			}

			// calculate energy
			double energy = 0.5*(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
			for (size_t i = 0; i < planet_num; i++){
				// calculate distance
				tmp[0] = planet_coords_next[i][0] - r[0];
				tmp[1] = planet_coords_next[i][1] - r[1];
				tmp[2] = planet_coords_next[i][2] - r[2];

				// calculate energy
				double r_norm = sqrt(tmp[0]*tmp[0] + tmp[1]*tmp[1] + tmp[2]*tmp[2]);
				energy += -G*planet_weights[i]/r_norm;
			}

			// save current position
			if(save){ fprintf(trajectory_file, "%g %g %g %g\n", r[0], r[1], r[2], energy); }

			// FOR ERROR FUNCTION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// add to error integral
			// read spacecraft position
			double spacecraft_coords[3];
			for (size_t j = 0; j < 3; j++){
				if(fscanf(spacecraft_file, "%lf", &spacecraft_coords[j])==0){
					perror("fscanf");
					exit(EXIT_FAILURE);
				}
			}
			error += (pow(r[0]-spacecraft_coords[0],2) + pow(r[1]-spacecraft_coords[1],2) + pow(r[2]-spacecraft_coords[2],2))/TMAX;
			errvec[0] += pow(r[0]-spacecraft_coords[0],2)/TMAX;
			errvec[1] += pow(r[1]-spacecraft_coords[1],2)/TMAX;
			errvec[2] += pow(r[2]-spacecraft_coords[2],2)/TMAX;


			
			// check error at specific days
			/*if(day == end_days[errdaycounter])
			{
				for(int i = 0 ; i < 3 ; i++)
				{
					errpoints[errdaycounter][i] = r[i];
					errvelocitys[errdaycounter][i] = v[i];
				}
				errdaycounter++;
			}*/

				
			// check error at closest to specific postions
			/*for (size_t i = 0; i < NUM_ENDS; i++){
				// calculate distance between r_end[i] and r
				tmp[0] = r_end[i][0] - r[0];
				tmp[1] = r_end[i][1] - r[1];
				tmp[2] = r_end[i][2] - r[2];
				double r_norm = tmp[0]*tmp[0] + tmp[1]*tmp[1] + tmp[2]*tmp[2];

				// check if distance is smaller than current minimum
				if(r_nor m < end_min[i]){
					end_min[i] = r_norm;
					errpoints[i][0] = r[0];
					errpoints[i][1] = r[1];
					errpoints[i][2] = r[2];
					errvelocitys[i][0] = v[0];
					errvelocitys[i][1] = v[1];
					errvelocitys[i][2] = v[2];
				}
			}*/
		}

		// close planet files
		for (size_t i = 0; i < planet_num; i++){
			fclose(planet_files[i]);
		}
		if(save){ fclose(trajectory_file);}

		fclose(spacecraft_file);

		//print errpoints
		/*printf("errpoints:\n	{");
		for (size_t i = 0; i < NUM_ENDS; i++){
			printf("{%lf, %lf, %lf}, ", errpoints[i][0], errpoints[i][1], errpoints[i][2]);
		}
		printf("}\n");*/


		//return(errfunction(&errpoints, &errvelocitys));

		//close spacecraft file
		return(error);
}

/**
 * @brief returns the value for the error function
 * 
 * @param r 
 * @param v 
 * @return double 
 */
double errfunction(double (*r)[NUM_ENDS][3], double (*v)[NUM_ENDS][3])
{
	/*
	valculates errorvector:
	
	 errvec = v_end,boundaryconditions - v_end,numericalsolution + r_end,boundaryconditions-r_end,numericalsolution
	
	inputs:
		None

	outputs :
		abserr : double
		absolute value of the errorvector

	————————————————————————————————————————————————
	*/

	for(int i = 0 ; i < 3 ; i++){
		errvec[i] = 0;
		for(int j = 0 ; j < NUM_ENDS ; j++){
			errvec[i] += powf(r_end[j][i]-(*r)[j][i],2) + powf(v_end[j][i]-(*v)[j][i],2);
			errvec[i] += fabs(r_end[j][i]-(*r)[j][i]);
		}
	}
		
	// calculate magnitude square of error vector
	double abserr = powf(errvec[0],2)+powf(errvec[1],2)+powf(errvec[2],2);
	//printf("%lf\n",abserr);
	return abserr;
}

/**
 * @brief 
 * 
 * @author Moritz Schroer
 */
void calcJacobian()
{	
	/*
    vec(v_n)=v_start^(n)
	Computes Jacobian Matrix Derr(vec(v_n)) by calculating the partial dericatives with difference quotient:

	d err_i/d x = (errvec(v_start[0] + h * e_x , v_start[1] , v_start[2]) - errvec(v_start[0] , v_start[1] , v_start[2])) / h
	d err_i/d y = (errvec(v_start[0] , v_start[1] + h *e_y , v_start[2]) - errvec(v_start[0] , v_start[1] , v_start[2])) / h
	d err_i/d z = (errvec(v_start[0] , v_start[1] , v_start[2] + h *e_z) - errvec(v_start[0] , v_start[1] , v_start[2])) / h

	and then resets errvec = err(v_start[0] , v_start[1] , v_start[2])

	inputs:
		None

	outputs :
		None

	————————————————————————————————————————————————
	*/

	for(int j = 0 ; j < 3 ; j++ )
	{
		
		v_start[j]+=h;
		err = trajectory(&v_start, false);

		for(int i = 0 ;  i < 3 ; i++)
		{
			Jacobian[i][j]=errvec[i];
		}
		v_start[j]-=h;

		err = trajectory(&v_start, false);
		for(int i = 0 ; i < 3 ; i++)
		{
			Jacobian[i][j]-=errvec[i];
			Jacobian[i][j]/=h;
		}
		
	}
}

/**
 * @brief 
 * 
 * @author Moritz Schroer
 */
double newtonstep(int ni)
{
	/*
	v_start^(n) := (v_start[0] , v_start[1] , v_start[2])

	Calculates Newtonstep v_start^(n+1) = v_start^(n) - (Derr(v_start^(n)))^-1 * err(v_start^(n))	

	Therefore the following System of equations is solved with Jacobi iteration (deltav_n := (delta v_x , delta v_y , delta v_z)):

	Derr(v_start^(n)) * deltav_n    =     -err(v_start^(n)) 
	
	then set:

	v_start^(n+1) = v_start^(n) + deltav_n

	inputs:
		None
	outputs:
		None

	—————————————————————————————————————————————————-
	*/


    /* numerical solution of equation system (doesn't work)
    -----------------------------------------------------------------------------------------------------------
    //calculate Jacobian Matrix Derr of the errorfunction
	calcJacobian();

	//delta_s vector to solve the linear system of equation with Jacobi iteration method
	double deltav_n[3] = {-0. , -0.0 , -0.0};
	double deltav_np[3] = {0 , 0 , 0}; 
	//sles := Derr*deltas, if | sles- |^2 < 1e-7 break; because linear equation system is solved.
	
	double abserrles = 100;
	//initial guess of the solution of the linear system of equations
	do{	
		
			for(int j = 0 ; j < 3 ; j++)
			{
				printf("errvec[%d] = %lf\n", j, errvec[j]);
			
		}

		//do Jacobi step
		deltav_np[0] =  (-errvec[0]-Jacobian[0][1]*deltav_n[1]-Jacobian[0][2]*deltav_n[2])/Jacobian[0][0]; 
		deltav_np[1] =  (-errvec[1]-Jacobian[1][0]*deltav_n[0]-Jacobian[1][2]*deltav_n[2])/Jacobian[1][1]; 
		deltav_np[2] =  (-errvec[2]-Jacobian[2][0]*deltav_n[0]-Jacobian[2][1]*deltav_n[1])/Jacobian[2][2]; 
		deltav_n[0] = deltav_np[0];
		deltav_n[1] = deltav_np[1];
		deltav_n[2] = deltav_np[2];
		

        double sles[3] = {0.0 , 0.0 , 0.0};
		//calculate Derr(v_start^(n))*(s_x,s_y,s_z)
		for(int i = 0 ; i < 3 ; i++)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				sles[i]+=(Jacobian[i][j]*deltav_n[j]);
			}
		}
		abserrles = (sles[0]+errvec[0])*(sles[0]+errvec[0])+(sles[1]+errvec[1])*(sles[1]+errvec[1])+(sles[2]+errvec[2])*(sles[2]+errvec[2]);
		//check if abserrless is finte else exit programm and display error message
		if(isinf(abserrles) || isnan(abserrles))
		{
			printf("error couldnt perform Newtonstep abserror exceeds limits %lf\n",abserrles);
			exit(0);
		}
	}while(abserrles > powf(10,-8));
	----------------------------------------------------------------------------------------------------------------------------*/

        //calculate Jacobian Matrix Derr of the errorfunction
	calcJacobian();

		
  	double solution[3] = {0 , 0 , 0};

	//Gauss Algorithmus für Dreiecksform
	for(int i = 0 ; i < 3 ; i++)
	{   
		double fac1 = Jacobian[i][i];
		if(Jacobian[i][i] != 0)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				Jacobian[i][j]/=fac1;
			}
			errvec[i]/=fac1;
		}
		for(int j = i+1 ; j < 3 ; j++)
		{
			double fac2 = Jacobian[j][i];
			errvec[j]-=errvec[i]*fac2;
			for(int k = 0 ; k < 3 ; k++)
			{   
				Jacobian[j][k]-=Jacobian[i][k]*fac2;
			}
		}
	}

	//solve System 
	for(int i = 2 ; i > 0 ; i--)
	{
		for(int j = 0 ; j < i ; j++)
		{
			double fac = Jacobian[j][i];                   
			Jacobian[j][i]=0;
			errvec[j]-=errvec[i]*fac;
		}
	}

	double gain = .1;

	v_start[0]-=gain*errvec[0];
	v_start[1]-=gain*errvec[1];
	v_start[2]-=gain*errvec[2];

	//solve boundary value problem with new initial velocity
	double abserror = trajectory(&v_start, false);
	printf("%4d. iteration: v_start = {%.10f, %.10f, %.10f}, err = %e\n", ni,v_start[0],v_start[1],v_start[2], abserror);
    return(abserror);
}
