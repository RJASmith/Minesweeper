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


void draw_leaderboard(LBEntry_t *leaderboard);

int check_alpha_order(char first[20], char second[20]);

LBEntry_t * insert_into_leaderboard(LBEntry_t *leaderboard, LBEntry_t *lbentry);

#endif
