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


#define VERLET false // false for leapfrog


// global variables in the heap  ///////////////////////////////////////////////////////////////////////////
//deviation of the numerical solution from the boundary conditions 
double err = 0;
double errvec[3]; // input of jacobian and input of newtonstep

// structs  ///////////////////////////////////////////////////////////////////////////////////////////////
__uint8_t getPlanetNumber();
void openPlanetFiles(__uint8_t planet_num, FILE ** planet_files);
double errfunction(double (*r)[NUM_ENDS][3], double (*v)[NUM_ENDS][3]);
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

		double r[3], r_old[3], v[3], a[3], tmp[3];

		//initialize boundaryvalues at TMIN
		r[0] = r_start[0];
		r[1] = r_start[1];
		r[2] = r_start[2];
		if(VERLET){
			// for verlet:	
			r_old[0] = r_start[0] - (*v_p)[0]/SUB_STEPS;
			r_old[1] = r_start[1] - (*v_p)[1]/SUB_STEPS;
			r_old[2] = r_start[2] - (*v_p)[2]/SUB_STEPS;
		} else {
			// for leapfrog
			v[0] = (*v_p)[0];
			v[1] = (*v_p)[1];
			v[2] = (*v_p)[2];
		}

		// for keeping track of the right boundary condition
		double end_min[NUM_ENDS] = {100,100,100};
		double errpoints[NUM_ENDS][3], errvelocitys[NUM_ENDS][3];
		int errdaycounter = 0;

		// open spacecraft file
		FILE * spacecraft_file = fopen("data/spacecraft.csv", "r+");
		double error = 0;
		//double fehler[NUM_ENDS];
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

				if(VERLET){
					// Verlet step
					tmp[0] = 2.0*r[0]+ pow(SUB_STEPS,-2)*a[0]-v[0];
					tmp[1] = 2.0*r[1]+ pow(SUB_STEPS,-2)*a[1]-v[1];
					tmp[2] = 2.0*r[2]+ pow(SUB_STEPS,-2)*a[2]-v[2];
					v[0] = r[0];
					v[1] = r[1];
					v[2] = r[2];
					r[0] = tmp[0];
					r[1] = tmp[1];
					r[2] = tmp[2];
				} else {
					// Leap Frog step
					v[0] += a[0] / SUB_STEPS;
					v[1] += a[1] / SUB_STEPS;
					v[2] += a[2] / SUB_STEPS;
					r[0] += v[0] / SUB_STEPS;
					r[1] += v[1] / SUB_STEPS;
					r[2] += v[2] / SUB_STEPS;
				}


				/*//Ab hier Randbedingungen unabhängig von Tagen
				for(int l = 0 ; l < NUM_ENDS ; l++)
				{
					double a = fabs(r[0]-r_end[l][0])+fabs(r[1]-r_end[l][1])+fabs(r[2]-r_end[l][2]);+fabs(v[0]-v_end[l][0])+fabs(v[1]-v_end[l][1])+fabs(v[2]-v_end[l][2]);
					if(a < fehler[l])
					{
						errpoints[l][0]=r[0];
						errpoints[l][1]=r[1];
						errpoints[l][2]=r[2];
						errvelocitys[l][0]=v[0];
						errvelocitys[l][1]=v[1];
						errvelocitys[l][2]=v[2];
						fehler[l]=a;
					}
				}
				*/

			}

			// calculate energy
			double energy;
			if(VERLET){
				energy = 0.5*(pow((r[0]-v[0])*SUB_STEPS,2)+pow((r[1]-v[1])*SUB_STEPS,2)+pow((r[2]-v[2])*SUB_STEPS,2)); // verlet
			} else {
				energy = 0.5*(pow(v[0],2)+pow(v[1],2)+pow(v[2],2)); // for newton
			}
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
				if(r_norm < end_min[i]){
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
	//return(errfunction(&errpoints, &errvelocitys));
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