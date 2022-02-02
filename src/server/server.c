#include "server/server.h"
#include "server/utils.h"

int main(int argc, char **argv)
{
    int listenfd, n;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in btreating the socket");
        exit(2);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.106");
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    for (;;)
    {

        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        printf("%s\n", "Received request...");

        if ((childpid = fork()) == 0)
        { 

            printf("%s\n", "Child created for dealing with client requests");

            close(listenfd);

            while ((n = recv(connfd, recv_info, MAX, 0)) > 0)
            {
                printf("%s", "String received from client: ");
                puts(recv_info);
                server_split_revc_info(recv_info);
                if (strcmp("REG", key) == 0)
                    registerr();
                else if (strcmp("LOG", key) == 0)
                    login();
                else if (strcmp("SUG", key) == 0)
                    suggestion();
                else if (strcmp("ENG", key) == 0)
                    translate();
                else if (strcmp("ADICT", key) == 0)
                    add_dict();
                else if (strcmp("EDIT", key) == 0)
                    edit_dict();
                else if (strcmp("DDICT", key) == 0)
                    del_dict();
                else if (strcmp("ANOTE", key) == 0)
                    add_note();
                else if (strcmp("DNOTE", key) == 0)
                    del_note();
                else if (strcmp("GNOTE", key) == 0)
                    get_note();
                else if (strcmp("DANOTE", key) == 0)
                    del_all_note();
                else if (strcmp("PRAC", key) == 0)
                    practice();
                else if (strcmp("NEWQ", key) == 0)
                    new_question();
                else if (strcmp("EXIT", key) == 0)
                    exit_game();
                else if (strcmp("SHIS", key) == 0)
                    get_history();
                else if (strcmp("DHIS", key) == 0)
                    del_his();
                else if (strcmp("GGHIS", key) == 0)
                    get_game_his();
                else if (strcmp("DGHIS", key) == 0)
                    del_game_his();
                else if (strcmp("CPASS", key) == 0)
                    change_pass();
                else printf("%s\n", "Wrong protocol");
            }

            if (n < 0)
                printf("%s\n", "Read error");

            exit(0);
        }
        signal(SIGCHLD, sig_chld);

        close(connfd);
    }
}