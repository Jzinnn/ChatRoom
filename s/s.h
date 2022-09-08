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
#include <pthread.h>

#define PORT 5555

typedef struct names
{
    char l_name[32];
    char r_name[32];
} names;

typedef struct cho
{
    char chose[5];
    char buf[5];
}cho;

typedef struct sta
{
    char name[32];
    char sentence[1024];
}sta;

typedef struct cfds
{
    int cfd;
    int cfd1;
    char r_name[32];
    char l_name[32];
    int tid;
}cfds;

// void child_exit(int sig);
void *child_run(void *arg);
int tcp_server_create();
void p_chat(int cfd,int cfd1,names n);
void *p_chat1(void *arg);