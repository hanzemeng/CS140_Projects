Flags = -lpthread -lm -Wall

build: histogram_1a.c histogram_1b.c histogram_2a.c histogram_2b.c
	gcc histogram_1a.c -o histogram_1a ${Flags}
	gcc histogram_1b.c -o histogram_1b ${Flags}
	gcc histogram_2a.c -o histogram_2a ${Flags}
	gcc histogram_2b.c -o histogram_2b ${Flags}

histogram_2a: histogram_2a.c
	gcc histogram_2a.c -o histogram_2a ${Flags}
histogram_1c: histogram_1c.c
	gcc histogram_1c.c -o histogram_1c ${Flags}
histogram_1b: histogram_1b.c
	gcc histogram_1b.c -o histogram_1b ${Flags}
histogram_1a: histogram_1a.c
	gcc histogram_1a.c -o histogram_1a ${Flags}

run:
	./histogram_2b 100 0 10000 40000 4
check:
	valgrind -s ./histogram_2b 100 0 100 1000000 4

clean:
	rm -f histogram histogram_1a histogram_1b histogram_2a histogram_2b

#16/7
