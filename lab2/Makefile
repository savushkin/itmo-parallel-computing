CC=gcc
CFLAGS=-Wall -Werror -O3 -std=gnu99
LDFLAGS=-I$IPPROOT/include -L$IPPROOT/lib/intel64 -lippvm -lippcc -lippi -lipps -lippcore -pthread -lm
SOURCE=lab2.c
#USAGE: source ./ippvars.sh -arch intel64 -platform linux

all: parallel

parallel: lab2.c
	$(CC) $(CFLAGS) lab2.c $(LDFLAGS) -o lab2-par

clean:
	rm -rf lab2-*
