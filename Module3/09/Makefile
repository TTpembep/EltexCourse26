all: producer consumer

producer: producer.o
	gcc producer.o -o producer -lrt

producer.o: producer.c
	gcc -c producer.c

consumer: consumer.o
	gcc consumer.o -o consumer	-lrt

consumer.o: consumer.c
	gcc -c consumer.c

clean:
	rm *.o *.txt producer consumer