CC = c99 
CFLAGS = -Wall # Show all reasonable warnings
LDFLAGS = 

all: server 

server: server.o connection.o gamehost.o

server.o: server.c

connection.o: connection.c

gamehost.o: gamehost.c

clean:
	rm -f server *.o

tidy:
	rm -f *.o
 
.PHONY: clean tidy