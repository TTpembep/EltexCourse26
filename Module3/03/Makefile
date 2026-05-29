all: main

main: main.o phonebook.o
	gcc main.o phonebook.o -o main

main.o: main.c
	gcc -c main.c

phonebook.o: phonebook.c
	gcc -c phonebook.c

clean:
	rm *.o main filebook