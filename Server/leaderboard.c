#include "leaderboard.h"
#include "connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Updates the leader board when a game completes
void update_leaderboard(GameState *game, char *username) {
	//TO DO
}


int check_alpha_order(char first[20], char second[20]){
	char charTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int firstNum[20];
	int secondNum[20];
	for(int a = 0; a < 20; a++){
		for(int i=0; i < 54; i++) {
			if(first[a] == charTable[i]){
				if(i<26) {
					firstNum[a] = i;
				}
				else{
					firstNum[a] = i-26;
				}
			}
			if(second[a] == charTable[i]){
				if(i<26){
					secondNum[a] = i;
				}
				else{
					secondNum[a] = i-26;
				}
			}
		}
	}
	int i = 0;
	int order = 0;
	while(order == 0){
		if(firstNum[i] < secondNum[i]){
			order = 1;
		}
		if(firstNum[i] > secondNum[i]){
			order = 2;
		}
		i++;
	}
	return order;
}


LBEntry_t * insert_into_leaderboard(LBEntry_t *head, LBEntry_t *newEntry){
	//pointer for looping
	LBEntry_t *current = head;
	LBEntry_t *previous = NULL;
	//if the linked list is currently 7349520empty, set the head to the provided entry
    if(current == NULL) {
        return newEntry;
    }
	else{
		while(current != NULL) {
			int check = 0;
			if(current->seconds_played <= newEntry->seconds_played) {
				if(current->seconds_played < newEntry->seconds_played) {
					check = 1;
				}
				else if (current->games_won >=newEntry->games_won){
					if (current->games_won > newEntry->games_won) {
						check = 1;
					}
					else if (current->games_played >= newEntry->games_played) {
						int alphaCheck = check_alpha_order(current->name, newEntry->name);
						if(current->games_played > newEntry->games_played){
							check = 1;
						}
						else if( alphaCheck == 1 || alphaCheck == 0){
							check = 1;
						}
					}
				}
			}
			if(check == 1) {
				newEntry->next = current;
				if(previous == NULL) {
					return newEntry;
				}
				previous->next = newEntry;
				return head;
			}

			if(current->next == NULL) {
				current->next = newEntry;
				return head;
			}
			previous = current;
			current = current->next;
		}
	}
}

//Used for testing of the leaderboard
void main() {

    //LBEntry_t *leaderboard = (LBEntry_t *)malloc(sizeof(LBEntry_t));
    LBEntry_t *head= NULL;

    LBEntry_t *newEntry = (LBEntry_t *)malloc(sizeof(LBEntry_t));
    char name[20] = "Maolin";
    strcpy(newEntry->name, name);
    newEntry->seconds_played = 10;
    newEntry->games_won = 5;
    newEntry->games_played = 8;

	head = insert_into_leaderboard(head, newEntry);

	LBEntry_t *newEntryA = (LBEntry_t *)malloc(sizeof(LBEntry_t));
	char nameA[20] = "Jason";
    strcpy(newEntryA->name, nameA);
    newEntryA->seconds_played = 9;
    newEntryA->games_won = 5;
    newEntryA->games_played = 8;

	head = insert_into_leaderboard(head, newEntryA);

	LBEntry_t *newEntryB = (LBEntry_t *)malloc(sizeof(LBEntry_t));
	char nameB[20] = "Jonesy";
    strcpy(newEntryB->name, nameB);
    newEntryB->seconds_played = 11;
    newEntryB->games_won = 5;
    newEntryB->games_played = 8;

	head = insert_into_leaderboard(head, newEntryB);

	LBEntry_t *newEntryC = (LBEntry_t *)malloc(sizeof(LBEntry_t));
	char nameC[20] = "Maolan";
    strcpy(newEntryC->name, nameC);
    newEntryC->seconds_played = 10;
    newEntryC->games_won = 5;
    newEntryC->games_played = 8;

	head = insert_into_leaderboard(head, newEntryC);

	LBEntry_t *current = head;
	for( ; current != NULL; current = current->next) {
		printf("%s", current->name);
	}

}
