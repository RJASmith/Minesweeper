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
};

typedef struct node node_t;
struct node {
    LBEntry_t *lbentry;
    node_t *next;
};

node_t * node_add(node_t *head, LBEntry_t *lbentry);

void draw_leaderboard(node_t *leaderboard);

int check_alpha_order(char first[20], char second[20]);

node_t * insert_into_leaderboard(node_t *leaderboard, LBEntry_t *lbentry);

#endif
