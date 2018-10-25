#include "gamehost.h"
#include "connection.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#define DEBUG 1

//Early Declaration for iterative function
void reveal_adjacent_tiles(GameState *game, int x , int y);

//Checks the coordinates for a mine
bool tile_contains_mine(GameState *game, int x, int y) {
	return (game->tiles[x][y].is_mine);
}

//Updates the leader board when a game completes
void update_leaderboard(GameState *game, char *username) {
	//TO DO
}

//Send a int[3] status and a int[9*9] with tile values
void send_game_data(GameState *game, int connection, char *username) {
	
	//Prepare game status
	if (DEBUG) printf("Preparing game status...\n");
	int status[3] = {game->inProgress, game->remainingMines, (int)difftime(game->time, time(0))};
	if (game->inProgress == 0) {
		game->time = difftime(game->time, time(0));
		update_leaderboard(game, username);
	}
	//Send game status
	if (DEBUG) printf("Sending game status...\n");
	send_int_array(connection, status, 3);
	
	//Prepare game field in array
	if (DEBUG) printf("Preparing game field...\n");
	int field[NUM_TILES_X*NUM_TILES_Y];
	for(int x=0; x < NUM_TILES_X; x++) {
		for(int y=0; y < NUM_TILES_Y; y++) {
			int val = -1; //Not revealed or flagged. Display nothing
			
			//If it has been flagged
			if (game->tiles[x][y].is_flag) {
				val = -2; //display: *
			}
			
			//If the tile is revealed or if game is over
			if ((game->tiles[x][y].revealed) || (game->inProgress == 0)) {
				val = game->tiles[x][y].adjacent_mines;
			}
			
			//If the game is over, show all mines
			if ((game->inProgress == 0) && (game->tiles[x][y].is_mine)) {
				val = -2; //display: *
			}
				
			field[x+NUM_TILES_X*y] = val;
		}
	}
	if (DEBUG) printf("Sending game status...\n");
	send_int_array(connection, field, NUM_TILES_X*NUM_TILES_Y);
	if (DEBUG) printf("Game data sent!\n");
	
	
	//REMOVE BELOW
	int val;
	for (int y=0; y< 9; y++) {
		printf("\n | ");
		for (int x=0; x < 9; x++) {
			val = field[x+(y*9)];
			if (val == -1) {
				printf("# ");
			} else if (val == -2) {
				printf("+ ");
			} else {
				printf("%d ", val);
			}
		}
		
	}
}

//Cycles through tiles detecting adjacent mines (9 tiles checking including itself)
void set_adjacencies(GameState *game) {
	if (DEBUG) printf("Setting Adjacencies\n");
	int x, y, xo ,yo;
	for(x=0; x < NUM_TILES_X; x++) {
		if (DEBUG) printf("Row Set\n");
		for(y=0; y < NUM_TILES_Y; y++) {
			if (tile_contains_mine(game, x, y)) {
				for (xo = -1; xo < 2; xo++) {
					for (yo = -1; yo < 2; yo++) {
						if (((x+xo >= 0) && (x+xo < NUM_TILES_X)) &&
							((y+yo >= 0) && (y+yo < NUM_TILES_Y))) {
							game->tiles[x+xo][y+yo].adjacent_mines++;
						}
					}
				}
			}
		}
	}
}


//If a tile has 0 adjacent tiles, it will reveal all around it
//This repeats for each tile with 0 adjacents
void reveal_adjacent_tiles(GameState *game, int x , int y) {
	int yo, xo;
	for (xo = -1; xo < 2; xo++) {
		for (yo = -1; yo < 2; yo++) {
			if (((x+xo >= 0) && (x+xo < NUM_TILES_X)) &&
				((y+yo >= 0) && (y+yo < NUM_TILES_Y))) {
				if (game->tiles[x+xo][y+yo].revealed == 0) {
					game->tiles[x+xo][y+yo].revealed = 1;
					if (game->tiles[x+xo][y+yo].adjacent_mines == 0) {
						reveal_adjacent_tiles(game, x+xo, y+yo);
					}
				}
			}
		}
	}
}


//Place all mines into the GameState Tiles at random locations
void place_mines(GameState *game) {
	for(int i=0; i < NUM_MINES; i++) {
		int x,y;
		do {
			x = rand() % NUM_TILES_X;
			y = rand() % NUM_TILES_X;
			
		} while (tile_contains_mine(game, x, y));
		//Place a mine
		if (DEBUG) printf("Place mine %d %d\n", x, y);
		game->tiles[x][y].is_mine = 1;
	}
	game->remainingMines = NUM_MINES;
}

//Initialisation of the game state.
//Allocates memeory, sets blank set
//Triggers mine placement
//Setup adjacencies
//Returns the GameState Pointer
void init_game(GameState *game) {
	if (DEBUG) printf("Init Started\n");

	//Zero all values
	memset(game, 0, sizeof(GameState));
	if (DEBUG) printf("Memory primed\n");
	/*for(int x=0; x < NUM_TILES_X; x++) {
		for(int y=0; y < NUM_TILES_Y; y++) {
			game->tiles[x][y].adjacent_mines = 0;
			game->tiles[x][y].is_mine = 0;
			game->tiles[x][y].revealed = 0;
		}
	}*/
	place_mines(game);
	set_adjacencies(game);
	game->time = time(0);
	game->inProgress = 1;
	if (DEBUG) printf("All Values set\n");
	
	if (DEBUG) display_gamestate(game);
}

//Returns the adjancies at that location
int reveal_tile(GameState *game, int x, int y) {
	if (tile_contains_mine(game, x, y)) {
		if (DEBUG) printf("GAME OVER");
		return -1;
	} else {
		game->tiles[x][y].revealed = 1;
		if (game->tiles[x][y].adjacent_mines == 0) {
			reveal_adjacent_tiles(game, x, y);
		}
		return 1;
	}
}


//DEBUG COMMAND Display all Game State stats
void display_gamestate(GameState *game) {
	for(int x=0; x < NUM_TILES_X; x++) {
		for(int y=0; y < NUM_TILES_Y; y++) {
				printf("X: %d Y: %d A: %d M: %d\n", x,y,game->tiles[x][y].adjacent_mines, game->tiles[x][y].is_mine);
		}
	}
	printf("\n");
	
	printf("TILES\n");
	for(int x=0; x < NUM_TILES_X; x++) {
		for(int y=0; y < NUM_TILES_Y; y++) {
			if (game->tiles[x][y].is_mine == 1) {
				printf("*");
			} else {
				printf("%d", game->tiles[x][y].adjacent_mines);
			}
		}
		printf("\n");
	}
}

