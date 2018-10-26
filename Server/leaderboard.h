#ifndef __LEADERBOARD_H__
#define __LEADERBOARD_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct LBEntry LBEntry_t;
//Defining a linked list for the leaderboard
struct LBEntry {
	char name[20];
	int seconds_played;
	int games_played;
	int games_won;
	bool visible; //If it will be displayed to the leaderboard
    LBEntry_t *next;
};


//The pointer to the leaderboard
LBEntry_t *leaderboard;

#endif //__LEADERBOARD_H__
