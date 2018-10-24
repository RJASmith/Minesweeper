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
#include <pthread.h>

#define RANDOM_NUMBER_SEED 42
#define DEFAULT_PORT 12345
#define CLIENT_CONNECTIONS 10

#define DEBUG 1

typedef struct { //Structure used to pass values to threads
	int connection;
	int *status;
} game_args;
	

void *run_game(void *vargs) { //Thread main
	if (DEBUG) printf("Thread started!\n");
		game_args *args = (game_args *)vargs;
		int *p = args->status;
		*p = 1;
	
	if (DEBUG) printf("Thread Connection: %d\n", args->connection);
	send_int(args->connection, 1); //Send confirmation to client
	char username[20], password[20];
	if (DEBUG) printf("Waiting for username...\n");
	recv_string(args->connection, (char *)username);
	if (DEBUG) printf("Received username: %s\n", username);
	
	if (DEBUG) printf("Waiting for password...\n");
	recv_string(args->connection, (char *)password);
	if (DEBUG) printf("Received password: %s\n", password);
	if (authenticate_user(username, password)) {
		if (DEBUG) printf("Login SUCCESS\n");
		send_int(args->connection, 1);
	} else {
		if (DEBUG) printf("Login FAILED\n");
		send_int(args->connection, 0);
	}

	close(args->connection);
	*p = 0;
	free(args);
	return 0;
}


int main(int argc, char **argv) {
	
	//Network Connection declaration
	int myPort, sockfd, clientfd;
	socklen_t sin_size;
	if (argc == 2) { //If a port number was given
		myPort = atoi(argv[1]); //Convert to intassignment makes pointer from integer without a cast  and set port
		if (myPort == 0) { //If the port number is invalid
			printf("Error: Invalid port number.");
			exit(1);
		}
	} else {
		myPort = DEFAULT_PORT; //If no port is specified, use default
	}
	
	//Set random seed
	srand(RANDOM_NUMBER_SEED);
	
	//Threads declaration
	pthread_t tid[CLIENT_CONNECTIONS]; //Thread IDs
	int tstatus[CLIENT_CONNECTIONS]; //Thread Status - 0 not in use

	//Initalising thread status values
	for (int i = 0; i < CLIENT_CONNECTIONS; i++) {
		tstatus[i] = 0;
	}
	
	//Retreiving a socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error: Socket -");
		exit(1);
	}
	
	//Decalring socket address
	struct sockaddr_in server_addr, client_addr; //Declare addresses
	
	//Settup socket for server
	bzero((char*) &server_addr, sizeof(server_addr)); //FilL the struct with zeros
	server_addr.sin_family = AF_INET;         // host byte order - TCP
	server_addr.sin_port = htons(myPort);     // short, network byte order
	server_addr.sin_addr.s_addr = INADDR_ANY; // auto-fill with the PC(Server) IP address
	
	//Bind to socket
	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) < 0) {
		perror("Error: Socket Bind -");
		exit(1);
	}
	
	//Start listening for connections
	if (listen(sockfd, CLIENT_CONNECTIONS) == -1) {
		perror("Error: Listen -");
		exit(1);
	}
	
	//Main loop - accepting connections and assigning to threads
	printf("Server Hosted on port: %d\n", myPort);
	while(1) {
		if (DEBUG) printf("Ready for connection...\n");
		
		//Wait for a connection
		sin_size = sizeof(struct sockaddr_in);
		if ((clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
			perror("Error: Accept -");
			continue;
		}
		if (DEBUG) printf("New Connection: %d\n", clientfd);
		
		//find a free thread to take the connection
		for (int i = 0; i <= CLIENT_CONNECTIONS; i++) {
			if (i == CLIENT_CONNECTIONS) { 			//If no free thread is found
				send_int(clientfd, 0);		 	//Return 0 to client
				close(clientfd);					//Close connection
				break; 								//Exit loop
			}
			
			if (tstatus[i] == 0) {					//If the thread ID is free
				game_args *args = (game_args *)malloc(sizeof(game_args));
				args->connection = clientfd;
				args->status = &tstatus[i];
				pthread_create(&tid[i], NULL, run_game, args);	//Create Thread
				if (DEBUG) printf("server: got connection on thread %d\n", i);
				break;
			}
		}
		/*if (send(clientfd, "no", 8,0)== -1) {
			perror("Error: Thread send -");
			exit(1);
		}*/
		//close(clientfd);
		
		
	}
	
	/*
	printf("START\n");
	GameState *game = init_game();
	if (DEBUG) {
		printf("Successfully initialised\n");
	}*/
	return 0;
}