#ifndef __CLIENTCONNECTION_H__
#define __CLIENTCONNECTION_H__
#include <stdbool.h>

//Send an integer to a client
void send_int(int connection, int msg);

//Retrive an integer from a client
int recv_int(int connection);

//Send a string to a network partner
void send_string(int connection, char msg[20]);

//Retrive a string from a network partner
void recv_string(int connection, char *msg[20]);

#endif //__CLIENTCONNECTION_H__