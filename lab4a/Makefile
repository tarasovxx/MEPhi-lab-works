CC=gcc

default: build clean

build:
	$(CC) -c main.c -o main.o
	$(CC) -c checkFunc.c -o checkFunc.o
	$(CC) -c binTree.c -o binTree.o
	$(CC) -c userFunc.c -o userFunc.o
	$(CC) main.o checkFunc.o userFunc.o binTree.o $(CFLAGS) -o lab4a
	
clean:
	rm *.o
