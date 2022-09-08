#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sqlite3.h>
#include <time.h>

typedef struct user
{
    int chosen;
    char name[32];
    char code [16];
}user;


// user u1={0,"123","123"};
// user u2={0,"456","456"};
// user u3={0,"789","789"};


void func_user(int cfd);
void write_online(char *name,int cfd);
void write_user(char *name,char *code);
int check_user_db(char *name,char *code);
int callback(void *para,int argc,char **argv,char **argv_name);
int callback1(void *para,int argc,char **argv,char **argv_name);
void trans_o_name(int cfd);
int check_online_db( char *name);
void delete_o(int cfd);
void trans_o_cfd(char *name,char *sentence);
void chat_record(char *name,char *l_name,char *r_name,char *message);
void check_record(char *l_name,char *r_name,int cfd);