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

    g_signal_connect(window_login, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_object_unref(builder);
    gtk_widget_show(window_login);
    gtk_main();

    return 0;
}

void show_message(GtkWidget *parent, GtkMessageType type, char *mms, char *content)
{
    GtkWidget *mdialog;
    mdialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     type,
                                     GTK_BUTTONS_OK,
                                     "%s", mms);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(mdialog), "%s", content);
    if (type == GTK_MESSAGE_INFO)
        gtk_widget_modify_fg(GTK_MESSAGE_DIALOG(mdialog), GTK_STATE_NORMAL, &green);
    else
        gtk_widget_modify_fg(GTK_MESSAGE_DIALOG(mdialog), GTK_STATE_NORMAL, &red);
    gtk_dialog_run(GTK_DIALOG(mdialog));
    gtk_widget_destroy(mdialog);
}

// void label_set_text(GtkWidget *widget, char *text)
// {
//     gtk_label_set_text(GTK_LABEL(widget), text);
//     if (res == ERROR)
//         gtk_widget_modify_fg(GTK_LABEL(widget), GTK_STATE_NORMAL, &red);
//     else if (res == SUCCESS)
//         gtk_widget_modify_fg(GTK_LABEL(widget), GTK_STATE_NORMAL, &green);
// }

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
    printf("String received from server: ");
    puts(recv_info);
    char *str = strdup(recv_info);
    strcpy(key, strsep(&str, "|"));
    char *noti_;
    if ((noti_ = strsep(&str, "|")) != NULL)
        strcpy(info1, noti_);
    if ((noti_ = strsep(&str, "|")) != NULL)
        strcpy(info2, noti_);
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
        show_message(window_register, GTK_MESSAGE_ERROR, "ERROR!", "Thông tin đăng ký còn thiếu");
    else if (strcmp(info2, repass) != 0)
        show_message(window_register, GTK_MESSAGE_ERROR, "ERROR!", "Mật khẩu không khớp");
    else
    {
        make_protocol("REG", info1, info2);
        if (strcmp(key, "OKE") == 0)
        {
            gtk_widget_hide(window_register);
            show_message(window_login, GTK_MESSAGE_INFO, "SUCCESS!", "Đăng ký thành công");
            gtk_entry_set_text(GTK_ENTRY(log_acc), info1);
        }
        else if (strcmp(key, "NOKE") == 0)
            show_message(window_register, GTK_MESSAGE_ERROR, "ERROR!", info1);
    }
    free(repass);
}

void login()
{
    strcpy(info1, gtk_entry_get_text(GTK_ENTRY(log_acc)));
    strcpy(info2, gtk_entry_get_text(GTK_ENTRY(log_pass)));
    if (strcmp(info1, "") == 0 || strcmp(info2, "") == 0)
        show_message(window_login, GTK_MESSAGE_ERROR, "ERROR!", "Thông tin đăng nhập còn thiếu");
    else
    {
        make_protocol("LOG", info1, info2);
        if (strcmp(key, "OKE") == 0)
        {
            strcpy(user, info1);
            show_main_window();
        }
        else if (strcmp(key, "NOKE") == 0)
            show_message(window_login, GTK_MESSAGE_ERROR, "ERROR!", info1);
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
                gtk_list_store_append(list, &suggestion_iter);
                gtk_list_store_set(list, &suggestion_iter, 0, e, -1);
            }
        }
    }
    free(pr);
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
        }
    }
    free(edited_mean);
    free(origin_mean);
}

void clear_history()
{
    strcpy(htr, "");
    set_mean_textview_text(textview_his, htr);
}

void extend()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

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
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

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

void practice()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

    window_practice = GTK_WIDGET(gtk_builder_get_object(builder, "window_practice"));

    gtk_builder_connect_signals(builder, NULL);
    textview3 = GTK_WIDGET(gtk_builder_get_object(builder, "textview3"));
    g_object_unref(builder);
    gtk_widget_show(window_practice);
}

char *convert_int_to_string(int i)
{
    char *s = (char *)malloc(sizeof(char) * 100);
    sprintf(s, "%d", i);
    return s;
}

void set_answers(char *vie1, char *vie2, char *vie3, char *vie4)
{
    gtk_button_set_label(btn_vie1, vie1);
    gtk_button_set_label(btn_vie2, vie2);
    gtk_button_set_label(btn_vie3, vie3);
    gtk_button_set_label(btn_vie4, vie4);
}

char *spliting_str;
int count_question = 0;
int correct_position = 0;
int choose_position = 0;

