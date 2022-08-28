My Name: Zach Han. My Partner's name: nobody. <br />

# Program Description
histogram_1a uses one mutex for the entire histogram. <br />
histogram_1b uses one mutex for each bin in the histogram. <br />
histogram_2a uses producers and consumers to sample values. <br />
histogram_2b combines producer and consumer into one function; it uses the combined function to sample values. <br />

# Complie and Run the Programs
1. Type "make build" to compile all programs.
2. To run histogram_1a, histogram_1b, or histogram_2b, type: <br />
./<program_name> <number_of_bins> <minimum_value> <maximum_value> <number_of_values_to_sample> <number_of_threads>
3. To run histogram_2a, type: <br />
./<program_name> <number_of_bins> <minimum_value> <maximum_value> <number_of_values_to_sample> <number_of_producers> <number_of_consumers>
4. Type "make clean" to remove all programs.
