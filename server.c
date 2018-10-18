#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#define RANDOM_NUMBER_SEED 42
#define NUM_MINES 10
#define NUM_TILES_X 9
#define NUM_TILES_Y 9
#define TRUE 1
#define FALSE 0

typedef struct {
  int adjacent_mines;
  bool revealed;
  bool is_mine;
} Tile;

typedef struct {
  Tile tiles[NUM_TILES_X][NUM_TILES_Y];
} GameState;

GameState initialiseGamestate() {
  GameState gameState;
  for(int a = 0; a < NUM_TILES_X; a++) {
    for(int b = 0; b < NUM_TILES_Y; b++) {
        gameState.tiles[a][b].adjacent_mines = 0;
        gameState.tiles[a][b].revealed = FALSE;
        gameState.tiles[a][b].is_mine = FALSE;
    }
  }
  return gameState;
}

bool tile_contains_mine(GameState gameState, int x, int y){
      return gameState.tiles[x][y].is_mine;
}

void place_mines(GameState gameState){
  for(int i = 0; i < NUM_MINES; i++) {
    int x, y;
    do{
      x = rand() % NUM_TILES_X;
      y = rand() % NUM_TILES_Y;
  } while (tile_contains_mine(gameState,x,y));
    //place mine at (x,y)
    gameState.tiles[x][y].is_mine = TRUE;
  }
}



int checkUsernamePassword(char username[20], char password[20]) {
  char storedPassword[20] = "Password";
  char storedUsername[20] = "Reilly";
  //int uRet;
  int uRet = strcmp(storedPassword, password);
  int pRet = strcmp(storedUsername, username);
  if(uRet == 0 && pRet == 0) {
    printf("Login Successful");
  }
  else{
    printf("Login Unsuccessful");
  }
  return 1;
}

void main(int argc, char argv) {
  //generating a random seed for the mine location generation
  srand(RANDOM_NUMBER_SEED);
  //generateMineLocations();
  printf("\nEnter Username: \n");
  char username[20];
  scanf("%s", username);
  printf("\nEnter Password: \n");
  char password[20];
  scanf("%s", password);
  checkUsernamePassword(username, password);
}
