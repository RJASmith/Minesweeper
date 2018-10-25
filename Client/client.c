#include "clientconnection.h"
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

#define DEFAULT_PORT 12345

//All function return the new "gameStatus" which correlates to a displayed screen
//Number - Effect
//0		- Login Screen
//1		- Main Menu
//2		- Game screen
//3		- Leader Boards
//4		- Exit Game


int gameStatus = 0;

int main(int argc, char **argv) {
	int myPort, sockfd;  
	struct hostent *he;
	struct sockaddr_in server_addr; // server address information

	if (argc < 2) {
		printf("Usage: Enter server address and optional port number\n");
		exit(1);
	}
	
	if (argc < 3) {
		myPort = DEFAULT_PORT;
	} else {
		myPort = atoi(argv[2]); //Convert to int and set port
		if (myPort == 0) { //If the port number is invalid
			printf("Error: Invalid port number.");
			exit(1);
		}
	}
	
	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		perror("Error: gethostbyname");
		exit(1);
	}
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error: Socket -");
		exit(1);
	}
	
	bzero((char*) &server_addr, sizeof(server_addr)); //FilL the struct with zeros
	server_addr.sin_family = AF_INET;         // host byte order - TCP
	server_addr.sin_port = htons(myPort);     // short, network byte order
	server_addr.sin_addr = *((struct in_addr *)he->h_addr_list[0]); // The server's IP address
	
	//Connect to the server
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
		perror("Error: Connect -");
		exit(1);
	}
	
	//Wait for server confirmation
	int val = recv_int(sockfd);
	
	//If server returns 0 (too busy)
	if (val == 0) {
		printf("The server has 10 connection and is too busy...\n");
		close(sockfd);
		exit(1);
	}
	
	while(gameStatus != 4) {
		if (gameStatus == 0) { //Game status is 0 = Login Screen
			gameStatus = draw_login(sockfd);
		}
		if (gameStatus == 1) { //Game status is 1 = Main menu
			gameStatus = draw_mainmenu(sockfd);
		}
		if (gameStatus == 2) { //Game status is 2 = gameview
			gameStatus = draw_gameview(sockfd);
		}
		if (gameStatus == 3) { //Game status is 3 + Leader Boards
			gameStatus = draw_leaderboard(sockfd);
		}

	} //Game status is 4 = Quitting
	//Free allocatied memory
}