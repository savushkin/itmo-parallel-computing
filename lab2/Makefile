CC=gcc
CFLAGS=-Wall -Werror -O3 -std=gnu99 -DMKL_ILP64 -m64 -I${MKLROOT}/include
LDFLAGS=-Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_ilp64.a ${MKLROOT}/lib/intel64/libmkl_tbb_thread.a ${MKLROOT}/lib/intel64/libmkl_core.a -Wl,--end-group -ltbb -lstdc++ -lpthread -lm -ldl
SOURCE=lab2.c
#USAGE: source ./ippvars.sh -arch intel64 -platform linux

all: parallel

parallel: lab2.c
	$(CC) $(CFLAGS) lab2.c $(LDFLAGS) -o lab2-par

clean:
	rm -rf lab2-*
