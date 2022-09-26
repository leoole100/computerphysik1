default:main

trajectory: trajectory.c
	gcc -O3 $< -o $@.out -lm
	./$@.out
	
main: main.c
	gcc -O3 $< -o $@.out -lm
	./$@.out

gridSearchMultithreading: gridSearchMultithreading.c
	gcc -O3 $< -o $@.out -lm -lpthread
	time ./$@.out
	tput bel