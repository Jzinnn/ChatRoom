#include "c.h"
#include "u.h"

int main(int argc, char **argv)
{
    printf("---------\n");
    //  连接服务器
    int sockfd = connect_server();
    // 用户登入
    char *name = func_user(sockfd);

    sen s;
    strcpy(s.name, name);
    s.sockfd = sockfd;

    if (strcmp(name, "admin") == 0)
    {
        pthread_t tid = 0;
        if (pthread_create(&tid, NULL, Recv_a, &s) != 0)
        {
            printf("pthread_create\n");
            return -1;
        }

        Send_a(&s);
        pthread_join(tid, NULL);
    }
    else
    {
        pthread_t tid = 0;
        if (pthread_create(&tid, NULL, Recv, &s) != 0)
        {
            printf("pthread_create\n");
            return -1;
        }
        pthread_t tid1 = 0;
        if (pthread_create(&tid1, NULL, Send, &s) != 0)
        {
            printf("pthread_create\n");
            return -1;
        }

        // Send(&s);
        if(pthread_join(tid, NULL) == 0)
        {
            pthread_cancel(tid1);
        }
    }

    return 0;
}