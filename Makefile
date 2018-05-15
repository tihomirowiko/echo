CC=gcc
SOURCES=serv.c
OBJ=$(SOURCES:.c=.o)
CFFLAGS=-c

all: serv
serv: $(OBJ)
	$(CC) $< -o $@
$(OBJ): $(SOURCES)
	$(CC) $(CFFLAGS) $< -o $@

clean:
	rm -f *.o serv
cleano:
	rm -f *.o
