/**
 * @file leapFrog.c
 * @brief simulation of the trajectory with the leap frog algorithm
 * @version 0.1
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

__uint8_t getPlanetNumber();
void openPlanetFiles(__uint8_t planet_num, FILE ** planet_files);

// coordinate and velocity of spacecraft
double r[3] = {-4.45081, 2.11055, 0.105762}; // actual data
double v[3] = {-1.0, -0.1, 0.0}; // test data 

double f[3]; // force on spacecraft

int main()
{
	// setup    ////////////////////////////////////////////////////////////////////////////////////////////////////

	// open output file for the trajectory
	FILE * trajectory_file = fopen("data/trajectory.txt", "rw");

	// count number of planets
	__uint8_t planet_num = getPlanetNumber();

	// create planet array
	FILE * planet_files[planet_num];
	openPlanetFiles(planet_num, planet_files);

	// read planet weights
	printf("Planet weights:\n");
	double planet_weights[planet_num-1];
	for (size_t i = 0; i < planet_num; i++){
		fscanf(planet_files[i], "%lf", &planet_weights[i]);
		printf("	%lf\n", planet_weights[i]);
	}

	// read planet cordinates
	printf("Planet cordinates:\n");
	double planet_coords[planet_num-1][3];
	for (size_t i = 0; i < planet_num; i++){
		for (size_t j = 0; j < 3; j++){
			fscanf(planet_files[i], "%lf", &planet_coords[i][j]);
			printf("	%lf\n", planet_coords[i][j]);
		}
	}

	// close files
	for (size_t i = 0; i < planet_num; i++){
		fclose(planet_files[i]);
	}

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