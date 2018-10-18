#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define RANDOM_NUMBER_SEED 42
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

void place_mines(GameState tiles){
  for(int i = 0; i < NUM_MINES; i++) {
    int x, y;
    do{
      x = rand() % NUM_TILES_X;
      y = rand() % NUM_TILES_Y;
    } while (tile_contains_mine(x,y));
    //place mine at (x,y)
    tiles[x][y]->is_mine = TRUE;
  }
}

bool tile_contains_mine(int x, int y){


}
//Function to generate the x and y positions for the mines
int generateMineLocations(){
  //defining the data for the nodes in our linked list which is storing the mine locations
  typedef struct node{
    int x;
    int y;
    struct node * next;
  } node_t;

  //Defining the head pointer for the linked list
  node_t * head = NULL;
  head = malloc(sizeof(node_t));
  if(head == NULL) {
    return 1;
  }

  //looping 10 times to create the mine locations
  for(int i = 0; i < MAX_MINES; i++){
    //set the current node in the list to the head
    node_t * current = head;
    //iterates until it finds an empty node
    while(current->next != NULL) {
      //If the next node isn't empty, set the current node to be the next one
      current = current->next;
    }
    int cont = 0;
    int x = 0;
    int y = 0;
    while(cont == 0){
      //Generate two random integers
      x = rand() % GRID_SIZE;
      y = rand() % GRID_SIZE;

      //ensure this variable is set to 0 each loop
      int unique = 0;
      //loop through and see if the generated numbers match others in the list
      node_t * currentTwo = head;
      while(currentTwo->next != NULL) {
        if(currentTwo->x == x && currentTwo->y == y){
          //if they match, increment integer, which means this will have to loop again.
          unique++;
        }
        //move onto the next node
        currentTwo = currentTwo->next;
      }
      //if no matches were found, end the while loop
      if(unique == 0){
        cont = 1;
      }
    }
    //create the next node and allocate it the two random digits created before
    current->next = malloc(sizeof(node_t));
    current->next->x = x;
    current->next->y = y;

  }
  //For testing. Loop through and print the integers stored
  node_t * current = head;
  while(current->next != NULL) {
    printf("x = %d; y = %d\n", current->next->x, current->next->y);

    current = current->next;
  }

  return 0;
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
