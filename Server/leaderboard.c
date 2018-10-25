#include "leaderboard.h"
#include "clientconnection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void draw_line(int length){
	printf("\n");
	for(int i = 0;i < length; i++){
		printf("=");
	}
	printf("\n");
}

//add to the head of the linked list
node_t * node_add(node_t *head, LBEntry_t *lbentry) {
    node_t *new = (node_t *)malloc(sizeof(node_t));
    if (new == NULL) {
        return NULL;
    }
    new->lbentry = lbentry;
    new->next = head;
    return new;
}

//Draws the Leader Boards
void draw_leaderboard(node_t *leaderboard) {
	int lineLength = 90;
	draw_line(lineLength);

	for( ; leaderboard != NULL; leaderboard = leaderboard->next) {
        printf("%s\t\t%d seconds\t\t%d games won, %d games played\n", leaderboard->lbentry->name, leaderboard->lbentry->seconds_played, leaderboard->lbentry->games_won, leaderboard->lbentry->games_played);
    }

	draw_line(lineLength);
}


int check_alpha_order(char first[20], char second[20]){
	char charTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int firstNum[20];
	int secondNum[20];
	for(int a = 0; a < 20; a++){
		for(int i=0; charTable; i++) {
			if(first[a] == charTable[i]){
				firstNum[a] = i;
			}
			if(second[a] == charTable[i]){
				secondNum[a] = i;
			}
		}
	}
	int i = 0;
	int order = 0;
	while(i<20){
		if(firstNum[i] < secondNum[i]){
			order = 1;
			i=20;
		}
		if(firstNum[i] > secondNum[i]){
			order = 2;
			i=20;
		}
		i++;
	}
	return order;
}


node_t * insert_into_leaderboard(node_t *leaderboard, LBEntry_t *lbentry){
	//loop through all nodes of the leaderboard until it reaches a NULL node
	node_t *newNode = (node_t *)malloc(sizeof(node_t));
	newNode->lbentry = lbentry;

	node_t * current = leaderboard;

	if(leaderboard == NULL) {
		printf("The leaderboard is NULL");
		leaderboard = newNode;
		return leaderboard;

	}
	else{
		for( ; current != NULL;current = current-> next){
			//if time played is faster than faster time, push to front
			if(current->lbentry->seconds_played > lbentry->seconds_played) {
				newNode->next = current;
				break;
			}
			//check to see if current node has less time
			if(current->lbentry->seconds_played < lbentry->seconds_played) {
				//check to see if next node has more time. If so, insert
				if(current->next->lbentry->seconds_played > lbentry->seconds_played) {
					newNode->next = current->next;
					current->next = newNode;
					break;
				}
			}
			//if only current has the same time score, only check against it
			if(current->lbentry->seconds_played == lbentry->seconds_played && current->next->lbentry->seconds_played > lbentry->seconds_played) {
				//check against games won
				if(current->lbentry->games_won > lbentry->games_won) {
					newNode->next = current->next;
					current->next = newNode;
					break;
				}
				//if games won is the same, check against games played
				if(current->lbentry->games_won == lbentry->games_won) {
					if(current->lbentry->games_played > lbentry->games_played) {
						newNode->next = current->next;
						current->next = newNode;
						break;
					}
					else if(current->lbentry->games_played == lbentry->games_played){
						int order = check_alpha_order(current->lbentry->name, lbentry->name);
						if(order == 1 || order == 0) {
							newNode->next = current->next;
							current->next = newNode;
							break;
						}
					}
				}
			}
			//if both current and next have the same seconds played, check against both
			if(current->lbentry->seconds_played == lbentry->seconds_played && current->next->lbentry->seconds_played == lbentry->seconds_played) {
				//check against games won
				if(current->lbentry->games_won > lbentry->games_won) {
					if(current->next->lbentry->games_won < lbentry->games_won){
						newNode->next = current->next;
						current->next = newNode;
						break;
					}
				}
				//if current is same games won but next is not, only check current played games
				if(current->lbentry->games_won == lbentry->games_won &&  current->next->lbentry->games_won < lbentry->games_won) {
					if(current->lbentry->games_played > lbentry->games_played) {
						newNode->next = current->next;
						current->next = newNode;
						break;
					}
				}

				//if current and next games won is the same, check both against games_played
				if(current->lbentry->games_won == lbentry->games_won &&  current->next->lbentry->games_won == lbentry->games_won) {
					if(current->lbentry->games_played > lbentry->games_played) {
						if(current->next->lbentry->games_played < lbentry->games_played){
							newNode->next = current->next;
							current->next = newNode;
							break;
						}
					}
					//if only current gamesplayed are the same check against current name
					if(current->lbentry->games_played == lbentry->games_played && current->next->lbentry->games_played < lbentry->games_played){
						int order = check_alpha_order(current->lbentry->name, lbentry->name);
						if(order == 1 || order == 0) {
							newNode->next = current->next;
							current->next = newNode;
							break;
						}

						if(order == 2) {
							newNode->next = current;
							break;
						}
					}

					//if both current and next have same games played, check both against name
					if(current->lbentry->games_played == lbentry->games_played && current->next->lbentry->games_played < lbentry->games_played){
						int currentOrder = check_alpha_order(current->lbentry->name, lbentry->name);
						int nextOrder = check_alpha_order(current->next->lbentry->name, lbentry->name);
						if(currentOrder == 1 || currentOrder == 0) {
							if(nextOrder == 2){
								newNode->next = current->next;
								current->next = newNode;
								break;
							}
						}
					}
				}
			}
		}
	}
	return leaderboard;
}

void main() {
	node_t *leaderboard = NULL;

	LBEntry_t *lbe = malloc(sizeof(LBEntry_t));
	char name[20] = "Maolin";
	strcpy(lbe->name, name);
	lbe->seconds_played = 60;
	lbe->games_won = 4;
	lbe->games_played = 8;
	leaderboard = insert_into_leaderboard(leaderboard, lbe);
	printf("\nMaolin was Inserted\n");

	LBEntry_t *lbea = malloc(sizeof(LBEntry_t));
	char namea[20] = "Jason";
	strcpy(lbea->name, namea);
	lbea->seconds_played = 30;
	lbea->games_won = 4;
	lbea->games_played = 8;
	leaderboard = insert_into_leaderboard(leaderboard, lbea);
	printf("\nJason was inserted\n");


	for( node_t * current = leaderboard; current != NULL;current = current-> next){
		printf("\n%s %d %d %d\n", current->lbentry->name, current->lbentry->seconds_played, current->lbentry->games_won, current->lbentry->games_played);
	}
	/*
	LBEntry_t *lbeb = malloc(sizeof(LBEntry_t));
	char nameb[20] = "Jonesy";
	strcpy(lbeb->name, nameb);
	lbeb->seconds_played = 60;
	lbeb->games_won = 3;
	lbeb->games_played = 3;
	leaderboard = insert_into_leaderboard(leaderboard, lbeb);
	printf("\nJonesy was inserted\n");
	*/

}
