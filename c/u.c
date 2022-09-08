#include "u.h"

//菜单1
void menu1()
{
    printf("\n************ChatRoom************\n");
    printf("           1.登录用户              \n");
    printf("           2.注册用户              \n");
    printf("           3.退出                  ");
    printf("\n********************************\n");
}

//隐藏scanf
char *Scanf()
{
    getchar();
    static char code[16];
    int i;
    for(i=0; i<16; i++)
    {
        code[i] = getch();
        // printf("%c@",code[i]);
        if(code[i] == 127 && i>0)
        {
            code[--i] = '\0';
            i--;
        }
        if(code[i] == 127 && i == 0)
        {
            i = -1;
        }
        if(code[i] == 10)
        {   
            code[i] = '\0';
            break;
        }
    }
    // printf("\ncode=%s\n",code);
    return code;
}

char * user_log( int sockfd)
{
    user u;
    u.chosen = 1;
    printf("请输入用户名：\n");
    scanf("%s",u.name);
    printf("请输入密码:\n");
    char * code = Scanf();
    // scanf("%s",u.code);
    strcpy(u.code,code);
    // printf("\nu.code=%s",u.code);
    if(send(sockfd,&u,sizeof(u),0) == -1)
    {
            perror("send");
            exit(1);
    }
    static char name[32];
    strcpy(name, u.name);
    return name;
}

void user_reg( int sockfd)
{
    user u;
    u.chosen = 2;
    printf("请设置用户名\n");
    scanf("%s",u.name);
    printf("请设置密码\n");
    scanf("%s",u.code);
    char code[32];
    printf("请再次输入密码：\n");
    scanf("%s",code);
    if(strcmp(code,u.code) != 0)
    {
        printf("两次输入密码不一样，注册失败\n");
        return ;
    }
    if(send(sockfd,&u,sizeof(u),0) == -1)
    {
        perror("send");
        exit(1);
    }

}

char * func_user( int sockfd)
{
        while(1)
    {
        menu1();
        int i = 0;
        //getchar();
        scanf("%d",&i);
        getchar();
        if(!(i<4&&i>0))
        {
            printf("请输入1-3的数字!!\n");
            continue;
        }
        switch (i)
        {
            case 1:
            {
                char *name = user_log(sockfd);
                char v[10];
                if(recv(sockfd,&v,sizeof(v),0) == -1)
                {
                    perror("recv");
                    exit(1);
                }
                if(strcmp(v,"success") == 0)
                {
                    printf("登录成功\n");
                    // break;
                    static char name1[32];
                    strcpy(name1,name);
                    return name1;
                }
                else
                {
                    printf("没有此用户或用户已在线,请重新输入或注册\n");
                    break;
                }
            }
                
            case 2:
            {
                user_reg(sockfd);
                char v;
                if(recv(sockfd, &v, sizeof(v), 0) == -1)
                {
                    perror("recv");
                    exit(1);
                }
                if(v == 1)
                {
                    printf("注册成功\n");
                }
                else
                {
                    printf("注册失败，已有该用户\n");
                }
                break;
            }
                
            case 3:
                exit(1);
        }
    }
}