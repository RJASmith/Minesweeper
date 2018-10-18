#include "gamehost.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#define DEBUG 1



//Checks the coordinates for a mine
bool tile_contains_mine(GameState *game, int x, int y) {
	return (game->tiles[x][y].is_mine);
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
}

//Initialisation of the game state.
//Allocates memeory, sets blank set
//Triggers mine placement
//Setup adjacencies
//Returns the GameState Pointer
GameState * init_game() {
	if (DEBUG) printf("Init Started\n");
	GameState *game = (GameState *)malloc(sizeof(GameState));
	if (game == NULL) {
		if (DEBUG) printf("Initialisation failure\n");
		return NULL;
	}
	if (DEBUG) printf("New Game Started\n");
	for(int x=0; x < NUM_TILES_X; x++) {
		for(int y=0; y < NUM_TILES_Y; y++) {
			game->tiles[x][y].adjacent_mines = 0;
			game->tiles[x][y].is_mine = 0;
			game->tiles[x][y].revealed = 0;
		}
	}
	place_mines(game);
	set_adjacencies(game);
	
	if (DEBUG) display_gamestate(game);
	return game;
}

//Returns the adjancies at that location
void reveal_tile(GameState *game, int x, int y) {
	if (tile_contains_mine(game, x, y)) {
		if (DEBUG) printf("GAME OVER");
		return -1;
	} else {
		game->tiles[x][y].revealed = 1;
		return game->tiles[x][y].adjacent_mines;
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

