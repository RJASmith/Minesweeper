#include "gameview.h"
#include <stdio.h>



//All function return the new "gameStatus" which correlates to a displayed screen
//Number - Effect
//0		- Login Screen
//1		- Main Menu
//2		- Game screen
//3		- Leader Boards
//4		- Exit Game
 
void draw_border() {
	for(int i=0; i < 10; i++) printf("========");
	printf("\n");
}

int get_input_letter() {
	char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char val = '\0';
	scanf("%c", &val);
	getchar();
	for(int i=0; letters[i]; i++) {
		if (letters[i] == val) return (i % 26);
	}
	return -1;
}

int get_input_number() {
	char numbers[] = "123456789";
	char val = '\0';
	scanf("%c", &val);
	getchar();
	for(int i=0; numbers[i]; i++) {
		if (numbers[i] == val) return i;
	}
	return -1;
}

//Draws the Login Screen
int draw_login() {
	draw_border();
	printf("Welcome to the Online Minesweeper Game!\n");
	draw_border();
	//Test Connection to Server
	
	printf("\n To begin, please login with you username and password:\n");
	printf("\nUsername: ");
	char username[20];
	scanf("%s", username);
	printf("Password: ");
	char password[20];
	scanf("%s", password);
	//Authenticate_user(username, password);
		printf("\n\nThe username or password you have entered is incorrect. Disconnecting.");
		//return 4
	
	return 1;
}

//Draws the Main menu
int draw_mainmenu() {
	getchar();
	printf("\n\nWelcome to Minesweeper Online!\n");
	printf("\nPlease enter a selection:\n");
	printf("<1> Start Game\n<2> View Leader Board\n<3> Quit Game\n");
	printf("\nSelect Option (1-3): ");
	char val = 1;
	scanf("%c", &val);
	if (val == '1') {
		return 2;
	} else if (val == '2') {
		return 3;
	} else if (val == '3') {
		printf("\nQuitting Game...");
		return 4;
	} else {
		printf("\nPlease select either 1, 2, or 3.\n");
	}
	return 1;
}

//Draws the game tiles in the mine field
int draw_gameview() {
	printf("\nGAME WINDOW\n");
	int selecting = 1;
	int input = 1;
	while(selecting) {
		printf("Enter the row Letter: ");
		input = get_input_letter();
		if (input == -1) {
			printf("TRASH");
		} else {
			printf("Value: %d\n", input);
		}
		printf("Enter the column Number ");
		input = get_input_number();
		if (input == -1) {
			printf("TRASH");
		} else {
			printf("Value: %d\n", input);
		}
	}
	return 4;
}

//Draws the Leader Boards
int draw_leaderboard() {
	return 4;
}

