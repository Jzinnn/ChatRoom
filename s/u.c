#include "u.h"
#include "s.h"

//写入在线表格
void write_online(char *name,int cfd)
{
    sqlite3 *ppdb;
    //打开或创建数据库
    int ret = sqlite3_open("user.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open:%s\n", sqlite3_errmsg(ppdb));
        exit(1);
    }
    // //创建表
    char sql[128] = {0};
    sprintf(sql,"create table if not exists online(name char,cfd int);");
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec:%s\n",sqlite3_errmsg(ppdb));
        exit(1);
    }
    memset(sql,0,sizeof(sql));
    sprintf(sql,"insert into online values('%s',%d);",name,cfd);
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec;%s\n",sqlite3_errmsg(ppdb));
        exit(1);
    }
}
//写入用户表格
void write_user(char *name,char *code)
{
    sqlite3 *ppdb;
    //打开或创建数据库
    int ret = sqlite3_open("user.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open:%s\n", sqlite3_errmsg(ppdb));
        exit(1);
    }
    //创建表
    char sql[128] = {0};
    sprintf(sql,"create table if not exists user(name char,code char);");
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec;%s\n",sqlite3_errmsg(ppdb));
        exit(1);
    }
    memset(sql,0,sizeof(sql));
    sprintf(sql,"insert into user values('%s','%s');",name,code);
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec;%s\n",sqlite3_errmsg(ppdb));
        exit(1);
    }
}
//回调函数
int callback(void *para,int argc,char **argv,char **argv_name)
{
    int i = 0;
    while(*(argv[1]+i) != '\0')
    {
        *((char *)para+i) = *(argv[1]+i);
        i++;
    }
    *((char *)para+i) = '\0';
    return 0;
}
//查询用户表格
int check_user_db(char *name,char *code)
{
    sqlite3 *ppdb;
    //打开或创建数据库
    int ret = sqlite3_open("user.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open:%s\n", sqlite3_errmsg(ppdb));
        return -1;
    }
    // //创建表
    char sql[128] = {0};
    sprintf(sql,"create table if not exists user(name char,code char);");
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec;%s\n",sqlite3_errmsg(ppdb));
        return -1;
    }
    memset(sql,0,sizeof(sql));
    //查询数据
    sprintf(sql,"select *from user where name = '%s' and code = '%s';",name,code);
    char *result = (char *)malloc(sizeof(char)*32);
    ret = sqlite3_exec(ppdb,sql,callback,result,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec;%s\n",sqlite3_errmsg(ppdb));
        return 0;
    }
    if(strcmp(result,code) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

//回调函数1
int callback1(void *para,int argc,char **argv,char **argv_name)
{
    *(int *)para = atoi(argv[1]);
    return 0;
}
//查询在线用户表格
int check_online_db( char *name)
{
    sqlite3 *ppdb;
    //打开或创建数据库
    int ret = sqlite3_open("user.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open:%s\n", sqlite3_errmsg(ppdb));
        exit(1);
    }
    // //创建表
    char sql[128] = {0};
    sprintf(sql,"create table if not exists online(name char,cfd int);");
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec:%s\n",sqlite3_errmsg(ppdb));
        exit(1);
    }
    memset(sql,0,sizeof(sql));
    //查询数据
    sprintf(sql,"select *from online where name = '%s'; ",name);
    int result;
    ret = sqlite3_exec(ppdb,sql,callback1,&result,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec:%s\n",sqlite3_errmsg(ppdb));
        exit(1);
    }
    return result;
}

//sta:查询在线用户的cfd发送群聊
void trans_o_cfd(char *name,char *sentence)
{
    sqlite3 *ppdb;
    //打开或创建数据库
    int ret = sqlite3_open("user.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open:%s\n", sqlite3_errmsg(ppdb));
        exit(1);
    }
    //查询数据
    char sql[128] = {0};
    sprintf(sql,"select *from online; ");
    char **result; 
    int row,column;
    ret = sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_get_table:%s\n",sqlite3_errmsg(ppdb)); 
        exit(1);
    }
    char Sentence[1024];
    char fa[1024]="发送了公屏消息：";
    strcpy(Sentence,name);
    strcat(Sentence,fa);
    strcat(Sentence,sentence);

    int j;
    int index = 2;
    int cfd;
    for(j=1;j<=row;j++)
    {
        cfd = atoi(result[j*2+1]);
        cho c_r;
            strcpy(c_r.chose,"r");
            strcpy(c_r.buf,"sta");
            if(send(cfd,&c_r,sizeof(c_r),0) == -1)
            {
                    perror("send1");
                    exit(1);
            }
        if(send(cfd,Sentence,sizeof(Sentence),0) == -1)
        {
            perror("send o_name");
            exit(1);
        }

    }
}


