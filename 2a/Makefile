CC = gcc
VECTOR = -D queue_vector
LIST = -D queue_list

vector:
	$(CC) -c main.c -lreadline -o main.o $(VECTOR)
	$(CC) -c queue_vector.c -o queue_vector.o $(VECTOR)
	$(CC) -c func.c -o func.o $(VECTOR)
	$(CC) main.o queue_vector.o func.o -lreadline -o lab2 

list:
	$(CC) -c main.c -lreadline -o main.o $(LIST)
	$(CC) -c queue_list.c -o queue_list.o $(LIST)
	$(CC) -c func.c -o func.o $(LIST)
	$(CC) main.o queue_list.o func.o -lreadline -o lab2 

clean:
	rm *.o
	rm lab2
