#include "connection.h"
#include "gamehost.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define RANDOM_NUMBER_SEED 42
#define DEBUG 1

int main(int argc, char **argv) {
	printf("START\n");
	srand(RANDOM_NUMBER_SEED);
	GameState *game = init_game();
	if (DEBUG) {
		printf("Successfully initialised\n");
	}
}