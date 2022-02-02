#ifndef _UTILS_H_
#define _UTILS_H_

#include "client/client.h"

void show_message(GtkWidget *parent, GtkMessageType type, char *mms, char *content);
void make_protocol(char *k, char *i1, char *i2);
void set_mean_textview_text(GtkWidget *textview, char *text);
char *make_long_to_string(long l);
GtkTextBuffer* clear_textview(GtkWidget *textview);

#endif
