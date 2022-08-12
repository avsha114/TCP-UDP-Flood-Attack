CC=gcc

default: flood6

flood6: flood6.c packets.o options.o
	$(CC) flood6.c -o flood6 packets.o options.o
	
packets.o: packets.c packets.h
	$(CC)  -c packets.c
	
options.o: options.c options.h
	$(CC)  -c options.c

.PHONY: clean

clean:
	rm -f *.o flood6
