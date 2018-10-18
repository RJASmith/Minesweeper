#include "clientconnection.h"
#include "gameview.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

//All function return the new "gameStatus" which correlates to a displayed screen
//Number - Effect
//0		- Login Screen
//1		- Main Menu
//2		- Game screen
//3		- Leader Boards
//4		- Exit Game


int gameStatus = 0;

int main(int argc, char **argv) {
	while(gameStatus != 4) {
		if (gameStatus == 0) { //Game status is 0 = Login Screen
			gameStatus = draw_login();
		}
		if (gameStatus == 1) { //Game status is 1 = Main menu
			gameStatus = draw_mainmenu();
		}
		if (gameStatus == 2) { //Game status is 2 = gameview
			gameStatus = draw_gameview();
		}
		if (gameStatus == 3) { //Game status is 3 + Leader Boards
			gameStatus = draw_leaderboard();
		}

	} //Game status is 4 = Quictting
	//Free allocatied memory
}