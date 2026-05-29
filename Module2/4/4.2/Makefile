all: main.exe

main.exe: main.o coolQ.o
	gcc main.o coolQ.o -o main.exe

main.o: main.c
	gcc -c main.c

coolQ.o: coolQ.c
	gcc -c coolQ.c

clean:
	del /f /q *.o main.exe