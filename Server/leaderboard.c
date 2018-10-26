#include "gamehost.h"
#include "connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static volatile int writers;
static volatile int writing;
static volatile int reading;



//Destroy leaderboard when game is over
void destroy_leaderboard() {
	//Cycle through updating existing entries
	LBEntry_t *current = leaderboard;
	LBEntry_t *next = NULL;
	while(current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
}

//Chech the alphabetical order
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

//Send the leaderboard
void send_leaderboard(int connection) {
	pthread_mutex_lock(&lb_mutex);
	while(writers != 0) {
		pthread_cond_wait(&cv, &lb_mutex);
	}
	reading++;
	pthread_mutex_unlock(&lb_mutex);
	//Cycle through updating existing entries
	LBEntry_t *current = leaderboard;
	for( ; current != NULL; current = current->next) {
		//If this is a visible entry (a winning game record)
		if (current->visible) {
			//Notify client that an entry is about to be sent
			send_int(connection, 1);

			//Ready message - Send username
			char buf[256];
			snprintf(buf, sizeof buf, "%s\t\t%d\tSeconds\t\t%d Games Won,\t%d Games Played\n",
					 current->name, current->seconds_played, current->games_won, current->games_played);
			send_string(connection, buf);
		}
	}
	pthread_mutex_lock(&lb_mutex);
	reading--;
	pthread_cond_broadcast(&cv);
	pthread_mutex_unlock(&lb_mutex);
	//Tell client there are no more entires to be sent
	send_int(connection, 0);
}


//Insert the new entry into the list at the appropriate location
LBEntry_t * insert_into_leaderboard(LBEntry_t *head, LBEntry_t *newEntry){
	pthread_mutex_lock(&lb_mutex);
	writers++;
	while(reading != 0 || writing != 0) {
		pthread_cond_wait(&cv, &lb_mutex);
	}
	writing++;
	pthread_mutex_unlock(&lb_mutex);
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
	pthread_mutex_lock(&lb_mutex);
	writing--;
	writers--;
	pthread_cond_broadcast(&cv);
	pthread_mutex_unlock(&lb_mutex);
	return head;
}


//Updates the leader board when a game completes
void update_leaderboard(GameState *game, char *username) {
	//initial values
	int game_was_won = 0, entry_found = 0, entry_place = 0;

	if (game->remainingMines == 0) {
		game_was_won = 1; //This was a winning game
		entry_place =1; //Mark it to be placed
	}

	//Set initial values
	LBEntry_t *newEntry = (LBEntry_t *)malloc(sizeof(LBEntry_t));
    strcpy(newEntry->name, username); //Set name to username
    newEntry->seconds_played = (int)game->time; //Get seconds
    newEntry->visible = game_was_won; //If game was not won, don't show entry

	//Cycle through updating existing entries
	LBEntry_t *current = leaderboard;
	for( ; current != NULL; current = current->next) {
		//If a match is found
		if (strcmp(current->name, newEntry->name)==0) {
			current->games_played++;
			current->games_won += game_was_won;
		}
		//If this is the first entry found
		if (entry_found == 0) {
			entry_found = 1;
			//Update newEntry stats
			newEntry->games_won = current->games_won;
    		newEntry->games_played = current->games_played;
		}
	}

	//If no entry was found
	if (entry_found == 0) {
		entry_place = 1; //Mark this entry to be placed
		//Update newEntry stats as first entry
		newEntry->games_won = game_was_won;
		newEntry->games_played = 1;
	}

	//If this entry is good to be added
	if (entry_place) {
		leaderboard = insert_into_leaderboard(leaderboard, newEntry);
	} else {
		free(newEntry); //Free the entry as it was not placed.
	}
}

//Used for testing of the leaderboard
int test_leaderboard() {

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
	return 0;
}
