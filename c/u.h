#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include "conio.h"

typedef struct user
{
    int chosen;
    char name[32];
    char code [16];
}user;


void menu1();
char * user_log(int sockfd);
void user_reg(int sockfd);
void user_exit(int sockfd);
char * func_user( int sockfd);
char *Scanf();