#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
  //generating a random seed for the mine location generation
  srand(time(NULL));

  //looping 10 times to create the mine locations
  for(int i = 0; i < 10; i++){
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
      x = rand() % 10;
      y = rand() % 10;

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

string checkPassword(string username, string password) {
  string storedPassword = "Password";
  string storedUsername = "Reilly"
  if(username == storedUsername && password == storedPassword) {
    printf("Login Successful");
  }
  else{
    printf("Login Unseccessful");
  }

}

void main(int argc, char argv) {

  //generateMineLocations();
  checkPassword("Reilly", "Password");
}