char *split_question_str(char *question_str)
{

    char *q = strdup(question_str);
    char *e;
    char *vie1 = (char *)malloc(sizeof(char) * 100);
    char *vie2 = (char *)malloc(sizeof(char) * 100);
    char *vie3 = (char *)malloc(sizeof(char) * 100);
    char *vie4 = (char *)malloc(sizeof(char) * 100);

    count_question++;
    gtk_label_set_text(lbl_count_question, convert_int_to_string(count_question));

    if (e = strsep(&q, "|"))
        gtk_label_set_text(lbl_eng, e);

    if (e = strsep(&q, "|"))
        correct_position = atoi(e);

    if (e = strsep(&q, "|"))
        strcpy(vie1, e);
    if (e = strsep(&q, "|"))
        strcpy(vie2, e);
    if (e = strsep(&q, "|"))
        strcpy(vie3, e);
    if (e = strsep(&q, "|"))
        strcpy(vie4, e);

    switch (correct_position)
    {
    case 1:
        set_answers(vie1, vie2, vie3, vie4);
        break;
    case 2:
        set_answers(vie2, vie1, vie3, vie4);
        break;
    case 3:
        set_answers(vie2, vie3, vie1, vie4);
        break;
    case 4:
        set_answers(vie2, vie3, vie4, vie1);
        break;
    default:
        break;
    }

    free(vie1);
    free(vie2);
    free(vie3);
    free(vie4);

    return q;
}

void start ()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

    window_game = GTK_WIDGET(gtk_builder_get_object(builder, "window_game"));

    lbl_count_question = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_count_question"));
    lbl_total_question = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_total_question"));
    lbl_count_correct = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_count_correct"));

    lbl_eng = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_eng"));

    btn_vie1 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_vie1"));
    btn_vie2 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_vie2"));
    btn_vie3 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_vie3"));
    btn_vie4 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_vie4"));

    send(sockfd, "PRAC", MAXLINE, 0);
    recv(sockfd, recv_question, MAXLINE, 0);
    puts(recv_question);

    char *str = strdup(recv_question);
    strcpy(key, strsep(&str, "|"));
    if (strcmp(key, "NOKE") == 0)
    {
        show_message(window_game, GTK_MESSAGE_ERROR, "ERROR!", info1);
        gtk_widget_destroy(window_practice);
        return;
    }

    char *total;
    if ((total = strsep(&str, "|")) != NULL)
        gtk_label_set_text(lbl_total_question, total);

    count_question = 0;
    spliting_str = split_question_str(str);

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window_game);
    gtk_widget_destroy(window_practice);
}

void next()
{
    spliting_str = split_question_str(spliting_str);
    if (spliting_str == NULL)
    {
        printf("%s\n", "END");
    }
}

// void set_disable_button()
// {
//     gtk_widget_set_sensitive(btn_vie1, false);
//     gtk_widget_set_sensitive(btn_vie2, false);
//     gtk_widget_set_sensitive(btn_vie3, false);
//     gtk_widget_set_sensitive(btn_vie4, false);
// }

// void set_color_for_correct_answer(int p1, int p2, int p3, GtkWidget *widget1, GtkWidget *widget2, GtkWidget *widget3)
// {
//     if(correct_position == p1)
//             gtk_widget_modify_fg(widget1, GTK_STATE_NORMAL, &green);
//         else if(correct_position == p2)
//             gtk_widget_modify_fg(widget2, GTK_STATE_NORMAL, &green);
//         else if(correct_position == p3)
//             gtk_widget_modify_fg(widget3, GTK_STATE_NORMAL, &green);
// }

// void choose_1()
// {
    // choose_position = 1;
    // if(choose_position == correct_position)
    //     gtk_widget_modify_fg(btn_vie1, GTK_STATE_NORMAL, &green);
    // else {
    //     gtk_widget_modify_fg(btn_vie1, GTK_STATE_NORMAL, &red);
    //     // set_color_for_correct_answer(2, 3, 4, btn_vie2, btn_vie3, btn_vie4);
    // }
    // set_disable_button();
    // printf("%s\n", "1");
// }

// void choose2(){
//     choose_position = 2;
//     if(choose_position == correct_position)
//         gtk_widget_modify_fg(btn_vie2, GTK_STATE_NORMAL, &green);
//     else {
//         gtk_widget_modify_fg(btn_vie2, GTK_STATE_NORMAL, &red);
//         set_color_for_correct_answer(1, 3, 4, btn_vie1, btn_vie3, btn_vie4);
//     }
//     // set_disable_button();
// }

// void choose3(){
//     choose_position = 3;
//     if(choose_position == correct_position)
//         gtk_widget_modify_fg(btn_vie3, GTK_STATE_NORMAL, &green);
//     else {
//         gtk_widget_modify_fg(btn_vie3, GTK_STATE_NORMAL, &red);
//         set_color_for_correct_answer(1, 2, 4, btn_vie1, btn_vie2, btn_vie4);
//     }
//     // set_disable_button();
// }

// void choose4(){
//     choose_position = 4;
//     if(choose_position == correct_position)
//         gtk_widget_modify_fg(btn_vie4, GTK_STATE_NORMAL, &green);
//     else {
//         gtk_widget_modify_fg(btn_vie4, GTK_STATE_NORMAL, &red);
//         set_color_for_correct_answer(1, 2, 3, btn_vie1, btn_vie2, btn_vie3);
//     }
//     // set_disable_button();
// }

void exit_game()
{
    gtk_widget_destroy(window_game);
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