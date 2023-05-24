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
	$(CC) -c binTree.c -o binTree.o
	$(CC) -c userFunc.c -o userFunc.o
	$(CC) main.o graphvizDopTask.o checkFunc.o userFunc.o binTree.o $(CFLAGS) $(GRAPHVIZ_LIB) $(GRAPHVIZ_LIBRARY) -o lab4a -lm -lgvc -lcgraph
	
clean:
	rm *.o
