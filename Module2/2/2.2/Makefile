all: main.exe

main.exe: main.o calc.o
	gcc main.o calc.o -o main.exe

main.o: main.c
	gcc -c main.c

phonebook.o: calc.c
	gcc -c calc.c

clean:
	del /f /q *.o main.exe