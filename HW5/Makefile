FLAGS = -Wall -fopenmp -lm

build:
	gcc histogram_dynamic.c -o histogram_dynamic ${FLAGS}
	gcc histogram_static.c -o histogram_static ${FLAGS}
	gcc omp_trap1.c -o omp_trap1 ${FLAGS}

histogram_dynamic: histogram_dynamic.c
	gcc histogram_dynamic.c -o histogram_dynamic ${FLAGS}
histogram_static: histogram_static.c
	gcc histogram_static.c -o histogram_static ${FLAGS}
trap: omp_trap1.c
	gcc omp_trap1.c -o omp_trap1 ${FLAGS}

run:
	./histogram_dynamic 100 0 10000 1500000000 256

clean:
	rm -f omp_trap1 histogram_dynamic histogram_static