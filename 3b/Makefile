CC=gcc
CFLAGS=-lreadline

default: build clean

build:
	$(CC) -c main.c -o main.o
	$(CC) -c func.c -o func.o
	$(CC) -c table.c -o table.o 
	$(CC) main.o func.o table.o $(CFLAGS) -o lab3b

clean:
	rm *.o
