CC=gcc

default: build clean

build:
	$(CC) -c main.c -o main.o
	$(CC) -c checkFunc.c -o checkFunc.o
	$(CC) -c B_Tree.c -o B_Tree.o
	$(CC) -c userFunc.c -o userFunc.o
	$(CC) -c otherFunc.c -o otherFunc.o
	$(CC) main.o checkFunc.o userFunc.o otherFunc.o B_Tree.o $(CFLAGS) -o lab4b

clean:
	rm *.o
