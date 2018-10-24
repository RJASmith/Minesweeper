#include "connection.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>  
#include <netinet/in.h>

#define TXT_MAX_LENGTH 1000
#define TRUE 1
#define FALSE 0

//Send an integer to a network partner
void send_int(int connection, int msg) {
    uint32_t val = htonl(msg);
		if (send(connection, &val, sizeof(uint32_t),0) == -1) {
			perror("Error: Send char-");
			exit(1);
		}
}

//Retrive an integer from a network partner
int recv_int(int connection) {
	uint32_t val = 0;	
    if (recv(connection, &val, sizeof(uint32_t),0) == -1) {
		perror("Error: Recv char-");
		exit(1);
    }
    return (int)ntohl(val);
}

bool authenticate_user(char username[20], char password[20]) {
    //filepath of authentication.txt. As it is in the same directory, just the filename is needed
    char* filepath = "Authentication.txt";
    //where the opened file will be stored to for reading
    FILE *openedFile;
    //where conents of the the opened file will be stored
    char txtContents[TXT_MAX_LENGTH];
    //opening the file given.
    openedFile = fopen(filepath, "r");
    //char set to tab for later use in removing whitespace
    char blankTab[2] = "\t";
    //int that stores whether both password and username matched
    int bMatch = 0;
    //loop through the txt contents
    while(fgets(txtContents, TXT_MAX_LENGTH, openedFile) != NULL){
        //store the username to ptr
        char *ptr = strtok(txtContents, blankTab);
        while(ptr != NULL){
            //initialise ints each loop
            int uMatch = 0;
            int pMatch = 0;
            //Trim spaces from username by looping and copying everything but whitespace
            char finU[strlen(ptr)];
            for(int i = 0; i < strlen(ptr); i++){
                if(isspace(ptr[i])){
                }
                else{
                    finU[i] = ptr[i];
                }
            }
            //compare the username from txt and the one given by user
            int uRet = strcmp(finU, username);
            //if the usernames are the same, increment the match int for username
            if(uRet == 0){
                uMatch++;
            }

            //switch to password
            ptr = strtok(NULL, blankTab);

            //Trim spaces from password
            char finP[strlen(ptr)];
            for(int i = 0; i < strlen(ptr); i++){
                if(isspace(ptr[i])){
                }
                else{
                    printf("%c", ptr[i]);
                    finP[i] = ptr[i];
                }
            }
            //compare given and stored passwords
            int pRet = strcmp(finP, password);
            //if passwords match, increment the password match int
            if(pRet == 0) {
                pMatch++;
            }
            //if both username and password are confirmed matches in one loop, increment both match int
            if(uMatch > 0 && pMatch > 0) {
                bMatch++;
            }
            //switch to username for next loop
            ptr = strtok(NULL, blankTab);
        }
    }
    //close opened file now that it is finished with
    fclose(openedFile);
    //if in one loop both username and password matched, return true
    if(bMatch > 0) {
        return TRUE;
    }
    else{
        return FALSE;
    }
}
/* main that was used in testing
void main() {
    char username[20] = "Maolin";
    char password[20] = "111111";
    authenticate_user(username, password);
}
*/
