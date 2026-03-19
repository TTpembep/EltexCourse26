all: main.exe

main.exe: main.o phonebook.o
	g++ main.o phonebook.o -o main.exe

main.o: main.c
	g++ -c main.c

phonebook.o: phonebook.c
	g++ -c phonebook.c

clean:
	del /f /q *.o main.exe