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
void recv_string(int connection, char *msg);

//Send an int array to a network partner
//Size must be declared and consistent between the sender and receiver
void send_int_array(int connection, int *array, int size);

//Send an int array from a network partner
//Size must be declared and consistent between the sender and receiver
int *recv_int_array(int connection, int size);


#endif //__CLIENTCONNECTION_H__