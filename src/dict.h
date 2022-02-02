#ifndef __DICT_H__
#define __DICT_H__

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <jrb.h>
#include <jval.h>
#include <string.h>
#include <btree.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/
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

void datainit();
void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void set_mean_textview_text(GtkWidget *textview, char *text);
void set_label_empty_text(GtkWidget *widget);
void translate();
void delay(int number_of_seconds);
void make_protocol(char *k, char *i1, char *i2);
void show_main_window();
void get_history();

int main_handler(int argc, char **argv);

#endif