all: main client

main: main.o
	gcc main.o -o main

main.o: main.c
	gcc -c main.c

client: client.o
	gcc client.o -o client

client.o: client.c
	gcc -c client.c

clean:
	rm *.o main client