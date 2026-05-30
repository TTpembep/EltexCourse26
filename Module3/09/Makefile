all: main

main: main.o
	gcc main.o -o main -lrt

main.o: main.c
	gcc -c main.c

clean:
	rm *.o *.txt main