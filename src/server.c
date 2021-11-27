#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>

#include <jrb.h>
#include <jval.h>
#include <btree.h>

#define MAXLINE 4096 /*max text line length*/
#define MAX 1000     /*max text line length*/

#define SERV_PORT 3000 /*port*/
#define LISTENQ 8      /*maximum number of client connections */

int connfd;
char username[MAX];
char suggest_protocol_str[MAX];

char key[MAX];
char info1[MAX];
char info2[MAX];

char recv_info[MAX];

BTA *user;
BTA *dict;
BTA *user_dict;

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

void split_revc_info(char *i)
{
    char *str = strdup(i);
    strcpy(key, strsep(&str, "|"));
    char *noti;
    if ((noti = strsep(&str, "|")) != NULL)
        strcpy(info1, noti);
    if ((noti = strsep(&str, "|")) != NULL)
        strcpy(info2, noti);
}

void make_protocol(char *k, char *mesg)
{
    char *pr = (char *)malloc(sizeof(char) * MAX);
    strcpy(pr, k);
    if (mesg != NULL)
    {
        strcat(pr, "|");
        strcat(pr, mesg);
    }
    send(connfd, pr, MAX, 0);
}

char *make_dict_path(char *username)
{
    char *path = (char *)malloc(sizeof(char) * MAX);
    strcpy(path, "../src/data/");
    strcat(path, username);
    strcat(path, "_dict.bt");
    return path;
}

void registerr()
{
    int rsize;
    user = btopn("../src/data/user.bt", 0, 0);
    btpos(user, ZSTART);
    if (!btsel(user, info1, info2, MAX, &rsize))
        make_protocol("NOKE", "Tài khoản đã tồn tại, vui lòng chọn tài khoản khác");
    else
    {
        if (!btins(user, info1, info2, MAX))
        {
            if (btcrt(make_dict_path(info1), 0, 0) == NULL)
            {
                perror("Lỗi không thể tạo file từ điển cá nhân");
                return -1;
            }
            make_protocol("OKE", NULL);
        }
        else
            make_protocol("NOKE", "Đăng ký thất bại, chương trình lỗi...");
    }
    btcls(user);
}

void login()
{
    char *pass = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    user = btopn("../src/data/user.bt", 0, 0);
    btpos(user, ZSTART);

    if (btsel(user, info1, pass, MAX, &rsize))
    {
        make_protocol("NOKE", "Tài khoản không tồn tại");
    }
    else
    {
        if (strcmp(pass, info2) != 0)
            make_protocol("NOKE", "Mật khẩu không đúng");
        else
        {
            strcpy(username, info1);
            make_protocol("OKE", NULL);
        }
    }
    btcls(user);
}

void suggest_query(BTA *b)
{
    char kd[2];
    int rsize, k = 0, count = 0;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    kd[0] = info1[0];
    kd[1] = '\0';

    if (!btsel(b, kd, vie, sizeof(char *), &rsize))
    {
        while (!btseln(b, eng, vie, MAX, &rsize))
        {
            if (eng[0] != info1[0])
                break;
            for (int i = 0; i < strlen(info1); i++)
            {
                if (eng[i] != info1[i])
                {
                    k = 1;
                    break;
                }
            }
            if (k == 0)
            {
                count++;
                strcat(suggest_protocol_str,"|");
                strcat(suggest_protocol_str,eng);
            }
            k = 0;
            if (count > 20)
                break;
        }
    }
    free(eng);
    free(vie);
}

void suggestion()
{
    dict = btopn("../src/data/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    strcpy(suggest_protocol_str,"ENG");
    suggest_query(user_dict);
    suggest_query(dict);
    puts(suggest_protocol_str);
    send(connfd, suggest_protocol_str, MAX, 0);
    btcls(dict);
    btcls(user_dict);
}

int main(int argc, char **argv)
{
    int listenfd, n;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    //Create a socket for the soclet
    //If sockfd<0 there was an error in the creation of the socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in btreating the socket");
        exit(2);
    }

    //preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    //bind the socket
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    //listen to the socket by creating a connection queue, then wait for clients
    listen(listenfd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    for (;;)
    {

        clilen = sizeof(cliaddr);
        //accept a connection
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        printf("%s\n", "Received request...");

        if ((childpid = fork()) == 0)
        { //if it’s 0, it’s child process

            printf("%s\n", "Child created for dealing with client requests");

            //close listening socket
            close(listenfd);

            while ((n = recv(connfd, recv_info, MAXLINE, 0)) > 0)
            {
                printf("%s", "String received from the client:");
                puts(recv_info);
                split_revc_info(recv_info);
                if (strcmp("REG", key) == 0)
                    registerr();
                else if (strcmp("LOG", key) == 0)
                    login();
                else if (strcmp("SUG", key) == 0)
                    suggestion();
            }

            if (n < 0)
                printf("%s\n", "Read error");

            exit(0);
        }
        signal(SIGCHLD, sig_chld);

        //close socket of the server
        close(connfd);
    }
}