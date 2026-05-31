all: serverRAW clientRAW

serverRAW: serverRAW.o
	gcc serverRAW.o -o serverRAW

serverRAW.o: serverRAW.c
	gcc -c serverRAW.c

clientRAW: clientRAW.o
	gcc clientRAW.o -o clientRAW

clientRAW.o: clientRAW.c
	gcc -c clientRAW.c

clean:
	rm *.o serverRAW clientRAW