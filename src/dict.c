#include "dict.h"

int main_handler(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    gdk_color_parse("red", &red);
    gdk_color_parse("green", &green);

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");
    window_login = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));

    gtk_builder_connect_signals(builder, NULL);

    log_acc = GTK_WIDGET(gtk_builder_get_object(builder, "log_acc"));
    log_pass = GTK_WIDGET(gtk_builder_get_object(builder, "log_pass"));
    log_noti = GTK_WIDGET(gtk_builder_get_object(builder, "log_noti"));

    // g_signal_connect(reg_acc, "key_press_event", G_CALLBACK(set_label_empty_text(reg_noti)), NULL);
    // g_signal_connect(reg_pass, "key_press_event", G_CALLBACK(set_label_empty_text(reg_noti)), NULL);
    // g_signal_connect(retype_pass, "key_press_event", G_CALLBACK(set_label_empty_text(reg_noti)), NULL);

    g_signal_connect(window_login, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // datainit();
    // dict = btopn("dict.bt", 0, 0);

    g_object_unref(builder);
    gtk_widget_show(window_login);
    gtk_main();

    return 0;
}

// void delay(int miniseconds)
// {
//     // Converting time into milli_seconds
//     int milli_seconds = 1000 * miniseconds;

//     // Storing start time
//     clock_t start_time = clock();

//     // looping till required time is not achieved
//     while (clock() < start_time + milli_seconds)
//         ;
// }

// void set_label_empty_text(GtkWidget *widget)
// {
//     sleep(5);
//     gtk_label_set_text(GTK_LABEL(widget), "");
// }

void label_set_text(RES res, GtkWidget *widget, char *text)
{
    gtk_label_set_text(GTK_LABEL(widget), text);
    if (res == ERROR)
        gtk_widget_modify_fg(GTK_LABEL(widget), GTK_STATE_NORMAL, &red);
    else if (res == SUCCESS)
        gtk_widget_modify_fg(GTK_LABEL(widget), GTK_STATE_NORMAL, &green);
}

void make_protocol(char *k, char *i1, char *i2)
{
    char *pr = (char *)malloc(sizeof(char) * MAX);
    strcpy(pr, k);
    strcat(pr, "|");
    strcat(pr, i1);
    if (i2 != NULL)
    {
        strcat(pr, "|");
        strcat(pr, i2);
    }
    send(sockfd, pr, MAX, 0);
    recv(sockfd, recv_info, MAX, 0);
    char *str = strdup(recv_info);
    strcpy(key, strsep(&str, "|"));
    char *noti_;
    if ((noti_ = strsep(&str, "|")) != NULL)
        strcpy(info1, noti_);
    free(pr);
}

void show_register()
{

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

    window_register = GTK_WIDGET(gtk_builder_get_object(builder, "window_register"));
    gtk_builder_connect_signals(builder, NULL);

    reg_acc = GTK_WIDGET(gtk_builder_get_object(builder, "reg_acc"));
    reg_pass = GTK_WIDGET(gtk_builder_get_object(builder, "reg_pass"));
    retype_pass = GTK_WIDGET(gtk_builder_get_object(builder, "retype_pass"));
    reg_noti = GTK_WIDGET(gtk_builder_get_object(builder, "reg_noti"));

    g_object_unref(builder);
    gtk_widget_show(window_register);
}

void registerr()
{
    char *repass = (char *)malloc(sizeof(char) * MAX);
    strcpy(info1, gtk_entry_get_text(GTK_ENTRY(reg_acc)));
    strcpy(info2, gtk_entry_get_text(GTK_ENTRY(reg_pass)));
    strcpy(repass, gtk_entry_get_text(GTK_ENTRY(retype_pass)));

    if (strcmp(info1, "") == 0 || strcmp(info2, "") == 0 || strcmp(repass, "") == 0)
        label_set_text(ERROR, reg_noti, "Thông tin đăng ký còn thiếu");
    else if (strcmp(info2, repass) != 0)
        label_set_text(ERROR, reg_noti, "Mật khẩu không khớp");
    else
    {
        make_protocol("REG", info1, info2);
        if (strcmp(key, "OKE") == 0)
        {
            gtk_widget_hide(window_register);
            label_set_text(SUCCESS, log_noti, "Đăng ký thành công");
        }
        else if (strcmp(key, "NOKE") == 0)
            label_set_text(ERROR, reg_noti, info1);
    }
    free(repass);
}

