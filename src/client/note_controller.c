#include "client/note_controller.h"

void add_to_note()
{
    char gettext[100];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
        show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Bạn chưa nhập vào từ cần thêm vào danh sách ghi chú");
    else
    {
        make_protocol("ANOTE", gettext, NULL);
        if (strcmp(key, "NOKE") == 0)
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", info1);
        else if (strcmp(key, "OKE") == 0)
            show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Thêm từ vào danh sách ghi chú thành công");
    }
}

void delete_from_note()
{
    char gettext[100];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));

    if (strcmp(gettext, "") == 0)
        show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Bạn chưa nhập vào từ cần xóa khỏi danh sách ghi chú");
    else
    {
        make_protocol("DNOTE", gettext, NULL);
        if (strcmp(key, "NOKE") == 0)
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", info1);
        else if (strcmp(key, "OKE") == 0)
            show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Xóa từ khỏi danh sách ghi chú thành công");
    }
}

void note()
{
    gtk_widget_destroy(window_note);
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../../../ui/dict-app.glade");

    window_note = GTK_WIDGET(gtk_builder_get_object(builder, "window_note"));
    gtk_builder_connect_signals(builder, NULL);

    textview2 = GTK_WIDGET(gtk_builder_get_object(builder, "textview2"));

    send(sockfd, "GNOTE", MAX, 0);
    recv(sockfd, recv_info, MAX, 0);
    puts(recv_info);
    char *word = (char *)malloc(sizeof(char) * 100);
    char *e;
    char *r = strdup(recv_info);
    strcpy(key, strsep(&r, "|"));
    GtkTextBuffer *buffer;
    GtkTextIter note_iter, start_note, end_note;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview2));
    gtk_text_buffer_get_start_iter(buffer, &start_note);
    gtk_text_buffer_get_end_iter(buffer, &end_note);
    gtk_text_buffer_delete(buffer, &start_note, &end_note);
    gtk_text_buffer_get_iter_at_offset(buffer, &note_iter, 0);
    if (strcmp(key, "OKE") == 0)
    {
        gtk_text_buffer_create_tag(buffer, "blue_fg", "foreground", "blue", NULL);
        while ((e = strsep(&r, "|")) != NULL)
        {
            sprintf(word, "%s\n", e);
            gtk_text_buffer_insert_with_tags_by_name(buffer, &note_iter, word, -1, "blue_fg", NULL);
        }
    }
    else if (strcmp(key, "NOKE") == 0)
    {
        if ((e = strsep(&r, "|")) != NULL)
        {
            gtk_text_buffer_create_tag(buffer, "red_fg", "foreground", "red", NULL);
            gtk_text_buffer_insert_with_tags_by_name(buffer, &note_iter, e, -1, "red_fg", NULL);
        }
    }
    g_object_unref(builder);
    gtk_widget_show(window_note);
}

void del_all_note()
{
    make_protocol("DANOTE", user, NULL);
    if (strcmp(key, "NOKE") == 0)
        show_message(window_note, GTK_MESSAGE_ERROR, "ERROR!", info1);
    else if (strcmp(key, "OKE") == 0)
    {
        show_message(window_note, GTK_MESSAGE_INFO, "SUCCESS!", "Xóa tất cả từ khỏi danh sách ghi chú thành công");
        GtkTextBuffer *buffer;
        GtkTextIter note_iter, start_note, end_note;
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview2));
        gtk_text_buffer_get_start_iter(buffer, &start_note);
        gtk_text_buffer_get_end_iter(buffer, &end_note);
        gtk_text_buffer_delete(buffer, &start_note, &end_note);
        gtk_text_buffer_get_iter_at_offset(buffer, &note_iter, 0);
        gtk_text_buffer_create_tag(buffer, "red_fg", "foreground", "red", NULL);
        gtk_text_buffer_insert_with_tags_by_name(buffer, &note_iter, "Danh sách ghi chú rỗng", -1, "red_fg", NULL);
    }
}