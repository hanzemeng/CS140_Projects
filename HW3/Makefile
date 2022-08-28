build: flat_pi.c tree_pi.c MPI_Reduce_pi.c tree_sum.c MPI_Reduce_sum.c
	mpicc flat_pi.c -o flat_pi -lm
	mpicc tree_pi.c -o tree_pi -lm
	mpicc MPI_Reduce_pi.c -o MPI_Reduce_pi -lm
	mpicc tree_sum.c -o tree_sum -lm
	mpicc MPI_Reduce_sum.c -o MPI_Reduce_sum -lm


core_size = 4
app_name = flat_pi

sample_size = 5000
run_pi:
	mpirun -n ${core_size} ${app_name} ${sample_size}

vector_count = 10000
vector_size = 1000
run_sum:
	mpirun -n ${core_size} ${app_name} ${vector_count} ${vector_size}

flat_pi: flat_pi.c
	mpicc flat_pi.c -o flat_pi -lm
tree_pi: tree_pi.c
	mpicc tree_pi.c -o tree_pi -lm
MPI_Reduce_pi: MPI_Reduce_pi.c
	mpicc MPI_Reduce_pi.c -o MPI_Reduce_pi -lm
tree_sum: tree_sum.c
	mpicc tree_sum.c -o tree_sum -lm
MPI_Reduce_sum: MPI_Reduce_sum.c
	mpicc MPI_Reduce_sum.c -o MPI_Reduce_sum -lm

clean:
	rm -f flat_pi tree_pi MPI_Reduce_pi tree_sum MPI_Reduce_sum 
