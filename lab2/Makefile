CC=gcc
CFLAGS=-Wall -Werror -O3 -std=gnu99
# LDFLAGS=-Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_ilp64.a ${MKLROOT}/lib/intel64/libmkl_tbb_thread.a ${MKLROOT}/lib/intel64/libmkl_core.a -Wl,--end-group -ltbb -lstdc++ -lpthread -lm -ldl
LDFLAGS=-lm -Llib/FW_1.3.1_Lin64/lib  -lfwBase -lfwImage -lfwJPEG -lfwSignal -lfwVideo
SOURCE=lab2.c

all: parallel

parallel: lab2.c
	$(CC) $(CFLAGS) lab2.c $(LDFLAGS) -o lab2-par

clean:
	rm -rf lab2-*
