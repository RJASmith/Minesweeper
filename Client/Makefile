CC = c99 
CFLAGS = -lpthread -Wall # Show all reasonable warnings
LDFLAGS = 

all: client

client: client.o clientconnection.o gameview.o

client.o: client.c

clientconnection.o: clientconnection.c

gameview.o: gameview.c

clean:
	rm -f client *.o

tidy:
	rm -f *.o
 
.PHONY: clean tidy