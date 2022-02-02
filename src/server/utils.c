#include "server/utils.h"
#include "server/server.h"

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

void server_split_revc_info(char *i)
{
    char *str = strdup(i);
    strcpy(key, strsep(&str, "|"));
    char *noti;
    if ((noti = strsep(&str, "|")) != NULL)
        strcpy(info1, noti);
    if ((noti = strsep(&str, "|")) != NULL)
        strcpy(info2, noti);
}

void server_make_protocol(char *k, char *mesg)
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

void make_protocol_two_msgs(char *k, char *mesg1, char *mesg2)
{
    char *pr = (char *)malloc(sizeof(char) * MAX);
    strcpy(pr, k);
    if (mesg1 != NULL)
    {
        strcat(pr, "|");
        strcat(pr, mesg1);
    }
    if (mesg2 != NULL)
    {
        strcat(pr, "|");
        strcat(pr, mesg2);
    }
    send(connfd, pr, MAX, 0);
}

char *make_dict_path(char *username)
{
    char *path = (char *)malloc(sizeof(char) * MAX);
    strcpy(path, "../../../db/userDict/");
    strcat(path, username);
    strcat(path, "_dict.bt");
    return path;
}

char *make_note_path(char *username)
{
    char *path = (char *)malloc(sizeof(char) * MAX);
    strcpy(path, "../../../db/userNote/");
    strcat(path, username);
    strcat(path, "_note.bt");
    return path;
}
char *make_game_his_path(char *username)
{
    char *path = (char *)malloc(sizeof(char) * MAX);
    strcpy(path, "../../../db/userGameHis/");
    strcat(path, username);
    strcat(path, "_gameHis.txt");
    return path;
}

char *make_his_path(char *username)
{
    char *path = (char *)malloc(sizeof(char) * MAX);
    strcpy(path, "../../../db/userHis/");
    strcat(path, username);
    strcat(path, "_his.txt");
    return path;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void randomize(int arr[], int n)
{
    srand(time(NULL));

    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

NewWord *make_word(char *eng, char *vie)
{
    NewWord *w = (NewWord *)malloc(sizeof(NewWord));
    w->eng = strdup(eng);
    w->vie = strdup(vie);
    return w;
}

char *convert_int_to_string(int i)
{
    char *str = (char *)malloc(sizeof(char) * MAX);
    sprintf(str, "%d", i);
    return str;
}

void protocol_str_cat(char *str)
{
    strcat(practice_protocol_str, "|");
    strcat(practice_protocol_str, str);
}

void free_id_word()
{
    JRB ptr;
    jrb_traverse(ptr, note)
    {
        NewWord *w = (NewWord *)ptr->val.v;
        free(w->eng);
        free(w->vie);
        free(w);
    }
    jrb_free_tree(note);
}

void wrong_word_str_cat(int wrong_word_id)
{
    strcat(practice_protocol_str, "|");
    NewWord *wd = (NewWord *)(jrb_find_int(note, wrong_word_id)->val.v);
    strcat(practice_protocol_str, wd->vie);
}