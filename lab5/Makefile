CC=gcc

default: build clean

build:
	$(CC) -c main.c -o main.o
	$(CC) -c checkFunc.c -o checkFunc.o
	$(CC) -c graph.c -o graph.o
	$(CC) -c userFunc.c -o userFunc.o
	$(CC) -c hashTable.c -o hashTable.o
	$(CC) main.o checkFunc.o userFunc.o hashTable.o graph.o $(CFLAGS) -o lab5

clean:
	rm *.o
