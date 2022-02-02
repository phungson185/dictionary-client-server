#ifndef _MAINCONTROLLER_H_
#define _MAINCONTROLLER_H_

#include "client/client.h"
#include "client/utils.h"

void show_main_window();
void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void translate();
void add_to_history(char *str);
int strremove(char *str, char *sub);
void clear_history();
void get_history();


#endif
