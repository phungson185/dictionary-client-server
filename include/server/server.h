#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "lib/jrb.h"
#include "lib/jval.h"
#include "btree-lib/inc/btree.h"


#define MAXLINE 4096
#define MAX 1000     

#define SERV_PORT 3000 
#define LISTENQ 8  

int connfd;
char username[50];
char suggest_protocol_str[MAX];
char get_note_protol_str[MAX];
char practice_protocol_str[MAXLINE];
char start_practice_protocol_str[MAXLINE];
int *note_id_arr;
char key[MAX];
char info1[MAX];
char info2[MAX];
int note_size;
int next_question;

char recv_info[MAX];
char his[MAX];

FILE *f;

BTA *user;
BTA *dict;
BTA *user_dict;
BTA *user_note;
JRB note;

#endif
