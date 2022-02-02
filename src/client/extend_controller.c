#include "client/extend_controller.h"

void extend()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../../../ui/dict-app.glade");

    window_advanced = GTK_WIDGET(gtk_builder_get_object(builder, "window_advanced"));
    gtk_builder_connect_signals(builder, NULL);

    entry_newword = GTK_WIDGET(gtk_builder_get_object(builder, "entry_newword"));
    entry_meanword = GTK_WIDGET(gtk_builder_get_object(builder, "entry_meanword"));
    entry_del = GTK_WIDGET(gtk_builder_get_object(builder, "entry_del"));

    gchar gettext[100];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));

    if (strlen(gettext) > 0)
    {
        gtk_entry_set_text(GTK_ENTRY(entry_newword), gettext);
        gtk_entry_set_text(GTK_ENTRY(entry_del), gettext);
    }

    g_object_unref(builder);
    gtk_widget_show(window_advanced);
}

void add_to_dict()
{
    gchar new_word[MAX];
    gchar mean_word[MAX];
    strcpy(new_word, gtk_entry_get_text(GTK_ENTRY(entry_newword)));
    strcpy(mean_word, gtk_entry_get_text(GTK_ENTRY(entry_meanword)));
    if (strcmp(new_word, "") == 0 || strcmp(mean_word, "") == 0)
        show_message(window_advanced, GTK_MESSAGE_ERROR, "ERROR!", "Thông tin thêm từ còn thiếu");
    else
    {
        make_protocol("ADICT", new_word, mean_word);
        if (strcmp(key, "NOKE") == 0)
            show_message(window_advanced, GTK_MESSAGE_ERROR, "ERROR!", info1);
        else if (strcmp(key, "OKE") == 0)
            show_message(window_advanced, GTK_MESSAGE_INFO, "SUCCESS!", "Thêm từ thành công");
    }
}

void repair_word()
{
    gchar new_word[MAX];
    gchar mean_word[MAX];
    strcpy(new_word, gtk_entry_get_text(GTK_ENTRY(entry_newword)));
    strcpy(mean_word, gtk_entry_get_text(GTK_ENTRY(entry_meanword)));
    if (strcmp(new_word, "") == 0 || strcmp(mean_word, "") == 0)
        show_message(window_advanced, GTK_MESSAGE_ERROR, "ERROR!", "Thông tin sửa từ còn thiếu");
    else
    {
        make_protocol("EDIT", new_word, mean_word);
        if (strcmp(key, "NOKE") == 0)
            show_message(window_advanced, GTK_MESSAGE_ERROR, "ERROR!", info1);
        else if (strcmp(key, "OKE") == 0)
            show_message(window_advanced, GTK_MESSAGE_INFO, "SUCCESS!", "Sửa từ thành công");
    }
}

void delete_from_dict()
{
    gchar gettext[MAX];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(entry_del)));
    if (strcmp(gettext, "") == 0)
        show_message(window_advanced, GTK_MESSAGE_ERROR, "ERROR!", "Bạn chưa nhập từ cần xóa");
    else
    {
        make_protocol("DDICT", gettext, NULL);
        if (strcmp(key, "NOKE") == 0)
            show_message(window_advanced, GTK_MESSAGE_ERROR, "ERROR!", info1);
        else if (strcmp(key, "OKE") == 0)
            show_message(window_advanced, GTK_MESSAGE_INFO, "SUCCESS!", "Xóa từ thành công");
    }
}
