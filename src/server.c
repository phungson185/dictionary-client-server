#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <time.h>
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
char get_note_protol_str[MAX];
char practice_protocol_str[MAXLINE];
char start_practice_protocol_str[MAXLINE];
int *note_id_arr;
char key[MAX];
char info1[MAX];
char info2[MAX];
int note_size=0;
int next_question=0;

char recv_info[MAX];

BTA *user;
BTA *dict;
BTA *user_dict;
BTA *user_note;
JRB note = NULL;

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
    strcpy(path, "../db/userDict/");
    strcat(path, username);
    strcat(path, "_dict.bt");
    return path;
}

char *make_note_path(char *username)
{
    char *path = (char *)malloc(sizeof(char) * MAX);
    strcpy(path, "../db/userNote/");
    strcat(path, username);
    strcat(path, "_note.bt");
    return path;
}
char *make_game_his_path(char *username)
{
    char *path = (char *)malloc(sizeof(char) * MAX);
    strcpy(path, "../db/userGameHis/");
    strcat(path, username);
    strcat(path, "_gameHis.txt");
    return path;
}

void registerr()
{
    int rsize;
    user = btopn("../db/user.bt", 0, 0);
    btpos(user, ZSTART);
    if (!btsel(user, info1, info2, MAX, &rsize))
        make_protocol("NOKE", "Tài khoản đã tồn tại, vui lòng chọn tài khoản khác");
    else
    {
        if (!btins(user, info1, info2, MAX))
        {
            if ((user_dict = btcrt(make_dict_path(info1), 0, 0)) == NULL)
            {
                perror("Lỗi không thể tạo file từ điển cá nhân");
                return;
            }
            if ((user_note = btcrt(make_note_path(info1), 0, 0)) == NULL)
            {
                perror("Lỗi không thể tạo file note");
                return;
            }
            make_protocol("OKE", NULL);
        }
        else
            make_protocol("NOKE", "Đăng ký thất bại, chương trình lỗi...");
    }
    btcls(user);
    btcls(user_dict);
    btcls(user_note);
}

void login()
{
    char *pass = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    user = btopn("../db/user.bt", 0, 0);
    btpos(user, ZSTART);

    if (btsel(user, info1, pass, MAX, &rsize))
        make_protocol("NOKE", "Tài khoản không tồn tại");
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

void scan_dict(BTA *b)
{
    int rsize, k = 0, count = 0;

    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
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
            strcat(suggest_protocol_str, "|");
            strcat(suggest_protocol_str, eng);
        }
        k = 0;
        if (count > 20)
            break;
    }
    free(eng);
    free(vie);
}

void suggest_query(BTA *b)
{
    char kd[2];
    int rsize;
    char *vie = (char *)malloc(sizeof(char) * MAX);

    if (b == dict)
    {
        kd[0] = info1[0];
        kd[1] = '\0';

        if (!btsel(b, kd, vie, sizeof(char *), &rsize))
        {
            scan_dict(b);
        }
    }
    else
    {
        btpos(b, ZSTART);
        scan_dict(b);
    }
    free(vie);
}

