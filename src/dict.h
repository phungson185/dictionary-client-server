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

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define MAX 1000

typedef enum
{
    ERROR,
    SUCCESS
} RES;

GtkBuilder *builder;
GtkWidget *window_login, *window_register, *window_main, *window_advanced, *window_note, *window_about;
GtkEntryCompletion *comple;

GtkListStore *list;
GtkTreeIter Iter;

GtkEntry *log_acc, *log_pass, *reg_acc, *reg_pass, *retype_pass, *searchentry, *entry_newword, *entry_meanword, *entry_del;

GtkWidget *reg_noti, *log_noti, *username, *textview1, *textview2, *textview3, *textview4, *textview_his;

BTA *dict;
BTA *note;
char htr[MAX];
char buftrans[MAX];
int sockfd;
char key[MAX];
char info1[MAX];
char info2[MAX];
char recv_info[MAX];

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

int main_handler(int argc, char **argv);

#endif