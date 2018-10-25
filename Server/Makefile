CC = c99 
CFLAGS = -lpthread -Wall # Show all reasonable warnings
LDFLAGS = 

all: server 

server: server.o connection.o gamehost.o leaderboard.o -lpthread

server.o: -lpthread server.c

connection.o: connection.c

gamehost.o: gamehost.c

leaderboard.o: leaderboard.c

clean:
	rm -f server *.o

tidy:
	rm -f *.o
 
.PHONY: clean tidy
