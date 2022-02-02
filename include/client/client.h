#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLINE 4096 
#define SERV_PORT 3000 
#define MAX 1000

GtkBuilder *builder;
GtkWidget *window_login, *window_register, *window_main, *window_advanced, *window_note, *window_about, *window_practice, *window_game, *window_profile, *window_change_pass;
GtkEntryCompletion *comple;

GtkListStore *list;
GtkTreeIter suggestion_iter;

GtkEntry *log_acc, *log_pass, *reg_acc, *reg_pass, *retype_pass, *searchentry, *entry_newword, *entry_meanword, *entry_del, *old_pass, *new_pass, *retype_new_pass;

GtkWidget *reg_noti, *log_noti, *username, *textview1, *textview2, *textview3, *textview4, *textview_his;

GtkWidget *lbl_eng, *lbl_count_question, *lbl_total_question, *lbl_count_correct, *lbl_vie1, *lbl_vie2, *lbl_vie3, *lbl_vie4;

GtkWidget *btn_vie1, *btn_vie2, *btn_vie3, *btn_vie4;

char htr[MAX];
char buftrans[MAX];
int sockfd;
char key[MAX];
char info1[MAX];
char info2[MAX];
char recv_info[MAX];
char recv_question[MAXLINE];
char user[MAX];

GdkColor red;
GdkColor green;

char *spliting_str;
static long count_question = 0;
static int correct_position = 0;
static int choose_position = 0;
char recv_start[MAX];
static long game_size = 0;
typedef struct game_result
{
    long total;
    long correct_num;
};
struct game_result game_result;

int main_handler(int argc, char **argv);

#endif
