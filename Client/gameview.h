#ifndef __GAMEVIEW_H__
#define __GAMEVIEW_H__
#include <stdbool.h>
#define NUM_MINES 10
#define NUM_TILES_X 9
#define NUM_TILES_Y 9

typedef struct {
  int adjacent_mines;
  bool revealed;
  bool is_flagged;
} Tile;

typedef struct {
  Tile tiles[NUM_TILES_X][NUM_TILES_Y];
} GameView;


//Defining a linked list for the leaderboard
typedef struct LBEntry LBEntry_t;

struct LBEntry {
	char name[20];
	int seconds_played;
	int games_played;
	int games_won;
	LBEntry_t *next;
};

//All function return the new "gameStatus" which correlates to a displayed screen
//Number - Effect
//0		- Login Screen
//1		- Main Menu
//2		- Game screen
//3		- Leader Boards
//4		- Exit Game

//Draws the Login Screen
int draw_login();

//Draws the Main menu
int draw_mainmenu();

//Draws the game tiles in the mine field
int draw_gameview();

//Draws the Leader Boards
int draw_leaderboard();

#endif //__GAMEVIEW_H__