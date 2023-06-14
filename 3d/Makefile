CC=gcc
CFLAGS=-lreadline

default: build clean

build:
	$(CC) -c main.c -o main.o
	$(CC) -c checkFunc.c -o checkFunc.o
	$(CC) -c userFunc.c -o userFunc.o
	$(CC) -c hashTable.c -o hashTable.o
	$(CC) -c hash.c -o hash.o
	$(CC) main.o hash.o checkFunc.o hashTable.o userFunc.o $(CFLAGS) -o lab3d
clean:
	rm *.o
