#include "gameview.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>  
#include <netdb.h> 
#include <arpa/inet.h>
#include <unistd.h>


//Send an integer to a client
void send_int(int connection, int msg) {
    uint32_t val = htonl(msg);
		if (send(connection, &val, sizeof(uint32_t),0) == -1) {
			perror("Error: Send char-");
			exit(1);
		}
}

//Retrive an integer from a client
int recv_int(int connection) {
	uint32_t val = 0;	
    if (recv(connection, &val, sizeof(uint32_t),0) == -1) {
		perror("Error: Recv char-");
		exit(1);
    }
    return (int)ntohl(val);
}