void login()
{
    strcpy(info1, gtk_entry_get_text(GTK_ENTRY(log_acc)));
    strcpy(info2, gtk_entry_get_text(GTK_ENTRY(log_pass)));
    if (strcmp(info1, "") == 0 || strcmp(info2, "") == 0)
        label_set_text(ERROR, log_noti, "Thông tin đăng nhập còn thiếu");
    else
    {
        make_protocol("LOG", info1, info2);
        if (strcmp(key, "OKE") == 0)
        {
            show_main_window();
        }
        else if (strcmp(key, "NOKE") == 0)
            label_set_text(ERROR, log_noti, info1);
    }
}

void show_main_window()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

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
                gtk_list_store_append(list, &Iter);
                gtk_list_store_set(list, &Iter, 0, e, -1);
            }
        }
    }
}

void set_mean_textview_text(GtkWidget *textview, char *text)
{
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    if (buffer == NULL)
    {
        printf("Get buffer fail!");
        buffer = gtk_text_buffer_new(NULL);
    }
    gtk_text_buffer_set_text(buffer, text, -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(textview), buffer);
}

void translate()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    char *buffer = (char *)malloc(sizeof(char) * MAX);
    btpos(dict, ZSTART);
    int rsize;
    gchar gettext[MAX];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "");
    }
    else
    {
        if (btsel(dict, gettext, value, MAX, &rsize))
            set_mean_textview_text(textview1, "Không tìm thấy từ bạn cần tìm");
        else
        {
            if (strcmp(buftrans, gettext) != 0)
            {
                strcpy(buftrans, gettext);
                strcpy(buffer, gettext);
                strcat(buffer, "\n");
                strcat(buffer, htr);
                strcpy(htr, buffer);
            }
            set_mean_textview_text(textview1, value);
        }
    }

    set_mean_textview_text(textview_his, htr);

    free(value);
    free(buffer);
}

void clear_history()
{
    strcpy(htr, "");
    set_mean_textview_text(textview_his, htr);
}

void add_to_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    gchar new_word[MAX];
    gchar mean_word[MAX];
    btpos(dict, ZSTART);
    strcpy(new_word, gtk_entry_get_text(GTK_ENTRY(entry_newword)));
    strcpy(mean_word, gtk_entry_get_text(GTK_ENTRY(entry_meanword)));
    if (strcmp(new_word, "") == 0 && strcmp(mean_word, "") == 0)
    {
        gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ mới và nghĩa cần thêm");
    }
    else
    {
        if (strcmp(new_word, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ mới cần thêm");
        }
        else if (strcmp(mean_word, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập nghĩa của từ cần thêm");
        }
        else
        {
            if (!btsel(dict, new_word, value, MAX, &rsize))
            {
                gtk_label_set_text(GTK_LABEL(textview2), "Từ này đã có trong từ điển");
            }
            else
            {
                if (!btins(dict, new_word, mean_word, MAX))
                    gtk_label_set_text(GTK_LABEL(textview2), "Đã thêm thành công");
                else
                    gtk_label_set_text(GTK_LABEL(textview2), "Thêm thất bại, chương trình lỗi...");
            }
        }
    }
    free(value);
}

void repair_word()
{
    gchar new_word[MAX];
    gchar mean_word[MAX];
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    btpos(dict, ZSTART);
    strcpy(new_word, gtk_entry_get_text(GTK_ENTRY(entry_newword)));
    strcpy(mean_word, gtk_entry_get_text(GTK_ENTRY(entry_meanword)));
    if (strcmp(new_word, "") == 0 && strcmp(mean_word, "") == 0)
    {
        gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ và nghĩa cần sửa");
    }
    else
    {
        if (strcmp(new_word, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ cần sửa");
        }
        else if (strcmp(mean_word, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập nghĩa cần thay thế");
        }
        else if (!btsel(dict, new_word, value, MAX, &rsize))
            if (strcmp(value, mean_word) == 0)
                gtk_label_set_text(GTK_LABEL(textview2), "Nghĩa bạn thay thế giống nghĩa của từ điển");
            else
            {
                if (!btupd(dict, new_word, mean_word, MAX))
                    gtk_label_set_text(GTK_LABEL(textview2), "Đã sửa thành công");
                else
                    gtk_label_set_text(GTK_LABEL(textview2), "Sửa thất bại, chương trình lỗi...");
            }
        else
            gtk_label_set_text(GTK_LABEL(textview2), "Từ bạn cần sửa không có trong từ điển, vui lòng dùng chức năng thêm");
    }
    free(value);
}

void delete_from_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    gchar gettext[MAX];
    btpos(dict, ZSTART);
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(entry_del)));
    if (strcmp(gettext, "") == 0)
    {
        gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ cần xóa");
    }
    else
    {
        if (btsel(dict, gettext, value, MAX, &rsize))
            gtk_label_set_text(GTK_LABEL(textview2), "Không tìm thấy từ bạn cần xóa");
        else
        {
            if (!btdel(dict, gettext))
                gtk_label_set_text(GTK_LABEL(textview2), "Đã xóa thành công");
            else
                gtk_label_set_text(GTK_LABEL(textview2), "Xóa thất bại, chương trình lỗi...");
        }
    }
    free(value);
}

