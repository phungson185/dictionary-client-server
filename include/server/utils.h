#ifndef _UTILS_H_
#define _UTILS_H_

#include "server/server.h"

typedef struct
{
    char *eng;
    char *vie;
} NewWord;

void sig_chld(int signo);
void server_split_revc_info(char *i);
void server_make_protocol(char *k, char *mesg);
void make_protocol_two_msgs(char *k, char *mesg1, char *mesg2);
char *make_dict_path(char *username);
char *make_note_path(char *username);
char *make_game_his_path(char *username);
char *make_his_path(char *username);
void swap(int *a, int *b);
void printArray(int arr[], int n);
void randomize(int arr[], int n);
NewWord *make_word(char *eng, char *vie);
char *convert_int_to_string(int i);
void protocol_str_cat(char *str);
void free_id_word();
void wrong_word_str_cat(int wrong_word_id);

#endif
