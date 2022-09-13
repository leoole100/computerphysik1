/**
 * @file optimisation.c
 * @brief optimizing the starting velocity wit the shooting method
 * @version 0.1
 * @date 2022-09-10
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <unistd.h>
#include <math.h>

#define PLANET_PATH "data/planets/"
#define TMIN 470
#define TMAX 520 // in days
#define SUB_STEPS 1e3

__uint8_t getPlanetNumber();
void openPlanetFiles(__uint8_t planet_num, FILE ** planet_files);

//boundary conditions of Dirichlet Problem (r=r(t_0), r_end=r(t_end))
double r_start[3] = {-4.89012662928316,2.06668956508917,0.107497865687179};

double r_end[3] = {-4.99420978998632,2.0533814621652,-0.038641227220261};

//initial guess for velocity v(r_0)
double v_start[3] = {-0.00917009, -0.00114656, 0.0001619}; // in AU per day

//arrays for calculated values
double r[3];
double v[3];

//new initial guess of v, after each shooting step v_p = v-(r_num(t_end)-r_end)/(t_end-t_0)
double v_p[3];

double G = 1.4882e-34; // in AU^3 / kg day^2
// double m =  367.0; // in kg // kuertzt sich eingenlicht raus
double a[3]; // force on spacecraft divided by mass

double tmp[3];

int main()
{
	// loop    /////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("starting loop\n");
	//start with given initialguess of v_start
	for(int iterationcounter = 0; iterationcounter < 1000	; iterationcounter++ ){
		
		// setup    ////////////////////////////////////////////////////////////////////////////////////////////////////
		// open output file for the trajectory
		FILE * trajectory_file = fopen("data/trajectory.dat", "w+");

		// count number of planets
		__uint8_t planet_num = getPlanetNumber();
		
			//initialize r(t_0) 
			r[0]=r_start[0];
			r[1]=r_start[1];
			r[2]=r_start[2];
			//initialize v(t_0)
			v[0]=v_start[0];
			v[1]=v_start[1];
			v[2]=v_start[2];
			
		//leap frog	
		for(int day = TMIN; day < TMAX; day++){

			// create planet array
			FILE * planet_files[planet_num];
			openPlanetFiles(planet_num, planet_files);

			// read planet weights
			
			double planet_weights[planet_num];
			for (size_t i = 0; i < planet_num; i++){
				fscanf(planet_files[i], "%lf", &planet_weights[i]);
			}

		// log progress
		if(day % (TMAX/10) == 0){
			printf("	%g % \n", (100. * day)/TMAX);
		}

		// save current position
		fprintf(trajectory_file, "%g %g %g\n", r[0], r[1], r[2]);

		double planet_coords[planet_num][3];
		//skip rows until number of row == day
			for(int row = 0 ; row < day ; row++){
				for (size_t i = 0; i < planet_num; i++)
					{
					for (size_t j = 0; j < 3; j++){
						fscanf(planet_files[i], "%lf", &planet_coords[i][j]);
					}
					}
					//printf("day %i", row);
			}

		// get planet positions 
		for (size_t i = 0; i < planet_num; i++){
			for (size_t j = 0; j < 3; j++){
				fscanf(planet_files[i], "%lf", &planet_coords[i][j]);
				//printf("	%lf\n", planet_coords[i][j]);
			}
		}

		// sup steps
		for(size_t sub_step = 0; sub_step < SUB_STEPS; sub_step++){	
			// reset force
			a[0] = 0;
			a[1] = 0;
			a[2] = 0;

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
		//printf("closing files\n");	
		// close planet files
		for (size_t i = 0; i < planet_num; i++){
			fclose(planet_files[i]);
			}
		}
		//dist of numerical solution to exact solution error=|r_end-r_num(t_end)|
		double error = sqrt(powf(r[0]-r_end[0],2)+powf(r[1]-r_end[1],2)+powf(r[2]-r_end[2],2));
		//print numbr of iterationstep and associated error
		printf("%i. iteration: error = ", iterationcounter);
		printf("%f AU\n",error);
		fclose(trajectory_file);

		//if |r_end-r_num(t_end)|<1e-6 break; and print last initial guesss of v_start
		//else do one more iteration and repeat leap frog procedure for the new initial
		//guess of v_start
		if(error<1e-6){
			printf("optimal solution found. Initital velocity is: \n");
			printf("v_x = %f AU/day\n",v_start[0]);
			printf("v_y = %f AU/day\n",v_start[1]);
			printf("v_z = %f AU/day\n",v_start[2]);
			break;
		}else{
			v_p[0] = v_start[0]-(r[0]-r_end[0])/(TMAX-TMIN);
			v_p[1] = v_start[1]-(r[1]-r_end[1])/(TMAX-TMIN);
			v_p[2] = v_start[2]-(r[2]-r_end[2])/(TMAX-TMIN);
			v_start[0] = v_p[0];
			v_start[1] = v_p[1];
			v_start[2] = v_p[2];
		}
		//if max number of iterationsteps > 100 print optimal solution not found
		if(iterationcounter >= 100){
			printf("optimal solution not found\n");
		}
	}
	// end    //////////////////////////////////////////////////////////////////////////////////////////////////////

	return(0);
}

// get the number of files in the planets directory
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

// open all planet files
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
