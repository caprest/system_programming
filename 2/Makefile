CC = gcc
LD = gcc
CFLAGS = -O3


all: mycp mywc myls

mycp : mycp.o systest.o
	$(CC) -o $@ mycp.o  systest.o

mywc : mywc.o systest.o
	$(CC) -o $@ mywc.o  systest.o

myls : myls.o systest.o
	$(CC) -o $@ myls.o systest.o

systest.o : systest.c systest.h 
	$(CC) $(CFLAGS) -c $^

mycp.o :mycp.c
	$(CC) $(CFLAGS) -c $^

mywc.o : mywc.c
	$(CC) $(CFLAGS) -c $^

myls.o : myls.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f  *.o  mycp mywc myls