void add_to_note()
{
    note = btopn("note.bt", 0, 0);
    char gettext[MAX];
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    btpos(dict, ZSTART);
    btpos(note, ZSTART);

    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "Bạn chưa nhập vào từ cần thêm vào danh sách ghi chú");
    }
    else
    {
        if (!btsel(note, gettext, value, MAX, &rsize))
        {
            set_mean_textview_text(textview1, "Từ này đã có trong danh sách ghi chú");
        }
        else if (btsel(dict, gettext, value, MAX, &rsize))
            set_mean_textview_text(textview1, "Từ bạn nhập không có trong từ điển, không thể thêm...");
        else
        {
            if (!btins(note, gettext, value, MAX))
            {
                set_mean_textview_text(textview1, "Đã thêm thành công");
            }
            else
                set_mean_textview_text(textview1, "Không thể thêm, chương trình lỗi...");
        }
    }
    free(value);
    btcls(note);
}

void delete_from_note()
{
    note = btopn("note.bt", 0, 0);
    char gettext[MAX];
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));

    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "Bạn chưa nhập vào từ cần xóa khỏi danh sách ghi chú");
    }
    else
    {
        if (btsel(note, gettext, value, MAX, &rsize))
            set_mean_textview_text(textview1, "Từ bạn nhập không có trong danh sách ghi chú");
        else
        {
            if (!btdel(note, gettext))
            {
                set_mean_textview_text(textview1, "Đã xóa thành công");
            }
            else
                set_mean_textview_text(textview1, "Không thể xóa, chương trình lỗi...");
        }
    }
    free(value);
    btcls(note);
}

void extend()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../src/dict-app.glade");

    window_advanced = GTK_WIDGET(gtk_builder_get_object(builder, "window_advanced"));
    gtk_builder_connect_signals(builder, NULL);

    entry_newword = GTK_WIDGET(gtk_builder_get_object(builder, "entry_newword"));
    entry_meanword = GTK_WIDGET(gtk_builder_get_object(builder, "entry_meanword"));
    entry_del = GTK_WIDGET(gtk_builder_get_object(builder, "entry_del"));
    textview2 = GTK_WIDGET(gtk_builder_get_object(builder, "textview2"));

    g_object_unref(builder);
    gtk_widget_show(window_advanced);
}

void practice()
{
    note = btopn("note.bt", 0, 0);
    int SIZE_OF_NOTE = 0;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    int rsize, flag = 0;
    btpos(note, ZSTART);

    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../src/dict-app.glade");

    window_note = GTK_WIDGET(gtk_builder_get_object(builder, "window_note"));
    gtk_builder_connect_signals(builder, NULL);

    textview3 = GTK_WIDGET(gtk_builder_get_object(builder, "textview3"));

    g_object_unref(builder);
    gtk_widget_show(window_note);

    while (!btseln(note, eng, vie, MAX, &rsize))
    {
        SIZE_OF_NOTE++;
    }
    char *list = (char *)malloc(sizeof(char) * MAX * SIZE_OF_NOTE);
    if (SIZE_OF_NOTE == 0)
        set_mean_textview_text(textview3, "Danh sách trống");
    else
    {
        btpos(note, ZSTART);
        while (!btseln(note, eng, vie, MAX, &rsize))
        {
            if (!flag)
            {
                strcpy(list, eng);
                strcat(list, "\n");
            }
            else
            {
                strcat(list, eng);
                strcat(list, "\n");
            }
            flag = 1;
        }
        set_mean_textview_text(textview3, list);
    }
    free(list);
    btcls(note);
}

void delete_all_note()
{
    note = btcrt("note.bt", 0, 0);
    set_mean_textview_text(textview3, "Danh sách trống");
    btcls(note);
}

void about()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../src/dict-app.glade");

    window_about = GTK_WIDGET(gtk_builder_get_object(builder, "window_about"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show(window_about);
}