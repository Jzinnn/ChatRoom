#include "s.h"
#include "u.h"

int flg = 0;

void menu()
{
    printf("\n**********************************\n");
    printf("       ChatRoom服务器已开启      ");
    printf("\n**********************************\n");
}

int tcp_server_create()
{
    signal(SIGPIPE,SIG_IGN);
    // signal(SIGINT,server_exit);
    int sockfd;
    socklen_t c_size;
    struct sockaddr_in s_addr;
    // struct sockaddr_in c_addr;

    // if((sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error!\n");
        exit(1);
    }
    
    printf("sockfd successfully!\n");

    int opt = 1;
    //设置套接字可以重复使用端口号
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt));

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    s_addr.sin_addr.s_addr = htons(INADDR_ANY);

    if(bind(sockfd,(struct sockaddr *)&s_addr,sizeof(struct sockaddr_in)) != 0)
    {
        perror("bind error");
        exit(1);
    }

    printf("bind successfully!\n");

    if(listen(sockfd,20) != 0)
    {
        perror("listen error");
        exit(1);
    }

    printf("listen successfully!\n");

    return sockfd;
}

void *child_run(void *arg)
{
    int cfd = *(int *)arg;
    //接收用户登入
    func_user(cfd);

    char buf[1024];
    while(1)
    {
        if(flg == 0)
        {
        memset(buf,0,sizeof(buf));
        //接收
        if(recv(cfd,buf,sizeof(buf),0) != 0)
        {
            // perror("recv buf");
            // exit(1);
        }
        printf("cfd = %d choose = %s\n",cfd,buf);
        //私聊
        if(strcmp(buf,"sto") == 0)
        {
            names n;
            //接收名字
            if(recv(cfd,&n,sizeof(n),0) != -1)
            {
                // perror ("recv l_name");
                // exit(1);
            }
            char sentence[1024];
            //接收信息
            if(recv(cfd,sentence,sizeof(sentence),0) == -1)
            {
                perror("recv ");
                exit(1);
            }
            cho c_r;
            strcpy(c_r.chose,"r");
            strcpy(c_r.buf,buf);
            if(check_online_db(n.l_name)>0 && strcmp(n.l_name,n.r_name) != 0)
            {
                int cfd1=check_online_db(n.l_name);
                cho c_l;
                strcpy(c_l.chose,"l");
                strcpy(c_l.buf,buf);
                //给被邀请的用户发信息
                //发送选择
                if(send(cfd1,&c_l,sizeof(c_l),0) == -1)
                {
                    perror("send1");
                    exit(1);
                }
                //发送名字
                if(send(cfd1,n.r_name,sizeof(n.r_name),0)==-1)
                {
                    perror("send2");
                    exit(1);
                }
                //发送信息
                if(send(cfd1,sentence,sizeof(sentence),0)==-1)
                {
                    perror("send");
                    exit(1);
                }
                chat_record(n.r_name,n.l_name,n.r_name,sentence);
                //给邀请者发选择
                if(send(cfd,&c_r,sizeof(c_r),0) == -1)
                {
                    perror("send1");
                    exit(1);
                }
                //给邀请者发成功消息
                char v[10] ="success";
                if(send(cfd,v,sizeof(v),0)==-1)
                {
                    perror("send3");
                    exit(1);
                }
            }
            else
            {
                //给邀请者发选择
                if(send(cfd,&c_r,sizeof(c_r),0) == -1)
                {
                    perror("send1");
                    exit(1);
                }
                //给邀请者发失败消息
                char v[10] ="fail";
                if(send(cfd,v,sizeof(v),0)==-1)
                {
                    perror("send3");
                    exit(1);
                }
            }
            continue;
        }
        //群聊
        if(strcmp(buf,"sta") == 0)
        {
            sta a;

            if(recv(cfd, &a, sizeof(a),0) == 1)
            {
                perror("a");
                exit(1);
            }
            trans_o_cfd(a.name,a.sentence);
            continue;
        }
       //查看在线
        if(strcmp(buf,"che") == 0)
        {
            cho c_r;
            strcpy(c_r.chose,"r");
            strcpy(c_r.buf,buf);
            if(send(cfd,&c_r,sizeof(c_r),0) == -1)
            {
                    perror("send1");
                    exit(1);
            }
            //向客户端发送名字
            trans_o_name(cfd);
            continue;
        }
        //退出
        if(strcmp(buf,"ext") == 0)
        {
            delete_o(cfd);
            printf("%d客户度端退出\n",cfd);
            break;
        }
        //传输文件
        if(strcmp(buf,"file") == 0)
        {
            names f;
            if(recv(cfd,&f,sizeof(f),0) == -1)
            {
                perror("recv");
                exit(1);
            }
            if(strcmp(f.l_name,"null") == 0)
            {
                printf("该文件不存在\n");
            }
            else
            {
                cho c_r;
                strcpy(c_r.chose,"r");
                strcpy(c_r.buf,buf);
                if(check_online_db(f.l_name)>0 )
                {
                    int cfd1=check_online_db(f.l_name);
                    cho c_l;
                    strcpy(c_l.chose,"l");
                    strcpy(c_l.buf,buf);
                    //给被邀请的用户发信息
                    //发送选择
                    if(send(cfd1,&c_l,sizeof(c_l),0) == -1)
                    {
                        perror("send1");
                        exit(1);
                    }
                    //发送名字
                    if(send(cfd1,&f,sizeof(f),0)==-1)
                    {
                        perror("send2");
                        exit(1);
                    }
                    char str[5];
                    while(1)
                    {
                        memset(str,0,sizeof(str));
                        if(recv(cfd,str,5,0) == -1)
                        {
                            perror("recv while");
                            exit(1);
                        }
                        if(send(cfd1,str,5,0) == -1)
                        {
                            perror("send while");
                            exit(1);
                        }
                        if(strcmp(str,"quit") == 0)
                        {
                            break;
                        }
                    }
                    //给邀请者发选择
                    if (send(cfd, &c_r, sizeof(c_r), 0) == -1)
                    {
                        perror("send1");
                        exit(1);
                    }
                    //给邀请者发失败消息
                    char v[10] = "success";
                    if (send(cfd, v, sizeof(v), 0) == -1)
                    {
                        perror("send3");
                        exit(1);
                    }
                }
                else
                {
                //     char str[5];
                // while(1)
                // {
                //     memset(str,0,sizeof(str));
                //     if(recv(cfd,str,5,0) == -1)
                //     {
                //         perror("recv while");
                //         exit(1);
                //     }
                //     if(strcmp(str,"quit") == 0)
                //     {
                //         break;
                //     }
                // }
                    //给邀请者发选择
                    if (send(cfd, &c_r, sizeof(c_r), 0) == -1)
                    {
                        perror("send1");
                        exit(1);
                    }
                    //给邀请者发失败消息
                    char v[10] = "fail";
                    if (send(cfd, v, sizeof(v), 0) == -1)
                    {
                        perror("send3");
                        exit(1);
                    }
                }
                continue;
            }
            
        }
        //查看聊天记录
        if(strcmp(buf,"rec") == 0)
        {
            cho c_r;
            strcpy(c_r.chose,"r");
            strcpy(c_r.buf,buf);
            names n;
            //接收名字
            if(recv(cfd,&n,sizeof(n),0) != -1)
            {
                // perror ("recv l_name");
                // exit(1);
            }
            if(send(cfd,&c_r,sizeof(c_r),0) == -1)
            {
                    perror("send1");
                    exit(1);
            }
            check_record(n.l_name,n.r_name,cfd);
            continue;
        }
        //管理员权限
        if(strcmp(buf,"shu") == 0)
        {
            char name[32];
            if(recv(cfd,name,sizeof(name),0) == -1)
            {
                perror("recv");
                exit(1);
            }
            if(check_online_db(name)>0)
            {
                int cfd1 = check_online_db(name);
                cho c_l;
                strcpy(c_l.chose,"l");
                strcpy(c_l.buf,buf);
                if (send(cfd1, &c_l, sizeof(c_l), 0) == -1)
                {
                    perror("exit");
                    exit(1);
                }
                cho c_r;
                strcpy(c_r.chose, "r");
                strcat(buf, "v");
                strcpy(c_r.buf, buf);
                if (send(cfd, &c_r, sizeof(c_r), 0) == -1)
                {
                    perror("exit");
                    exit(1);
                }
                // delete_o(cfd1);
            }
            else
            {
                cho c_r;
                strcpy(c_r.chose, "r");
                strcat(buf, "f");
                strcpy(c_r.buf, buf);
                if (send(cfd, &c_r, sizeof(c_r), 0) == -1)
                {
                    perror("exit");
                    exit(1);
                }
            }
            continue;
        }
        if(strcmp(buf,"bye") == 0)
        {
            char name[32];
            if(recv(cfd,name,sizeof(name),0) == -1)
            {
                perror("recv");
                exit(1);
            }
            if(check_online_db(name)>0)
            {
                int cfd1 = check_online_db(name);
                cho c_l;
                strcpy(c_l.chose,"l");
                strcpy(c_l.buf,buf);
                if (send(cfd1, &c_l, sizeof(c_l), 0) == -1)
                {
                    perror("exit");
                    exit(1);
                }
                delete_o(cfd1);
                cho c_r;
                strcpy(c_r.chose, "r");
                strcat(buf, "v");
                strcpy(c_r.buf, buf);
                if (send(cfd, &c_r, sizeof(c_r), 0) == -1)
                {
                    perror("exit");
                    exit(1);
                }
                
            }
            else
            {
                cho c_r;
                strcpy(c_r.chose, "r");
                strcat(buf, "f");
                strcpy(c_r.buf, buf);
                if (send(cfd, &c_r, sizeof(c_r), 0) == -1)
                {
                    perror("exit");
                    exit(1);
                }
            }
        }
        }
    }
    // pthread_exit(NULL);
}