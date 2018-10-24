#ifndef __CLIENTCONNECTION_H__
#define __CLIENTCONNECTION_H__
#include <stdbool.h>

//Send in integer to a client
void send_int(int connection, int msg);

//Retrive an integer from a client
int recv_int(int connection);

#endif //__CLIENTCONNECTION_H__