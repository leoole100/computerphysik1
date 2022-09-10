all:trajectory optimisation

readPlanetList: readPlanetList.c
	gcc -O3 $< -o $@.out
	./$@.out
	echo ""

trajectory: trajectory.c
	gcc -O3 $< -o $@.out -lm
	./$@.out
	echo ""

optimisation: optimisation.c
	gcc -O3 $< -o $@.out -lm
	./$@.out
	echo ""