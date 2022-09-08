#include "s.h"

int main(int argc,char **argv)
{
    signal(SIGPIPE,SIG_IGN);
    
    int cfd; 
    int sockfd = tcp_server_create();
    socklen_t c_size;

    struct sockaddr_in c_addr;
    menu();
    while(1)
    {
        c_size = sizeof(struct sockaddr_in);
        cfd = accept(sockfd,(struct sockaddr *)&c_addr,&c_size);
        if(cfd<0)
        {
            if(errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
            {
                perror("accept error");
                exit(1);
            }           
            continue;
        }
        printf("info client :ip =%s port = %d\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
        printf("cfd = %d\n",cfd);
        // pid_t pid=fork();
        // if(-1==pid)
        // {
        //     perror("fork");
        //     return -1;
        // }
        // else if(pid==0)
        // {
        //     child_run(cfd);
        // }
        // else if(pid > 0)
        // {
        //     // close(cfd);
        // }
        pthread_t tid=0;
        if(pthread_create(&tid,NULL,child_run,&cfd)!=0)
        {
            printf("pthread_create\n");
            return -1;
        }
        pthread_detach(tid);
    }
    return 0;
}