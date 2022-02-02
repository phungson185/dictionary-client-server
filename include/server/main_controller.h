#ifndef _MAINCONTROLLER_H_
#define _MAINCONTROLLER_H_

#include "server/server.h"



void scan_dict(BTA *b);
void suggest_query(BTA *b);
void suggestion();
void get_history();
void del_his();
void new_history_handle(char *text);
int strremove(char *str, char *sub);
void add_to_history(char *buf);
void translate();

#endif
