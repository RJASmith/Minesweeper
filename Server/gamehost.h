#ifndef __GAMEHOST_H__
#define __GAMEHOST_H__
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#define NUM_MINES 10
#define NUM_TILES_X 9
#define NUM_TILES_Y 9

typedef struct {
	int adjacent_mines;
	bool revealed;
	bool is_mine;
	bool is_flag;
} Tile;

typedef struct {
	Tile tiles[NUM_TILES_X][NUM_TILES_Y];
	int remainingMines;
	int inProgress;
	time_t time;
} GameState;

pthread_mutex_t rand_mutex;

//Initialise a new game
void init_game(GameState *game);

//Send a int[3] status and a int[9*9] with tile values
void send_game_data(GameState *game, int connection, char *username);

// Places mines at random tiles in the field.
void place_mines(GameState *game);

//Checks if the tile at coordinates is a mine
bool tile_contains_mine(GameState *game, int x, int y);

//DEBUG COMMAND Displays the Gamestate stats
void display_gamestate(GameState *game);

//Returns the adjancies at that location. Ends the game if a mine is revealed
void reveal_tile(GameState *game, int x, int y);

//Flag a mine - ends the game if all mines are flagged
void flag_tile(GameState *game, int x, int y);

#endif //__GAMEHOST_H__
