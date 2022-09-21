all:shootingmethod

trajectory: trajectory.c
	gcc -O3 $< -o $@.out -lm
	./$@.out

shootingmethod: shootingmethod.c
	gcc -O3 $< -o $@.out -lm
	./$@.out
	
main: main.c
	gcc -O3 $< -o $@.out -lm
	./$@.out
	echo ""

gridSearch: gridSearch.c
	gcc -O3 $< -o $@.out -lm
	./$@.out


gridSearchMultithreading: gridSearchMultithreading.c
	gcc -O3 $< -o $@.out -lm
	./$@.out