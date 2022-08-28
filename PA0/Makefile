# Makefile for CS 140 homework 1

# This is a list of the source (.c) files you use
#
SRC = testbed.c timer.c matrix_multiply.c check_answer.c

# This is the name of the executable file you will run; here, ./matrix_multiply
#
EXEC = matrix_multiply

# This gives the compiler flags. Uncomment one of the two lines.
#
# This one is for performance: use max optimization, skip assertions.
CFLAGS = -Wall -m64 -DBUILD_64 -O3 -DNDEBUG
#
# This one is for debugging: generate symbols and check assertions.
# CFLAGS = -Wall -m64 -DBUILD_64 -g -O0 -DDEBUG
 
# This gives the linker flags, specifying any additional libraries etc.
#
LFLAGS =



# YOU PROBABLY DO NOT HAVE TO CHANGE ANYTHING BELOW THIS LINE.
 
# This generates a list of object file names from the source file names
OBJ = $(addsuffix .o, $(basename $(SRC)))

# "make" makes the executable.
$(EXEC): $(OBJ)
	gcc $(LFLAGS) $(OBJ) -o $(EXEC)

# This says how to build an object (.o) file from a source (.c) file
%.o : %.c
	gcc $(CFLAGS) -c $< -o $@

# "make clean" deletes objects and executable
clean:
	rm -f $(EXEC) *.o 

run:
	@./matrix_multiply -n 4 -a 6
	@./matrix_multiply -n 8 -a 6
	@./matrix_multiply -n 16 -a 6
	@./matrix_multiply -n 32 -a 6
	@./matrix_multiply -n 64 -a 6
	@./matrix_multiply -n 128 -a 6
	@./matrix_multiply -n 256 -a 6
	@./matrix_multiply -n 512 -a 6
	@./matrix_multiply -n 1024 -a 6
	@./matrix_multiply -n 2048 -a 6