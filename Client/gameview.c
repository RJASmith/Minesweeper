#include "gameview.h"
#include "clientconnection.h"
#include <stdio.h> 
#include <stdlib.h> 
#define BORDER_LENGTH 9
#define DEBUG 0
#define TILES_PER_ROW 9


//All function return the new "gameStatus" which correlates to a displayed screen
//Number - Effect
//0		- Login Screen
//1		- Main Menu
//2		- Game screen
//3		- Leader Boards
//4		- Exit Game
 
char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

void draw_border() {
	for(int i=0; i < BORDER_LENGTH; i++) printf("========");
	printf("\n");
}

void clear_buffer() {
	while ((getchar()) != '\n');
}

//Converts an integer or
int char_to_int(char ch, char type, int limit) {
	if (type == 'c') {
		for(int i=0; letters[i]; i++) {
			if (letters[i] == ch) {
				if ((i % 26) < limit) return (i % 26);
				return -1;
			}
		}
	} else {
		return atoi(&ch)-1;
	}
	return -1;
}

//Return the numerical value of the alphabet letter
int get_input_letter(int limit) {
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

//takes a number char and return it's integer value-1
int get_input_number() {
	char val = '\0';
	scanf("%c", &val);
	clear_buffer();
	return atoi(&val)-1;
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

//Send a 3 int array to the server
void send_instruction(int connection, int a, int x, int y) {
	int array[3] = {a , x, y};
	send_int_array(connection, array, 3);
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
		//Tell server a new game is starting
		send_instruction(connection, 0, 0, 0); //Request new game from server
		printf("\nStarting a new Game...\n");
		return 2; //Switch to game menu
	} else if (val == 1) {
		send_instruction(connection, 1, 0, 0); //Request leaderboards from server
		printf("\nDisplaying Leaderboards...\n");
		return 3; //Swicth to leader board view
	} else if (val == 2) {
		send_instruction(connection, 4, 0, 0); //Tell server to close connection
		printf("\nQuitting Game...\n");
		return 4; //Swicth to game clean up screen
	} else {
		printf("\nPlease select either 1, 2, or 3.\n");
	}
	return 1;
}

//Draws the game tiles in the mine field
int draw_gameview(int connection) {
	draw_border();
	
	//Receive game status from the server
	int *status = recv_int_array(connection, 3);
	if (DEBUG) printf("Recvd Status\n");
	//Get tile values
	int *field = recv_int_array(connection, TILES_PER_ROW*TILES_PER_ROW);
	if (DEBUG) printf("Recvd Tiles\n");
	
	//Displaying mines left and current game time
	printf("\nRemaining Mines: %d\n", status[1]);
	
	
	
	//Draw mine field
	printf("\n     "); //Print numbers at top
	for (int i=1; i<= TILES_PER_ROW; i++) {
		printf("%d ", i);
	}
	printf("\n "); //Printing the top margin
	for (int i=0; i< 2*TILES_PER_ROW+4; i++) {
		printf("-");
	}
	int val;
	for (int i=0; i< TILES_PER_ROW; i++) {
		printf("\n %c | ", letters[i]);
		for (int x=0; x < TILES_PER_ROW; x++) {
			val = field[x+(i*TILES_PER_ROW)];
			if (val == -1) {
				printf("  ");
			} else if (val == -2) {
				printf("+ ");
			} else if (val == -3) {
				printf("* ");
			} else {
				printf("%d ", val);
			}
		}
	}
	printf("\n");
	fflush(stdout);
	
	//If the game is over, check to see if player won
	if (status[0] == 0) {
		if (status[1] == 0) {
			printf("\nCongradulations! You found all the mines!\n");
			printf("You won in %d seconds!\n", status[2]);
		} else {
			printf("\nGame Over! You hit a mine.\n");
			printf("The game lasted %d seconds.\n", status[2]);
		}
		return 1; //Returns to the main menu
	}
		
	printf("\nChoose an Option:\n");
	printf("<R> Reveal a Tile\n");
	printf("<P> Place a Flag\n");
	printf("<Q> Quit Game\n");
	
	free(status);
	
	//Get input and action game menu
	int input;
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
				printf("\nEnter the tile coordinates: ");
				char val[2];
				scanf("%2s", val);
				clear_buffer();
				y = char_to_int(val[0],'c',9);
				x = char_to_int(val[1],'n',9);
				if ((x == -1)||(y == -1)) printf("\nPlease enter the coordinates (e.g. A2)");
			} while ((x == -1)||(y == -1));

			//Place flag at x,y.
			send_instruction(connection, 2, x, y);
		}
		
		//If Q was selected - Quit Game
		if (input == 16) {
			printf("\nReturning to Main Menu...\n");
			return 1;
		}
	
		//If R was selected - Reveal Tile
		if (input == 17) {
			int x,y;
			printf("\nRevealing a Tile!");
			do {
				printf("\nEnter the tile coordinates: ");
				char val[2];
				scanf("%2s", val);
				clear_buffer();
				y = char_to_int(val[0],'c',9);
				x = char_to_int(val[1],'n',9);
				if ((x == -1)||(y == -1)) printf("\nPlease enter the coordinates (e.g. A2)");
			} while ((x == -1)||(y == -1));
			
			//Reveal tile at x,y.
			send_instruction(connection, 3, x, y);
		}
	
	return 2;
}

//Draws the Leader Boards
int draw_leaderboard(int connection) {
	return 4;
}

