CC = gcc
LD = gcc
CFLAGS = -O3


all: mycp mywc

mycp : mycp.o systest.o
	$(CC) -o mycp main2.o systest.o

mywc : mycp.o systest.o
	$(CC) -o mycp main2.o systest.o


systest.o : systest.c systest.h
	$(CC) $(CFLAGS) -c $*.c

mycp.o : mycp.c systest.h
	$(CC) $(CFLAGS) -c $*.c


clean:
	rm -f  *.o  mycp mywc