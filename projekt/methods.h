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
//vector to store the deviation of the numerical solution from the boundary conditions
double errvec[3];


// structs  ///////////////////////////////////////////////////////////////////////////////////////////////
__uint8_t getPlanetNumber();
void openPlanetFiles(__uint8_t planet_num, FILE ** planet_files);
double errfunction(double (*r)[3], double (*v)[3]);
void makeJacobian();
void newtonstep();
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

		double r[3], v[3], a[3], tmp[3], checkpoint_dist;

		//initialize boundaryvalues at TMIN
		r[0] = r_start[0];
		r[1] = r_start[1];
		r[2] = r_start[2];
		v[0] = (*v_p)[0];
		v[1] = (*v_p)[1];
		v[2] = (*v_p)[2];


		// check for reaching checkpoints
		double closest_checkpoint_dist[CHECKPOINTS] = {10, 10, 10}; // square of closest distance to checkpoints
		
		//leap frog	
		for(int day = 0; day < TMAX; day++){

			// save current position
			if(save){ fprintf(trajectory_file, "%g %g %g\n", r[0], r[1], r[2]); }


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
			//printf("x_koordinate %f\n", planet_coords[1][0]);



			// calculate distance to checkpoints
			for (size_t i = 0; i < CHECKPOINTS; i++){
				checkpoint_dist = powf(r[0] - r_checkpoints[i][0], 2)+ powf(r[1] - r_checkpoints[i][1], 2) + powf(r[2] - r_checkpoints[i][2], 2);
				if(checkpoint_dist < closest_checkpoint_dist[i]){
					closest_checkpoint_dist[i] = checkpoint_dist;
				}
			}

			
		}
		//printf("closing files\n");	
		// close planet files
		for (size_t i = 0; i < planet_num; i++){
			fclose(planet_files[i]);
		}
		if(save){ fclose(trajectory_file);}

		//return(errfunction(&r, &v));

		// sum closest distances to checkpoints
		double err = 0;
		for (size_t i = 0; i < CHECKPOINTS; i++){
			err += closest_checkpoint_dist[i];
		}

		return err;
}
