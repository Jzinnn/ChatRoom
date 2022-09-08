#include "c.h"

int flg = 0;
int v = 0;

//连接服务器
int connect_server()
{
    int sockfd;
    //第一步：创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        ERRLOG("socket error");
    }
    struct sockaddr_in serveraddr;
    //填充服务器网络信息结构体
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);

    socklen_t addrlen = sizeof(serveraddr);
    //第三步：给服务器发送客户端的链接请求
    if (connect(sockfd, (struct sockaddr *)&serveraddr, addrlen) == -1)
    {
        ERRLOG("connect error");
    }

    return sockfd;
}

void menu2()
{
    printf("\n********************************\n");
    printf("欢迎来到聊天室\n");
    printf("输入sto对某人说话\n");
    printf("输入sta对大家说话\n");
    printf("输入che查看当前用户\n");
    printf("输入ext退出聊天室\n");
    printf("输入file发送文件\n");
    printf("输入rec查找聊天记录");
    printf("\n********************************\n");
}
void menu3()
{
    printf("\n********************************\n");
    printf("超级管理员你好\n");
    printf("输入shu将成员禁言\n");
    printf("输入bye将成员强制下线\n");
    printf("输入che查看当前用户\n");
    printf("输入ext退出聊天室");
    printf("\n********************************\n");
}

//传输文件
void Send_file(FILE *fp,int sockfd,char *filename)
{
    char str[5];
    while(1)
    {
        memset(str,0,5);
        fread(str,5,1,fp);
        if(send(sockfd,str,5,0)==-1)
        {
            perror("send str");
            exit(1);
        }
        if(feof(fp))
        {
            char str1[4]="quit";
            if(send(sockfd,str1,4,0)==-1)
            {
                perror("send str1");
                exit(1);
            }
            break;
        }
    }
}
//接收文件
void Recv_file(int sockfd, names f)
{
    char filename[1024];
    strcpy(filename,f.l_name);
    strcat(filename,"_send_");
    strcat(filename,f.r_name);
    FILE *fp = fopen(filename, "a");
    char str[5];
    while(1)
    {
        memset(str,0,N);
        if(recv(sockfd,str,5,0)==-1)
        {
            perror("recv str");
            exit(1);
        }
        if(strcmp(str,"quit") == 0)
        {
            break;
        }
        if(fwrite(str,strlen(str),1,fp) == 0)
        {
            perror("fread");
            exit(1);
        }
    }
    fclose(fp);
}

