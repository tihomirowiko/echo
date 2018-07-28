CC=g++
SOURCES=user.c serv.c client.c
OBJ=$(SOURCES:.c=.o)
EXECUTEABLE=client serv
INCLUDE=-I .
CFFLAGS=-c -lm -DLINUX -Wall

all: $(EXECUTEABLE)
client: client.o user.o
	$(CC) $< user.o $(INCLUDE) -o $@
serv: serv.o user.o
	$(CC) $< user.o $(INCLUDE) -o $@
$(OBJ):%.o:%.c
	$(CC) $(CFFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -f *.o serv client
cleano:
	rm -f *.o
.PHONY: run
run:
	./serv &
.PHONY: k
k:
	./kill.sh client
	./kill.sh serv
