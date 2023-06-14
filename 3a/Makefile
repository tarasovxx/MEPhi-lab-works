CC=gcc

default: build clean

build:
	$(CC) -c main.c -o main.o
	$(CC) -c func.c -o func.o
	$(CC) -c table.c -o table.o
	$(CC) -c stack.c -o stack.o
	$(CC) main.o func.o stack.o table.o -o lab3

clean:
	rm *.o
