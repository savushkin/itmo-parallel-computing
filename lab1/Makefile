CC=gcc
CFLAGS=-Wall -Werror -O3 -std=gnu99
LDFLAGS=-lm
SOURCE=lab1.c

all:	lab_seq	parallel-2 parallel-4 parallel-5 parallel-8

lab_seq: lab1.c
	$(CC) $(CFLAGS) lab1.c $(LDFLAGS) -o lab1-seq

parallel-2: lab1.c
	$(CC) $(CFLAGS) -floop-parallelize-all -ftree-parallelize-loops=2 lab1.c -lm -o lab1-par-2

parallel-4: lab1.c
	$(CC) $(CFLAGS) -floop-parallelize-all -ftree-parallelize-loops=4 lab1.c -lm -o lab1-par-4

parallel-5: lab1.c
	$(CC) $(CFLAGS) -floop-parallelize-all -ftree-parallelize-loops=5 lab1.c -lm -o lab1-par-5

parallel-8: lab1.c
	$(CC) $(CFLAGS) -floop-parallelize-all -ftree-parallelize-loops=8 lab1.c -lm -o lab1-par-8

clean:
	rm -rf lab1-*
