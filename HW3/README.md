# How to build the programs
1. Type "make build" to build all programs
2. flat_pi, tree_pi, MPI_Reduce_pi are programs for estimating pi. To run them type: <br />
mpirun -n <number_of_cores> <name_of_the_program> <sample_size_for_a_single_core> <br />
For example: "mpirun -n 6 tree_pi 1000" uses 6 cores to run tree_pi to sample a total of 6*1000=6000 points. <br />
3. tree_sum, MPI_Reduce_sum are programs for adding vectors. To run them type: <br />
mpirun -n <number_of_cores> <name_of_the_program> <number_of_vector> <size_of_each_vector> <br />
For example: "mpirun -n 2 MPI_Reduce_sum 1000 10" uses 2 cores to run MPI_Reduce_sum to sum 1000 vectors each of size 10. <br />
4. Note that tree_sum and MPI_Reduce_sum always check every element in the result vector before printing out the first 30 elements. If there is an error in the result vector, the test function will report the location of the error and terminate the program immediately.
5. Type "make clean" to remove all programs.