#include "client/main_controller.h"

void show_main_window()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../../../ui/dict-app.glade");

    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    searchentry = GTK_WIDGET(gtk_builder_get_object(builder, "searchentry"));

    textview1 = GTK_WIDGET(gtk_builder_get_object(builder, "textview1"));
    textview_his = GTK_WIDGET(gtk_builder_get_object(builder, "textview_his"));
    username = GTK_WIDGET(gtk_builder_get_object(builder, "username"));

    gtk_label_set_text(GTK_LABEL(username), info1);

    comple = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(comple, 0);
    list = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_entry_completion_set_model(comple, GTK_TREE_MODEL(list));
    gtk_entry_set_completion(GTK_ENTRY(searchentry), comple);

    g_object_unref(builder);
    gtk_widget_show(window_main);
    gtk_widget_hide(window_login);

    get_history();
    g_signal_connect(searchentry, "key_press_event", G_CALLBACK(on_key_press), NULL);
    g_signal_connect(window_main, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    char *pr = (char *)malloc(sizeof(char) * MAX);
    gchar gettext[100];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    int text_length = strlen(gettext);
    if (event->keyval != GDK_KEY_BackSpace)
    {
        gettext[text_length] = event->keyval;
        gettext[text_length + 1] = '\0';
    }
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "");
    }
    else
    {
        gtk_list_store_clear(list);
        strcpy(pr, "SUG|");
        strcat(pr, gettext);
        send(sockfd, pr, MAX, 0);
        recv(sockfd, recv_info, MAX, 0);
        char *e;
        char *r = strdup(recv_info);
        strcpy(key, strsep(&r, "|"));
        if (strcmp(key, "ENG") == 0)
        {
            while ((e = strsep(&r, "|")) != NULL)
            {
                gtk_list_store_append(list, &suggestion_iter);
                gtk_list_store_set(list, &suggestion_iter, 0, e, -1);
            }
        }
    }
    free(pr);
}

void translate()
{
    int rsize;
    char *edited_mean = (char *)malloc(sizeof(char) * MAX);
    char *origin_mean = (char *)malloc(sizeof(char) * MAX);
    gchar gettext[MAX];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
        set_mean_textview_text(textview1, "");
    else
    {
        make_protocol("ENG", gettext, NULL);
        if (strcmp(key, "NOKE") == 0)
        {
            show_message(window_login, GTK_MESSAGE_ERROR, "ERROR!", info1);
            set_mean_textview_text(textview1, "");
        }
        else if (strcmp(key, "VIE") == 0)
        {
            GtkTextBuffer *buffer;
            GtkTextIter translation_iter, start, end;
            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview1));
            gtk_text_buffer_get_start_iter(buffer, &start);
            gtk_text_buffer_get_end_iter(buffer, &end);
            gtk_text_buffer_delete(buffer, &start, &end);
            gtk_text_buffer_create_tag(buffer, "blue_fg", "foreground", "blue", NULL);
            gtk_text_buffer_create_tag(buffer, "red_fg", "foreground", "red", NULL);
            gtk_text_buffer_get_iter_at_offset(buffer, &translation_iter, 0);
            if (strcmp(info1, "") == 0)
                gtk_text_buffer_insert_with_tags_by_name(buffer, &translation_iter, info2, -1, "blue_fg", NULL);
            else if (strcmp(info2, "") == 0)
                gtk_text_buffer_insert_with_tags_by_name(buffer, &translation_iter, info1, -1, "blue_fg", NULL);
            else
            {
                sprintf(edited_mean, "Nghĩa đã sửa: %s\n", info1);
                sprintf(origin_mean, "Nghĩa gốc: %s", info2);
                gtk_text_buffer_insert_with_tags_by_name(buffer, &translation_iter, edited_mean, -1, "red_fg", NULL);
                gtk_text_buffer_insert_with_tags_by_name(buffer, &translation_iter, origin_mean, -1, "blue_fg", NULL);
            }
            strcpy(info1, "");
            // get_history();
            add_to_history(gettext);
            set_mean_textview_text(textview_his, htr);
        }
    }
    free(edited_mean);
    free(origin_mean);
}

void add_to_history(char *str)
{
    char *buffer = (char *)malloc(sizeof(char) * MAX);
    char *buftrans = (char *)malloc(sizeof(char) * MAX);
    sprintf(buftrans, "%s\n", str);
    int i = strremove(htr, buftrans);
    strcpy(buffer, buftrans);
    strcat(buffer, htr);
    strcpy(htr, buffer);
    free(buffer);
}

int strremove(char *str, char *sub)
{
    size_t len = strlen(sub);
    if (len > 0)
    {
        char *p = str;
        if ((p = strstr(p, sub)) != NULL)
        {
            memmove(p, p + len, strlen(p + len) + 1);
            return 1;
        }
    }
    return 0;
}

void clear_history()
{
    strcpy(htr, "");
    set_mean_textview_text(textview_his, htr);
    send(sockfd, "DHIS", MAX, 0);
    recv(sockfd, recv_info, MAX, 0);
    printf("String received from server: ");
    puts(recv_info);
    char *str = strdup(recv_info);
    if (strcmp(str, "OKE") == 0)
    {
        strcpy(htr, "");
        set_mean_textview_text(textview_his, htr);
        show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Xóa lịch sử tra cứu thành công");
    }
    else
    {
        show_message(window_main, GTK_MESSAGE_ERROR, "ERROR", "Xóa lịch lịch sử tra cứu thất bại");
    }
}

void get_history()
{
    make_protocol("SHIS", user, NULL);
    if (strcmp(key, "NOKE") == 0)
        printf("%s", info1);
    else if (strcmp(key, "OKE") == 0)
    {
        strcpy(htr, info1);
        set_mean_textview_text(textview_his, htr);
    }
}
