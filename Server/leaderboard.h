#ifndef __LEADERBOARD_H__
#define __LEADERBOARD_H__
#include <stdbool.h>

typedef struct LBEntry LBEntry_t;
//Defining a linked list for the leaderboard
struct LBEntry {
	char name[20];
	int seconds_played;
	int games_played;
	int games_won;
    LBEntry_t *next;
};


//Updates the leader board when a game completes
void update_leaderboard(GameState *game, char *username) {
	//TO DO
}

LBEntry_t * insert_into_leaderboard(LBEntry_t *leaderboard, LBEntry_t *lbentry);

#endif