void suggestion()
{
    dict = btopn("../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    strcpy(suggest_protocol_str, "ENG");
    suggest_query(user_dict);
    suggest_query(dict);
    send(connfd, suggest_protocol_str, MAX, 0);
    btcls(dict);
    btcls(user_dict);
}

void translate()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    char *edited_mean = (char *)malloc(sizeof(char) * MAX);
    char *origin_mean = (char *)malloc(sizeof(char) * MAX);
    dict = btopn("../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    btpos(dict, ZSTART);
    btpos(user_dict, ZSTART);
    int rsize;
    strcpy(edited_mean, "");
    strcpy(origin_mean, "");

    if (!btsel(user_dict, info1, value, MAX, &rsize))
        strcpy(edited_mean, value);
    if (!btsel(dict, info1, value, MAX, &rsize))
        strcpy(origin_mean, value);
    if (strlen(edited_mean) == 0 && strlen(origin_mean) == 0)
        make_protocol("NOKE", "Không tìm thấy từ");
    else
        make_protocol_two_msgs("VIE", edited_mean, origin_mean);
    free(value);
    free(edited_mean);
    free(origin_mean);
    btcls(dict);
    btcls(user_dict);
}

void add_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    dict = btopn("../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    btpos(dict, ZSTART);
    btpos(user_dict, ZSTART);
    if (!btsel(user_dict, info1, value, MAX, &rsize) || !btsel(dict, info1, value, MAX, &rsize))
        make_protocol("NOKE", "Từ đã tồn tại trong từ điển");
    else
    {
        if (!btins(user_dict, info1, info2, MAX))
            make_protocol("OKE", NULL);
        else
            make_protocol("NOKE", "Thêm từ thất bại");
    }
    free(value);
    btcls(dict);
    btcls(user_dict);
}

void edit_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    dict = btopn("../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    if (!btsel(user_dict, info1, value, MAX, &rsize))
    {
        if (!btupd(user_dict, info1, info2, MAX))
            make_protocol("OKE", NULL);
        else
            make_protocol("NOKE", "Sửa từ thất bại");
    }
    else
    {
        if (!btsel(dict, info1, value, MAX, &rsize))
        {
            if (!btins(user_dict, info1, info2, MAX))
                make_protocol("OKE", NULL);
            else
                make_protocol("NOKE", "Sửa từ thất bại");
        }
        else
            make_protocol("NOKE", "Từ bạn muốn chỉnh sửa không có trong từ điển");
    }
    free(value);
    btcls(dict);
    btcls(user_dict);
}

void del_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    user_dict = btopn(make_dict_path(username), 0, 0);
    btpos(user_dict, ZSTART);
    if (!btsel(user_dict, info1, value, MAX, &rsize))
    {
        if (!btdel(user_dict, info1))
            make_protocol("OKE", NULL);
        else
            make_protocol("NOKE", "Xóa từ thất bại");
    }
    else
        make_protocol("NOKE", "Từ bạn muốn xóa không có trong từ điển");

    free(value);
    btcls(user_dict);
}

void add_note()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    dict = btopn("../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    user_note = btopn(make_note_path(username), 0, 0);

    if (!btsel(user_note, info1, value, MAX, &rsize))
        make_protocol("NOKE", "Từ này đã có trong danh sách ghi chú");
    else
    {
        if (!btsel(user_dict, info1, value, MAX, &rsize))
        {
            if (!btins(user_note, info1, value, MAX))
                make_protocol("OKE", NULL);
            else
                make_protocol("NOKE", "Thêm ghi chú thất bại");
        }
        else if (!btsel(dict, info1, value, MAX, &rsize))
        {
            if (!btins(user_note, info1, value, MAX))
                make_protocol("OKE", NULL);
            else
                make_protocol("NOKE", "Thêm ghi chú thất bại");
        }
        else
            make_protocol("NOKE", "Từ bạn muốn thêm ghi chú không có trong từ điển");
    }
    free(value);
    btcls(dict);
    btcls(user_dict);
    btcls(user_note);
}

void del_note()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    user_note = btopn(make_note_path(username), 0, 0);
    if (!btsel(user_note, info1, value, MAX, &rsize))
    {
        if (!btdel(user_note, info1))
            make_protocol("OKE", NULL);
        else
            make_protocol("NOKE", "Xóa ghi chú thất bại");
    }
    else
        make_protocol("NOKE", "Từ bạn muốn xóa không có trong danh sách ghi chú");
    free(value);
    btcls(user_note);
}

void del_all_note()
{
    if ((user_note = btcrt(make_note_path(info1), 0, 0)) == NULL)
        make_protocol("NOKE", "Xóa tất cả ghi chú thất bại");
    else
        make_protocol("OKE", NULL);
    btcls(user_note);
}

