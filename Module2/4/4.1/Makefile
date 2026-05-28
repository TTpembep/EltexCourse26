all: main.exe

main.exe: main.o phonebook.o
	gcc main.o phonebook.o -o main.exe

main.o: main.c
	gcc -c main.c

phonebook.o: phonebook.c
	gcc -c phonebook.c

clean:
	del /f /q *.o main.exe