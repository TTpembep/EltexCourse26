all: producer consumer

producer: producer.o
	gcc producer.o -o producer

producer.o: producer.c
	gcc -c producer.c

consumer: consumer.o
	gcc consumer.o -o consumer

consumer.o: consumer.c
	gcc -c consumer.c

clean:
	rm *.o *.txt producer consumer