#ifndef __CONNECTION_H__
#define __CONNECTION_H__
#include <stdbool.h>

//Takes a username and password and attempts to match them to those in a txt.
bool authenticate_user(char username[20], char password[20]);

//Send an integer to a client
void send_int(int connection, int msg);

//Retrive an integer from a client
int recv_int(int connection);

#endif //__CONNECTION_H__