void *Recv(void *arg)
{
    sen s = *(sen *)arg;
    char name[32];
    strcpy(name, s.name);
    int sockfd = s.sockfd;

    cho c;
    while (1)
    {
        memset(&c, 0, sizeof(c));
        if (recv(sockfd, &c, sizeof(c), 0) == -1)
        {
            perror("recv c");
            exit(1);
        }
        if (strncmp(c.chose, "r", 1) == 0)
        {
            if(strcmp(c.buf,"file") == 0)
            {
                char v[10];
                if (recv(sockfd, v, sizeof(v), 0) == -1)
                {
                    perror("recv");
                    exit(1);
                }
                if (strcmp(v, "success") == 0)
                {
                    printf(" 发送成功\n");
                    flg = 0;
                }
                else
                {
                    printf("发送失败\n该用户不在线 \n");
                    flg = 0;
                }
            }
            if (strcmp(c.buf, "sto") == 0)
            {
                char v[10];
                if (recv(sockfd, v, sizeof(v), 0) == -1)
                {
                    perror("recv");
                    exit(1);
                }
                if (strcmp(v, "success") == 0)
                {
                    printf(" 发送成功\n");
                    flg = 0;
                }
                else
                {
                    printf("发送失败\n1.该用户不在线 2.不可以给自己发信息\n");
                    flg = 0;
                }
            }
            if (strcmp(c.buf, "che") == 0)
            {
                char o_name[32];
                printf("当前在线用户有：\n");
                while (1)
                {
                    if (recv(sockfd, o_name, sizeof(o_name), 0) == -1)
                    {
                        perror("recv o_name");
                        exit(1);
                    }
                    if (strcmp(o_name, "quit") == 0)
                    {
                        break;
                    }
                    printf("%s\n", o_name);
                    memset(o_name, 0, sizeof(o_name));
                }
            }
            if (strcmp(c.buf, "sta") == 0)
            {
                char sentence[1024];
                if (recv(sockfd, sentence, sizeof(sentence), 0) == -1)
                {
                    perror("recv o_name");
                    exit(1);
                }
                printf("%s\n", sentence);
            }
            if (strcmp(c.buf, "rec") == 0)
            {
                printf("聊天记录如下\n");
                char senn[1024];
                while (1)
                {
                    if (recv(sockfd, senn, sizeof(senn), 0) == -1)
                    {
                        perror("recv ");
                        exit(1);
                    }
                    if (strcmp(senn, "no") == 0)
                    {
                        break;
                    }
                    printf("%s\n", senn);
                    memset(senn, 0, sizeof(senn));
                }
                flg = 0;
            }
        }
        if (strncmp(c.chose, "l", 1) == 0)
        {
            if (strcmp(c.buf, "sto") == 0)
            {
                char l_name[32];
                if (recv(sockfd, l_name, sizeof(l_name), 0) == -1)
                {
                    perror("recv2");
                    exit(1);
                }
                char sentence[1024];
                if(recv(sockfd, sentence, sizeof(sentence),0) == -1)
                {
                    perror("recv2");
                    exit(1);
                }
                printf("%s给你发私聊消息说：%s\n",l_name,sentence);
            }
            if (strcmp(c.buf, "shu") == 0)
            {
                printf("你已被管理员禁言\n");
                v = 1;
            }
            if(strcmp(c.buf, "bye") == 0)
            {
                printf("你已被管理员踢下线\n");
                // break;
                send(sockfd, "ext", sizeof("ext"),0);
                pthread_exit(NULL);
            }
            if(strcmp(c.buf, "file") == 0)
            {
                names f;
                if(recv(sockfd,&f,sizeof(f),0) == -1)
                {
                    perror("recv");
                    exit(1);
                }
                printf("%s向你发送文件%s\n",f.l_name,f.r_name);
                printf("正在接收...\n");
                Recv_file(sockfd,f);
                sleep(2);
                printf("接收完毕\n");
            }
        }
    }
}
void *Send(void *arg)
{
    sen s = *(sen *)arg;
    char name[32];
    strcpy(name, s.name);
    int sockfd = s.sockfd;
    char buf[N];

    while (1)
    {
        if (flg == 0)
        {
            menu2();
            memset(buf, 0, sizeof(buf));

            scanf("%s", buf);

            buf[sizeof(buf) - 1] = '\0';
            if(!((strcmp(buf, "sto") == 0)||(strcmp(buf, "sta") == 0)||strcmp(buf, "che") == 0||strcmp(buf, "ext") == 0||strcmp(buf, "file") == 0||strcmp(buf, "rec") == 0))
            {
                printf("请输入菜单上的选项\n");
                continue;
            }
            //管理员
            if ((v == 1) && ((strcmp(buf, "sto") == 0 || strcmp(buf, "sta") == 0)))
            {
                printf("你已被禁言\n");
                continue;
            }
            //发送
            if (send(sockfd, buf, sizeof(buf), 0) == -1)
            {
                perror("buf send");
                exit(1);
            }
            //私聊
            if ((strcmp(buf, "sto") == 0))
            {
                printf("请输入你要聊天的用户名：\n");

                char l_name[32];
                scanf("%s", l_name);
                // printf("l_name = %s\n",l_name);
                l_name[sizeof(l_name) - 1] = '\0';
                names n; //两个名字放进结构体
                strcpy(n.l_name, l_name);
                strcpy(n.r_name, name);
                //发送名字
                if (send(sockfd, &n, sizeof(n), 0) == -1)
                {
                    perror("send l_name");
                    exit(1);
                }
                printf("请输入你要私聊的信息：\n");
                char sentence[1024];
                scanf("%s", sentence);
                if(send(sockfd, sentence, sizeof(sentence),0) == -1)
                {
                    perror("send");
                    exit(1);
                }
            }
            //群聊
            if ((strcmp(buf, "sta") == 0))
            {
                printf("请输入你要公屏发送的消息：\n");
                sta a;

                strcpy(a.name, name);
                a.name[sizeof(a.name) - 1] = '\0';
                memset(a.sentence, 0, sizeof(a.sentence));
                scanf("%s", a.sentence);
                if (send(sockfd, &a, sizeof(a), 0) == -1)
                {
                    perror("send sentence");
                    exit(1);
                }
                // flg = 1;
            }
            //查看在线
            if (strcmp(buf, "che") == 0)
            {
                sleep(1);
                continue;
            }
            //退出
            if (strcmp(buf, "ext") == 0)
            {
                buf[sizeof(buf) - 1] = '\0';
                exit(1);
            }
            //传输文件
            if (strcmp(buf, "file") == 0)
            {
                names f;
loop:
                printf("请输入你要传输文件的对象：\n");
                scanf("%s", f.l_name);
                if(strcmp(name,f.l_name) == 0)
                {
                    printf("不能给自己发送文件\n");
                    memset(&f, 0, sizeof(f));
                    goto loop;
                }
                printf("请输入需要传输的文件名字：\n");
                scanf("%s", f.r_name);
                FILE *fp=fopen(f.r_name,"r");
                if(fp == NULL)
                {
                    names f1;
                    strcpy(f1.l_name,"null");
                    if(send(sockfd,&f1,sizeof(f1),0) == -1)
                    {
                        perror("send f");
                        exit(1);
                    }
                    printf("该文件不存在\n");
                    continue;
                }
                else
                {
                    if(send(sockfd,&f,sizeof(f),0) == -1)
                    {
                        perror("send f");
                        exit(1);
                    }
                    printf("文件存在开始传输...\n");
                    Send_file(fp,sockfd,f.r_name);
                    fclose(fp);
                    sleep(3);
                    continue;
                }
            }
            //查看聊天记录
            if (strcmp(buf, "rec") == 0)
            {
                char l_name[32];
                printf("请输入你要查找聊天记录的私聊对象：\n");
                scanf("%s", l_name);
                l_name[sizeof(l_name) - 1] = '\0';
                names n; //两个名字放进结构体
                strcpy(n.l_name, l_name);
                strcpy(n.r_name, name);
                //发送名字
                if (send(sockfd, &n, sizeof(n), 0) == -1)
                {
                    perror("send l_name");
                    exit(1);
                }
                flg = 1;
            }
        }
    }
}

