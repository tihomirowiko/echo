CC=g++
SOURCES=serv.c client.c
OBJ=$(SOURCES:.c=.o)
CFFLAGS=-c

all: serv client
serv: $(OBJ)
	$(CC) $< -o $@
serv.o: serv.c
	$(CC) $(CFFLAGS) $< -o $@
client: client.o
	$(CC) $< -o $@
client.o: client.c
	$(CC) $(CFFLAGS) $< -o $@

clean:
	rm -f *.o serv client
cleano:
	rm -f *.o
