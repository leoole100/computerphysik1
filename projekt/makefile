default: main

trajectory: trajectory.c
	gcc -O3 $< -o $@.out -lm
	./$@.out
	

main: newton

newton: main.c
	gcc -O3 $< -o $@.out -lm
	./$@.out

gridSearchMultithreading: gridSearchMultithreading.c
	gcc -O3 $< -o $@.out -lm -lpthread
	time ./$@.out
	tput bel

	
gradientDescent: gradientDescent.c
	gcc -O3 $< -o $@.out -lm -fopenmp
	./$@.out

newtonHesseOMP: newtonHesseOMP.c
	gcc -O3 $< -o $@.out -lm -fopenmp
	./$@.out

