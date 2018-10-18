#ifndef __GAMEHOST_H__
#define __GAMEHOST_H__
#include <stdbool.h>
#define NUM_MINES 10
#define NUM_TILES_X 9
#define NUM_TILES_Y 9

typedef struct {
  int adjacent_mines;
  bool revealed;
  bool is_mine;
} Tile;

typedef struct {
  Tile tiles[NUM_TILES_X][NUM_TILES_Y];
} GameState;

//Initialise a new game
GameState * init_game();

// Places mines at random tiles in the field.
void place_mines(GameState *game);

//Checks if the tile at coordinates is a mine
bool tile_contains_mine(GameState *game, int x, int y);

//DEBUG COMMAND Displays the Gamestate stats
void display_gamestate(GameState *game);

#endif //__GAMEHOST_H__