//che:输出在线表格名单
void trans_o_name(int cfd)
{
    sqlite3 *ppdb;
    //打开或创建数据库
    int ret = sqlite3_open("user.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open:%s\n", sqlite3_errmsg(ppdb));
        exit(1);
    }
    // //创建表
    char sql[128] = {0};
    sprintf(sql,"create table if not exists online(name char,cfd int);");
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec;%s\n",sqlite3_errmsg(ppdb));
        exit(1);
    }
    memset(sql,0,sizeof(sql));
    //查询数据
    sprintf(sql,"select *from online; ");
    char **result; 
    int row,column;
    ret = sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_get_table:%s\n",sqlite3_errmsg(ppdb)); 
        exit(1);
    }
    int j;
    int index = 2;
    char o_name[32];
    for(j=1;j<=row;j++)
    {
        strcpy(o_name,result[j*2]);
        if(send(cfd,o_name,sizeof(o_name),0) == -1)
        {
            perror("send o_name");
            exit(1);
        }
        memset(o_name,0,sizeof(o_name));
    }
    if(send(cfd,"quit",sizeof("quit"),0) == -1)
    {
        perror("send o_name");
        exit(1);
     }
}
//用户登录
void func_user(int cfd)
{
   while(1)
    {
        user u;
        memset(&u, 0, sizeof(u));
        if(recv(cfd, &u, sizeof(u),0)== -1)
        {
            perror("recv");
            exit(1);
        }
        if(u.chosen == 1)
        {
            // 在表格中查询用户
            int check = check_online_db(u.name);
            printf("check = %d\n",check_online_db(u.name));
            if(check_user_db(u.name,u.code)&&!(check > 0))
            {
                printf("用户%s登录成功\n",u.name);

                //写入在线表格
                write_online(u.name,cfd);

                char v[10] ="success";
                if(send(cfd,&v,sizeof(v),0) == -1)
                {
                    perror("send");
                    exit(1);
                }
                break;
            }
            else //没有查询到该用户
            {
                char v[10] = "fail";
                if(send(cfd,&v,sizeof(v),0) == -1)
                {
                    perror("send");
                    exit(1);
                }
                continue;
            }
        }
        if(u.chosen == 2)
        {
            //在表格中查询 看是否有重复注册
            if((check_user_db(u.name,u.code)) == 0 )
            {
                //写入用户表格
                write_user(u.name,u.code);
                char v =1;
                if(send(cfd,&v,sizeof(v),0) == -1)
                {
                    perror("send");
                    exit(1);
                }
                continue;
            }
            else//重复不写入
            {
                char v =0;
                if(send(cfd,&v,sizeof(v),0) == -1)
                {
                    perror("send");
                    exit(1);
                }
                continue;
            }
            
        }
        else
        {
            pthread_exit(NULL);
        }
    }    
}


//记录聊天记录
void chat_record(char *name,char *l_name,char *r_name,char *message)
{
    if(strcmp(l_name,r_name) > 0)
    {
        char temp [32];
        strcpy(temp,l_name);
        memset(l_name,0,32);
        strcpy(l_name,r_name);
        memset(r_name,0,sizeof(r_name));
        strcpy(r_name,temp);
    }
    sqlite3 *ppdb;
    //打开或创建数据库
    int ret = sqlite3_open("chat_record.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open:%s\n", sqlite3_errmsg(ppdb));
        exit(1);
    }
    // //创建表
    char sql[128] = {0};
    sprintf(sql,"create table if not exists '%s%s'(message char,time char);",l_name,r_name);
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec:%s\n",sqlite3_errmsg(ppdb));
        exit(1);
    }
    memset(sql,0,sizeof(sql));
    time_t t;
    char *pc;
    time(&t);
    pc = ctime(&t);
    sprintf(sql,"insert into '%s%s' values('%s:%s','%s');",l_name,r_name,name,message,pc);
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec;%s\n",sqlite3_errmsg(ppdb));
        exit(1);
    }
}
//rec:查找聊天记录
void check_record(char *l_name,char *r_name,int cfd)
{
    if(strcmp(l_name,r_name) > 0)
    {
        char temp [32];
        strcpy(temp,l_name);
        memset(l_name,0,32);
        strcpy(l_name,r_name);
        memset(r_name,0,sizeof(r_name));
        strcpy(r_name,temp);
    }
    int j;
    int index = 2;
    char sentence[1024];
    char time[1024];
    sqlite3 *ppdb;
    //打开或创建数据库
    int ret = sqlite3_open("chat_record.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open:%s\n", sqlite3_errmsg(ppdb));
        exit(1);
    }
    char sql[128];
    memset(sql,0,sizeof(sql));
    //查询数据
    sprintf(sql,"select *from '%s%s'; ",l_name,r_name);
    char **result; 
    int row,column;
    ret = sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL);
    if(ret == SQLITE_OK)
    {
        for(j=1;j<=row;j++)
        {
            strcpy(sentence,result[j*2]);
            strcpy(time,result[j*2+1]);
            strcat(time,sentence);
            if(send(cfd,time,sizeof(time),0) == -1)
            {
                perror("send sentence");
                exit(1);
            }
            // printf("sentence: %s\n",sentence);
            memset(time,0,sizeof(time));
            memset(sentence,0,sizeof(sentence));
        }
        
    }

    if(send(cfd,"no",sizeof("quit"),0) == -1)
    {
        perror("send o_name");
        exit(1);
     }
}
//ext:用户退出
void delete_o(int cfd)
{
    sqlite3 *ppdb;
    //打开或创建数据库
    int ret = sqlite3_open("user.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open:%s\n", sqlite3_errmsg(ppdb));
        exit(1);
    }
    char sql[128];
    sprintf(sql,"delete from online where cfd = %d; ",cfd);
    ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        // printf("sqlite3_exec;%s\n",sqlite3_errmsg(ppdb));
        // exit(1);
    }
}