#include "connection.h"
#include "gamehost.h"

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

#define RANDOM_NUMBER_SEED 42
#define DEFAULT_PORT 12345
#define CLIENT_CONNECTIONS 10

#define DEBUG 1


int main(int argc, char **argv) {
	int myPort, sockfd, clientfd;
	socklen_t sin_size;
	if (argc == 2) { //If a port number was given
		myPort = atoi(argv[1]); //Convert to int and set port
		if (myPort == 0) { //If the port number is invalid
			printf("Error: Invalid port number.");
			exit(1);
		}
	} else {
		myPort = DEFAULT_PORT;
	}
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error: Socket -");
		exit(1);
	}
	
	struct sockaddr_in server_addr, client_addr; //Declare addresses
	
	bzero((char*) &server_addr, sizeof(server_addr)); //FilL the struct with zeros
	server_addr.sin_family = AF_INET;         // host byte order - TCP
	server_addr.sin_port = htons(myPort);     // short, network byte order
	server_addr.sin_addr.s_addr = INADDR_ANY; // auto-fill with the PC(Server) IP address
	
	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) < 0) {
		perror("Error: Socket Bind -");
		exit(1);
	}
	
	if (listen(sockfd, CLIENT_CONNECTIONS) == -1) {
		perror("Error: Listen -");
		exit(1);
	}
	

	printf("Server Hosted on port: %s : %d\n", inet_ntoa(server_addr.sin_addr), myPort);
	
	while(1) {
		printf("LOL\n");
		sin_size = sizeof(struct sockaddr_in);
		if ((clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
			perror("Error: Accept -");
			continue;
		}
		
	}
	
	/*
	printf("START\n");
	srand(RANDOM_NUMBER_SEED);
	GameState *game = init_game();
	if (DEBUG) {
		printf("Successfully initialised\n");
	}*/
	return 0;
}