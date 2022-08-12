CC=gcc

default: flood4

flood4: flood4.c packets.o options.o
	$(CC) flood4.c -o flood4 packets.o options.o
	
packets.o: packets.c packets.h
	$(CC)  -c packets.c
	
options.o: options.c options.h
	$(CC)  -c options.c

.PHONY: clean

clean:
	rm -f *.o flood4
