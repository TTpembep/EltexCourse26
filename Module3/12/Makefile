all: serverUDP clientUDP

serverUDP: serverUDP.o
	gcc serverUDP.o -o serverUDP

serverUDP.o: serverUDP.c
	gcc -c serverUDP.c

clientUDP: clientUDP.o
	gcc clientUDP.o -o clientUDP

clientUDP.o: clientUDP.c
	gcc -c clientUDP.c

clean:
	rm *.o serverUDP clientUDP