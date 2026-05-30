all: client1 client2

client1: client1.o
	gcc client1.o -o client1 -lrt

client1.o: client1.c
	gcc -c client1.c

client2: client2.o
	gcc client2.o -o client2	-lrt

client2.o: client2.c
	gcc -c client2.c

clean:
	rm *.o client1 client2