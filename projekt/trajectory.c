/**
 * @file leapFrog.c
 * @brief simulation of the trajectory with the leap frog algorithm
 * @version 1.0
 * @date 2022-09-08
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <unistd.h>
#include <math.h>

#define PLANET_PATH "data/planets/"
#define TMAX 700 // in days
#define SUB_STEPS 1e3

__uint8_t getPlanetNumber();
void openPlanetFiles(__uint8_t planet_num, FILE ** planet_files);

// coordinate and velocity of spacecraft
double r[3] = {0.901865, 0.424962, 0.00748233}; // in AU
//double v[3] = {-0.00937424, 0.0215741, 0.000878397}; // in AU per day
//double v[3] = {-0.002326, -0.000308, -0.003768}; 
double v[3] = {-0.008298, 0.021828, -0.000301};


double G = 1.4882e-34; // in AU^3 / kg day^2
// double m =  367.0; // in kg // kuertzt sich eingenlicht raus
double a[3]; // force on spacecraft divided by mass

double tmp[3];

int main()
{
	// setup    ////////////////////////////////////////////////////////////////////////////////////////////////////
	// open output file for the trajectory
	FILE * trajectory_file = fopen("data/trajectory.dat", "w+");

	// count number of planets
	__uint8_t planet_num = getPlanetNumber();

	// create planet array
	FILE * planet_files[planet_num];
	openPlanetFiles(planet_num, planet_files);

	// read planet weights
	printf("Planet weights:\n");
	double planet_weights[planet_num];
	for (size_t i = 0; i < planet_num; i++){
		fscanf(planet_files[i], "%lf", &planet_weights[i]);
		printf("	%g\n", planet_weights[i]);
	}

	// tweak velocity
	v[0] *= 1.00902;
	v[1] *= 1.00902;
	v[2] *= 0.91;


	// loop    /////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("starting loop\n");
	for(int day = 0; day < TMAX; day++){
		// log progress
		if(day % (TMAX/10) == 0){
			printf("	%g % \n", (100. * day)/TMAX);
		}

		// save current position
		fprintf(trajectory_file, "%g %g %g\n", r[0], r[1], r[2]);

		// get planet positions
		double planet_coords[planet_num][3];
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
	}

	// end    //////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("closing files\n");	
	// close planet files
	for (size_t i = 0; i < planet_num; i++){
		fclose(planet_files[i]);
	}

	// close trajectory file
	fclose(trajectory_file);

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
	printf("found %i planets:\n", planet_num);

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
			printf("	%s\n", tmp_ent->d_name);

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