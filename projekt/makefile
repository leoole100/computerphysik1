all:shootingmethod

readPlanetList: readPlanetList.c
	gcc -O3 $< -o $@.out
	./$@.out

trajectory: trajectory.c
	gcc -O3 $< -o $@.out -lm
	./$@.out

shootingmethod: shootingmethod.c
	gcc -O3 $< -o $@.out -lm
	./$@.out