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


//Send an integer to a network partner
void send_int(int connection, int msg) {
    uint32_t val = htonl(msg);
		if (send(connection, &val, sizeof(uint32_t),0) == -1) {
			perror("Error: Send char-");
			exit(1);
		}
}

//Retrive an integer from a network partner
int recv_int(int connection) {
	uint32_t val = 0;	
    if (recv(connection, &val, sizeof(uint32_t),0) == -1) {
		perror("Error: Recv char-");
		exit(1);
    }
    return (int)ntohl(val);
}

//Send a string to a network partner
void send_string(int connection, char msg[20]) {
		if (send(connection, msg, 20,0) == -1) {
			perror("Error: Send char-");
			exit(1);
		}
}

//Retrive a string from a network partner
void recv_string(int connection, char *msg) {
    int bytes;
    if ((bytes = recv(connection, msg, 20,0)) == -1) {
		perror("Error: Recv char-");
		exit(1);
    }
    msg[bytes] = '\0';
}


//Send an int array to a network partner
//Size must be declared and consistent between the sender and receiver
void send_int_array(int connection, int *array, int size) {
	uint32_t value;  
	for (int i = 0; i < size; i++) {
		value = htonl(array[i]);
		send(connection, &value, sizeof(uint32_t), 0);
	}
}

//Receive an int array from a network partner
//Size must be declared and consistent between the sender and receiver
int *recv_int_array(int connection, int size) {
    int number_of_bytes, i=0;
    uint32_t value;
	
	int *results = malloc(sizeof(int)*size);
	for (i=0; i < size; i++) {
		if ((number_of_bytes=recv(connection, &value, sizeof(uint32_t), 0))
		         == -1) {
			perror("recv");
			exit(1);			
		    
		}
		results[i] = ntohl(value);
	}
	return results;
}