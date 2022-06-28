all: multithreading test poisson poisson2 summation vorlesung schroedinger errorFkt

multithreading: multithreading.c
	gcc -O3 $< -o $@ -Wall -lm -pthread
	./$@

test: test.c
	gcc -O3 $< -o $@ -Wall -lm -pthread
	./$@

poisson: poisson.c
	gcc -O3 $< -o $@ -Wall -lm -pthread
	./$@

poisson2: poisson2.c
	gcc -O3 $< -o $@ -Wall -lm -pthread
	./$@

summation: summation.c
	gcc -O3 $< -o $@ -Wall -lm -pthread
	./$@


vorlesung: vorlesung.c
	gcc -O3 $< -o $@ -Wall -lm -pthread
	./$@


schroedinger: schroedinger.c
	gcc -O3 $< -o $@ -Wall -lm -pthread
	./$@


errorFkt: errorFkt.c
	gcc -O3 $< -o $@ -Wall -lm -pthread
	./$@
