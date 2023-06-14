CC=gcc
CFLAGS=-lreadline

default: build clean

build:
	$(CC) -c main.c -o main.o
	$(CC) -c func.c -o func.o
	$(CC) -c table.c -o table.o
	$(CC) -c hash.c -o hash.o
	$(CC) main.o hash.o func.o table.o $(CFLAGS) -o lab3c

clean:
	rm *.o