void *Send_a(void *arg)
{
    sen s = *(sen *)arg;
    char name[32];
    strcpy(name, s.name);
    int sockfd = s.sockfd;
    char buf[N];

    while (1)
    {
        if (flg == 0)
        {
            menu3();
            memset(buf, 0, sizeof(buf));

            scanf("%s", buf);

            buf[sizeof(buf) - 1] = '\0';
            if(!((strcmp(buf, "shu") == 0)||(strcmp(buf, "bye") == 0)||strcmp(buf, "che") == 0||strcmp(buf, "ext") == 0))
            {
                printf("请输入菜单上的选项\n");
                continue;
            }
            // printf("choose = %s\n", buf);
            //发送
            if (send(sockfd, buf, sizeof(buf), 0) == -1)
            {
                perror("buf send");
                exit(1);
            }
            if (strcmp(buf, "che") == 0)
            {
                sleep(1);
                continue;
            }
            if (strcmp(buf, "shu") == 0)
            {
                printf("请输入你要禁言的人：");
                char name[32];
                scanf("%s", name);
                if (send(sockfd, name, sizeof(name), 0) == 0)
                {
                    perror("send");
                    exit(1);
                }
            }
            if (strcmp(buf, "ext") == 0)
            {
                buf[sizeof(buf) - 1] = '\0';
                exit(1);
            }
            if (strcmp(buf, "bye") == 0)
            {
                printf("请输入你要踢下线的人：\n");
                char name[32];
                scanf("%s",name);
                if(send(sockfd,name,sizeof(name),0) == 0)
                {
                    perror("send");
                    exit(1);
                }
            }
        }
    }
}
void *Recv_a(void *arg)
{
    sen s = *(sen *)arg;
    char name[32];
    strcpy(name, s.name);
    int sockfd = s.sockfd;

    cho c;
    while (1)
    {
        memset(&c, 0, sizeof(c));
        if (recv(sockfd, &c, sizeof(c), 0) == -1)
        {
            perror("recv c");
            exit(1);
        }
        if (strncmp(c.chose, "r", 1) == 0)
        {
            if (strcmp(c.buf, "che") == 0)
            {
                char o_name[32];
                printf("当前在线用户有：\n");
                while (1)
                {
                    if (recv(sockfd, o_name, sizeof(o_name), 0) == -1)
                    {
                        perror("recv o_name");
                        exit(1);
                    }
                    if (strcmp(o_name, "quit") == 0)
                    {
                        break;
                    }
                    printf("%s\n", o_name);
                    memset(o_name, 0, sizeof(o_name));
                }
            }
            if (strcmp(c.buf, "shuv") == 0)
            {
                printf("禁言成功\n");
            }
            if (strcmp(c.buf, "shuf") == 0)
            {
                printf("禁言失败\n");
            }
            if(strcmp(c.buf, "byev") == 0)
            {
                printf("成功踢下线\n");
            }
            if(strcmp(c.buf, "byef") == 0)
            {
                printf("踢下线失败\n");
            }
        }
    }
}