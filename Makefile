CC=gcc
CFLAGS=-Wall -Werror -O3 -std=gnu99 -m64

LAB1_LDFLAGS=-lm
LAB2_LDFLAGS=-lm -Llib/FW_1.3.1_Lin64/lib  -lfwBase -lfwImage -lfwJPEG -lfwSignal -lfwVideo
LAB3_LDFLAGS=-lm

SOURCE_LAB1=./src/lab1.c
SOURCE_LAB2=./src/lab2.c
SOURCE_LAB3=./src/lab3.c

all: clean lab_seq lab1-parallel-2 lab1-parallel-4 lab1-parallel-5 lab1-parallel-8 lab2-parallel lab3-parallel-static-1-1 lab3-parallel-static-1-2 lab3-parallel-static-1-4 lab3-parallel-static-100-1 lab3-parallel-static-100-2 lab3-parallel-static-100-4 lab3-parallel-dynamic-1-1 lab3-parallel-dynamic-1-2 lab3-parallel-dynamic-1-4 lab3-parallel-dynamic-100-1 lab3-parallel-dynamic-100-2 lab3-parallel-dynamic-100-4 lab3-parallel-guided-1-1 lab3-parallel-guided-1-2 lab3-parallel-guided-1-4

lab_seq: $(SOURCE_LAB1)
	$(CC) $(CFLAGS) $(SOURCE_LAB1) $(LAB1_LDFLAGS) -o ./bin/lab1-seq

lab1-parallel-2: $(SOURCE_LAB1)
	$(CC) $(CFLAGS) -floop-parallelize-all -ftree-parallelize-loops=2 $(SOURCE_LAB1) -lm -o ./bin/lab1-par-2

lab1-parallel-4: $(SOURCE_LAB1)
	$(CC) $(CFLAGS) -floop-parallelize-all -ftree-parallelize-loops=4 $(SOURCE_LAB1) -lm -o ./bin/lab1-par-4

lab1-parallel-5: $(SOURCE_LAB1)
	$(CC) $(CFLAGS) -floop-parallelize-all -ftree-parallelize-loops=5 $(SOURCE_LAB1) -lm -o ./bin/lab1-par-5

lab1-parallel-8: $(SOURCE_LAB1)
	$(CC) $(CFLAGS) -floop-parallelize-all -ftree-parallelize-loops=8 $(SOURCE_LAB1) -lm -o ./bin/lab1-par-8

lab2-parallel: $(SOURCE_LAB2)
	$(CC) $(CFLAGS) $(SOURCE_LAB2) $(LAB2_LDFLAGS) -o ./bin/lab2-par

lab3-parallel-static-1-1: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=1 -DTHREADS=1 -DSCHEDULE=static $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-static-1-1

lab3-parallel-static-1-2: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=1 -DTHREADS=2 -DSCHEDULE=static $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-static-1-2

lab3-parallel-static-1-4: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=1 -DTHREADS=4 -DSCHEDULE=static $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-static-1-4

lab3-parallel-static-100-1: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=100 -DTHREADS=1 -DSCHEDULE=static $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-static-100-1

lab3-parallel-static-100-2: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=100 -DTHREADS=2 -DSCHEDULE=static $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-static-100-2

lab3-parallel-static-100-4: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=100 -DTHREADS=4 -DSCHEDULE=static $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-static-100-4

lab3-parallel-dynamic-1-1: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=1 -DTHREADS=1 -DSCHEDULE=dynamic $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-dynamic-1-1

lab3-parallel-dynamic-1-2: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=1 -DTHREADS=2 -DSCHEDULE=dynamic $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-dynamic-1-2

lab3-parallel-dynamic-1-4: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=1 -DTHREADS=4 -DSCHEDULE=dynamic $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-dynamic-1-4

lab3-parallel-dynamic-100-1: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=100 -DTHREADS=1 -DSCHEDULE=dynamic $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-dynamic-100-1

lab3-parallel-dynamic-100-2: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=100 -DTHREADS=2 -DSCHEDULE=dynamic $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-dynamic-100-2

lab3-parallel-dynamic-100-4: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=100 -DTHREADS=4 -DSCHEDULE=dynamic $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-dynamic-100-4

lab3-parallel-guided-1-1: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=1 -DTHREADS=1 -DSCHEDULE=guided $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-guided-1-1

lab3-parallel-guided-1-2: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=1 -DTHREADS=2 -DSCHEDULE=guided $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-guided-1-2

lab3-parallel-guided-1-4: $(SOURCE_LAB3)
	$(CC) $(CFLAGS) -fopenmp -DCHUNK=1 -DTHREADS=1 -DSCHEDULE=guided $(SOURCE_LAB3) $(LAB3_LDFLAGS) -o ./bin/lab3-par-guided-1-4

clean:
	rm -rf ./bin/*
