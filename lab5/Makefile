CC=gcc
CFLAGS=-Wall

GRAPHVIZ_INCLUDE=-I/usr/include/graphviz
GRAPHVIX_LIB=-L/usr/lib/graphviz
GRAPHVIX_LIBRARY=-lgvc

default: build clean

build:
	$(CC) -c main.c -o main.o
	$(CC) -c graphImage.c -o graphImage.o $(GRAPHVIZ_INCLUDE)
	$(CC) -c checkFunc.c -o checkFunc.o
	$(CC) -c graph.c -o graph.o
	$(CC) -c userFunc.c -o userFunc.o
	$(CC) -c hashTable.c -o hashTable.o
	$(CC) main.o checkFunc.o userFunc.o hashTable.o graphImage.o graph.o $(CFLAGS) $(GRAPHVIZ_LIB) $(GRAPHVIZ_LIBRARY) -o lab5 -lgvc -lcgraph

clean:
	rm *.o
