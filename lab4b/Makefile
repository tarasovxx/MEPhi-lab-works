CC=gcc
CFLAGS=-Wall

GRAPHVIZ_INCLUDE=-I/usr/include/graphviz
GRAPHVIX_LIB=-L/usr/lib/graphviz
GRAPHVIX_LIBRARY=-lgvc

default: build clean

build:
	$(CC) -c main.c -o main.o
	$(CC) -c graphvizDopTask.c -o graphvizDopTask.o $(GRAPHVIZ_INCLUDE)
	$(CC) -c checkFunc.c -o checkFunc.o
	$(CC) -c B_Tree.c -o B_Tree.o
	$(CC) -c userFunc.c -o userFunc.o
	$(CC) -c otherFunc.c -o otherFunc.o
	$(CC) main.o checkFunc.o graphvizDopTask.o userFunc.o otherFunc.o B_Tree.o $(CFLAGS) $(GRAPHVIZ_LIB) $(GRAPHVIZ_LIBRARY) -o lab4b -lm -lgvc -lcgraph

clean:
	rm *.o
