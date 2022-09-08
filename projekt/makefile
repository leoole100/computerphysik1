all:leapFrog

readPlanetList: readPlanetList.c
	gcc -O3 $< -o $@.out
	./$@.out

leapFrog: leapFrog.c
	gcc -O3 $< -o $@.out -lm
	./$@.out