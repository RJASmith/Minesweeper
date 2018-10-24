#include "gameview.h"
#include "clientconnection.h"
#include <stdio.h>
#define BORDER_LENGTH 9
#define DEBUG 1


//All function return the new "gameStatus" which correlates to a displayed screen
//Number - Effect
//0		- Login Screen
//1		- Main Menu
//2		- Game screen
//3		- Leader Boards
//4		- Exit Game
 
void draw_border() {
	for(int i=0; i < BORDER_LENGTH; i++) printf("========");
	printf("\n");
}

void clear_buffer() {
	while ((getchar()) != '\n');
}

//Return the numerical value of the alphabet letter
int get_input_letter(int limit) {
	char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char val = '\0';
	scanf("%c", &val);
	clear_buffer();
	for(int i=0; letters[i]; i++) {
			if (letters[i] == val) {
				if ((i % 26) < limit) return (i % 26);
				return -1;
			}
	}
	return -1;
}

int get_input_number() {
	char numbers[] = "123456789";
	char val = '\0';
	scanf("%c", &val);
	clear_buffer();
	for(int i=0; numbers[i]; i++) {
		if (numbers[i] == val) return i;
	}
	return -1;
}

//Draws the Login Screen
int draw_login(int connection) {
	draw_border();
	printf("Welcome to the Online Minesweeper Game!\n");
	draw_border();
	printf("\n To begin, please login with you username and password:\n");
	printf("\nUsername: ");
	char username[20];
	scanf("%s", username);
	if (DEBUG) printf("Entered Username %s\n", username);
	send_string(connection, username);
	printf("Password: ");
	char password[20];
	scanf("%s", password);
	if (DEBUG) printf("Entered Password %s\n", password);
	send_string(connection, password);
	clear_buffer();
	if (recv_int(connection)==0) {
		printf("\n\nThe username or password you have entered is incorrect. Disconnecting.\n");
		return 4;
	}
	return 1;
}

//Draws the Main menu
int draw_mainmenu(int connection) {
	draw_border();
	printf("\nWelcome to Minesweeper Online!\n");
	printf("\nPlease enter a selection:\n");
	printf("<1> Start Game\n<2> View Leader Board\n<3> Quit Game\n");
	printf("\nSelect Option (1-3): ");
	int val = get_input_number(3);
	if (val == 0) {
		printf("\nStarting a new Game...\n");
		return 2;
	} else if (val == 1) {
		printf("\nDisplaying Leaderboards...\n");
		return 3;
	} else if (val == 2) {
		printf("\nQuitting Game...\n");
		return 4;
	} else {
		printf("\nPlease select either 1, 2, or 3.\n");
	}
	return 1;
}

//Draws the game tiles in the mine field
int draw_gameview(int connection) {
	draw_border();	
	printf("\nRemaining Mines: %d\n", 10);
	//Get_updated_field
	//Draw mine field
	printf("\nChoose an Option:\n");
	printf("<R> Reveal a Tile\n");
	printf("<P> Place a Flag\n");
	printf("<Q> Quit Game\n");
	
	int input, gameProgress = 1;
	
		//Get the option selected
		do {
			printf("\nOption (R, P, Q): ");
			input = get_input_letter(26);
			if (DEBUG) printf("Imput value %d\n",input);
			
			if ((input < 15) || (input > 17)) {//Input is not Q R or P
				printf("Please enter one of the listed options.\n");
				input = -1;
			}
		} while (input == -1);
		
		//If P was selected - Place Flag
		if (input == 15) {
			int x,y;
			printf("\nPlacing a Flag!");
			do {
				printf("\nEnter the Column Number: ");
				x = get_input_number();
				if (x == -1) printf("\nPlease try again.");
			} while (x == -1);
			
			do {
				printf("Enter the Row Letter: ");
				y = get_input_letter(9);
				if (y == -1) printf("\nPlease try again\n.");
			} while (y == -1);
			printf("\nPlacing Flag!\n");
			//Place flag at x,y.
		}
		
		//If Q was selected - Quit Game
		if (input == 16) {
			printf("\nReturning to Main Menu...\n");
			gameProgress = 0;
		}
	
		//If R was selected - Reveal Tile
		if (input == 17) {
			int x,y;
			printf("\nRevealing a Tile!");
			do {
				printf("\nEnter the Column Number: ");
				x = get_input_number();
				if (x == -1) printf("\nPlease try again.");
			} while (x == -1);
			
			do {
				printf("Enter the Row Letter: ");
				y = get_input_letter(9);
				if (y == -1) printf("\nPlease try again\n.");
			} while (y == -1);
			printf("\nRevealing Tile!\n");
			//Reveal tile at x,y.
		}
	

	
	if (!gameProgress) {
		return 1;
	}
	return 2;
}

//Draws the Leader Boards
int draw_leaderboard(int connection) {
	return 4;
}

