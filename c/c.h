#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/stat.h>

#define PORT 5555
#define N 1024
#define ERRLOG(errmsg) do{\
                        perror(errmsg);\
                        printf("%s - %s - %d\n",__FILE__, __func__,__LINE__);\
                        exit(1);\
                        }while(0) 

typedef struct names
{
    char l_name[32];
    char r_name[32];
} names;

typedef struct sen
{
    char name[32];
    int sockfd;
} sen;

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

int connect_server();
void menu2( );
void *Recv(void *arg);
void *Send(void *arg);
int p_chat(int sockfd,names n);
void *Send_a(void *arg);
void *Recv_a(void *arg);
void Send_file(FILE *fp,int sockfd,char *filename);
void Recv_file(int sockfd, names f);