void get_note()
{
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    int rsize, isEmpty = 1;
    user_note = btopn(make_note_path(username), 0, 0);
    btpos(user_note, ZSTART);
    strcpy(get_note_protol_str, "OKE");
    while (!btseln(user_note, eng, vie, MAX, &rsize))
    {
        isEmpty = 0;
        strcat(get_note_protol_str, "|");
        strcat(get_note_protol_str, eng);
    }
    if (isEmpty)
        make_protocol("NOKE", "Danh sách ghi chú rỗng");
    else
        send(connfd, get_note_protol_str, MAX, 0);
    free(eng);
    free(vie);
    btcls(user_note);
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

typedef struct
{
    char *eng;
    char *vie;
} word;

word *make_word(char *eng, char *vie)
{
    word *w = (word *)malloc(sizeof(word));
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
        word *w = (word *)ptr->val.v;
        free(w->eng);
        free(w->vie);
        free(w);
    }
    jrb_free_tree(note);
}

void wrong_word_str_cat(int wrong_word_id)
{
    strcat(practice_protocol_str, "|");
    word *wd = (word *)(jrb_find_int(note, wrong_word_id)->val.v);
    strcat(practice_protocol_str, wd->vie);
}
void practice(){
    strcpy(start_practice_protocol_str, "OKE");
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    int rsize = 0;
    int correct_answer_position;
    int wrong_answer_id1 = 0, wrong_answer_id2 = 0, wrong_answer_id3 = 0;
    note = make_jrb();
    user_note = btopn(make_note_path(username), 0, 0);
    // scan user note to make note tree
    btpos(user_note, ZSTART);
    
    while (!btseln(user_note, eng, vie, MAX, &rsize))
    {
        jrb_insert_int(note, note_size, (Jval){.v = make_word(eng, vie)});
        note_size++;
    }

    if (note_size < 4)
        make_protocol("NOKE", "Danh sách từ ghi chú cần có 4 từ trở lên!");
    else
    {
        // insert note size to protocol
        strcat(start_practice_protocol_str, "|");
        strcat(start_practice_protocol_str, convert_int_to_string(note_size));
        send(connfd, start_practice_protocol_str, MAX, 0);
    }

    note_id_arr = (int *)malloc(sizeof(int) * note_size);
    for (int i = 0; i < note_size; i++)
        note_id_arr[i] = i;
    randomize(note_id_arr, note_size);

    free(eng);
    free(vie);
    btcls(user_note);
}
void new_question()
{
    strcpy(practice_protocol_str, "OKE");
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    // int rsize, note_size = 0;
    int correct_answer_position;
    int wrong_answer_id1 = 0, wrong_answer_id2 = 0, wrong_answer_id3 = 0;
    
    // get word of question
    word *w = (word *)(jrb_find_int(note, note_id_arr[next_question])->val.v);
    
    // insert correct answer and position to protocol
    protocol_str_cat(w->eng);
    protocol_str_cat(convert_int_to_string(1 + rand() % 4));
    protocol_str_cat(w->vie);

    // generate wrong answer id
    srand(time(NULL));
    wrong_answer_id1 = wrong_answer_id2 = wrong_answer_id3 = note_id_arr[next_question];
    while (wrong_answer_id1 == note_id_arr[next_question])
        wrong_answer_id1 = rand() % note_size;
    while (wrong_answer_id2 == note_id_arr[next_question] || wrong_answer_id2 == wrong_answer_id1)
        wrong_answer_id2 = rand() % note_size;
    while (wrong_answer_id3 == note_id_arr[next_question] || wrong_answer_id3 == wrong_answer_id1 || wrong_answer_id3 == wrong_answer_id2)
        wrong_answer_id3 = rand() % note_size;

    // insert wrong answer to protocol
    wrong_word_str_cat(wrong_answer_id1);
    wrong_word_str_cat(wrong_answer_id2);
    wrong_word_str_cat(wrong_answer_id3);
    send(connfd, practice_protocol_str, MAX, 0);
    next_question++;
    // if(next_question==note_size){
        // free(note_id_arr);
        // free_id_word();
    // }
    free(eng);
    free(vie);
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

            while ((n = recv(connfd, recv_info, MAX, 0)) > 0)
            {
                printf("%s", "String received from client: ");
                puts(recv_info);
                split_revc_info(recv_info);
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