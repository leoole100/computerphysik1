all:main

trajectory: trajectory.c
	gcc -O3 $< -o $@.out -lm
	./$@.out

	
main: main.c
	gcc -O3 $< -o $@.out -lm
	./$@.out

gridSearch: gridSearch.c
	gcc -O3 $< -o $@.out -lm
	./$@.out

gridSearchMultithreading: gridSearchMultithreading.c
	gcc -O3 $< -o $@.out -lm
	./$@.out