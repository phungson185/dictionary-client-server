#include "client/practice_controller.h"

void practice()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../../../ui/dict-app.glade");

    window_practice = GTK_WIDGET(gtk_builder_get_object(builder, "window_practice"));

    gtk_builder_connect_signals(builder, NULL);
    textview3 = GTK_WIDGET(gtk_builder_get_object(builder, "textview3"));
    g_object_unref(builder);
    show_game_his();
    gtk_widget_show(window_practice);
}

void set_answers(char *vie1, char *vie2, char *vie3, char *vie4)
{
    gtk_label_set_text(lbl_vie1, vie1);
    gtk_label_set_text(lbl_vie2, vie2);
    gtk_label_set_text(lbl_vie3, vie3);
    gtk_label_set_text(lbl_vie4, vie4);
}

void set_lbl_info_game()
{
    char *str = (char *)malloc(sizeof(char) * 10);
    sprintf(str, "%d", game_result.total);
    gtk_label_set_text(lbl_count_question, str);

    sprintf(str, "%d", game_result.correct_num);
    gtk_label_set_text(lbl_count_correct, str);
}

void create_question(char *question_str)
{
    set_lbl_info_game();
    char *q = strdup(question_str);
    char *e;
    char *vie1 = (char *)malloc(sizeof(char) * MAX);
    char *vie2 = (char *)malloc(sizeof(char) * MAX);
    char *vie3 = (char *)malloc(sizeof(char) * MAX);
    char *vie4 = (char *)malloc(sizeof(char) * MAX);

    if (e = strsep(&q, "|"))
        gtk_label_set_text(GTK_LABEL(lbl_eng), e);

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
}

void new_record_result_of_game()
{
    game_result.total = 0;
    game_result.correct_num = 0;
}

void start()
{
    send(sockfd, "PRAC", MAX, 0);
    recv(sockfd, recv_start, MAX, 0);
    puts(recv_start);

    char *str = strdup(recv_start);
    strcpy(key, strsep(&str, "|"));
    if (strcmp(key, "NOKE") == 0)
    {
        show_message(window_game, GTK_MESSAGE_ERROR, "ERROR!", strsep(&str, "|"));
        gtk_widget_destroy(window_practice);
        return;
    }

    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../../../ui/dict-app.glade");

    window_game = GTK_WIDGET(gtk_builder_get_object(builder, "window_game"));

    lbl_count_question = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_count_question"));
    lbl_total_question = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_total_question"));
    lbl_count_correct = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_count_correct"));

    lbl_eng = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_eng"));

    lbl_vie1 = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_vie1"));
    lbl_vie2 = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_vie2"));
    lbl_vie3 = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_vie3"));
    lbl_vie4 = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_vie4"));

    btn_vie1 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_vie1"));
    btn_vie2 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_vie2"));
    btn_vie3 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_vie3"));
    btn_vie4 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_vie4"));

    new_record_result_of_game();
    char *total;
    if ((total = strsep(&str, "|")) != NULL)
    {
        gtk_label_set_text(GTK_LABEL(lbl_total_question), total);
        game_size = atoi(total);
    }

    new_question();

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window_game);
    gtk_widget_destroy(window_practice);
}

void new_question()
{
    reset_color_of_button();

    if (game_result.total == game_size)
    {
        exit_game();
        return;
    }

    send(sockfd, "NEWQ", MAX, 0);
    recv(sockfd, recv_info, MAX, 0);
    puts(recv_info);
    char *str = strdup(recv_info);
    strcpy(key, strsep(&str, "|"));
    if (strcmp(key, "NOKE") == 0)
    {
        show_message(window_game, GTK_MESSAGE_ERROR, "ERROR!", strsep(&str, "|"));
        gtk_widget_destroy(window_practice);
        return;
    }
    game_result.total++;

    create_question(strdup(str));
}

void set_disable_button(bool b)
{
    gtk_widget_set_sensitive(btn_vie1, !b);
    gtk_widget_set_sensitive(btn_vie2, !b);
    gtk_widget_set_sensitive(btn_vie3, !b);
    gtk_widget_set_sensitive(btn_vie4, !b);
}
void reset_color_of_button()
{
    set_disable_button(FALSE);
    gtk_widget_modify_fg(lbl_vie1, GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_fg(lbl_vie2, GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_fg(lbl_vie3, GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_fg(lbl_vie4, GTK_STATE_NORMAL, NULL);
}
void set_color_for_correct_answer()
{
    set_lbl_info_game();
    if (correct_position == 1)
        gtk_widget_modify_fg(lbl_vie1, GTK_STATE_NORMAL, &green);
    else if (correct_position == 2)
        gtk_widget_modify_fg(lbl_vie2, GTK_STATE_NORMAL, &green);
    else if (correct_position == 3)
        gtk_widget_modify_fg(lbl_vie3, GTK_STATE_NORMAL, &green);
    else if (correct_position == 4)
        gtk_widget_modify_fg(lbl_vie4, GTK_STATE_NORMAL, &green);
}

void choose_1()
{
    choose_position = 1;
    if (choose_position != correct_position)
        gtk_widget_modify_fg(lbl_vie1, GTK_STATE_NORMAL, &red);
    else
        game_result.correct_num++;
    set_color_for_correct_answer();
    set_disable_button(TRUE);
}

void choose_2()
{
    choose_position = 2;
    if (choose_position != correct_position)
        gtk_widget_modify_fg(lbl_vie2, GTK_STATE_NORMAL, &red);
    else
        game_result.correct_num++;
    set_color_for_correct_answer();
    set_disable_button(TRUE);
}

void choose_3()
{
    choose_position = 3;
    if (choose_position != correct_position)
        gtk_widget_modify_fg(lbl_vie3, GTK_STATE_NORMAL, &red);
    else
        game_result.correct_num++;
    set_color_for_correct_answer();
    set_disable_button(TRUE);
}

void choose_4()
{
    choose_position = 4;
    if (choose_position != correct_position)
        gtk_widget_modify_fg(lbl_vie4, GTK_STATE_NORMAL, &red);
    else
        game_result.correct_num++;
    set_color_for_correct_answer();
    set_disable_button(TRUE);
}

void exit_game()
{
    gtk_widget_destroy(window_game);
    char *end_info = (char *)malloc(sizeof(char) * 100);
    strcpy(end_info, "Bạn đã hoàn thành bài thi!\n");
    strcat(end_info, "Số câu đúng: ");
    strcat(end_info, make_long_to_string(game_result.correct_num));
    strcat(end_info, "/");
    strcat(end_info, make_long_to_string(game_result.total));
    show_message(window_main, GTK_MESSAGE_INFO, "KẾT THÚC", end_info);
    free(end_info);
    make_protocol("EXIT", make_long_to_string(game_result.correct_num), make_long_to_string(game_result.total));
    printf("String received from server: ");
    puts(recv_info);
}

void show_game_his()
{
    GtkTextBuffer *buffer;
    GtkTextIter iter;

    buffer = clear_textview(textview3);
    gtk_text_buffer_create_tag(buffer, "red_fg", "foreground", "#ff5c5c", NULL);
    gtk_text_buffer_create_tag(buffer, "yellow_fg", "foreground", "#f8bb32", NULL);
    gtk_text_buffer_create_tag(buffer, "green_fg", "foreground", "#40ad87", NULL);
    gtk_text_buffer_create_tag(buffer, "greeen_fg", "foreground", "blue", NULL);
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);

    char *get_end_time;
    char *get_correct_num;
    char *get_game_tree_size;
    char line[100];
    char buf[50];

    send(sockfd, "GGHIS", MAX, 0);
    recv(sockfd, recv_info, MAX, 0);
    char *str = strdup(recv_info);
    char* e = strsep(&str, "|");

    while ((e = strsep(&str, "|")) != NULL)
    {
        strcpy(buf,e);
        get_end_time = strtok(&buf, "-");
        get_correct_num = strtok(NULL, "-");
        get_game_tree_size = strtok(NULL, "-");
        sprintf(line, "%-30s%-10s%-5s\n", get_end_time, get_correct_num, get_game_tree_size);
        float lv = (atof(get_correct_num) / atof(get_game_tree_size)) * 10;
        if (lv >= 0 && lv < 5)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, line, -1, "red_fg", NULL);
        else if (lv >= 5 && lv < 8)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, line, -1, "yellow_fg", NULL);
        else
            gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, line, -1, "green_fg", NULL);
        puts(e);
    }
}

void del_game_his()
{
    make_protocol("DGHIS", NULL, NULL);
    if (strcmp(key, "OKE") == 0)
        set_mean_textview_text(textview3, "Danh sách trống!");
    else
        show_message(window_practice, GTK_MESSAGE_ERROR, "ERROR", "Xóa lịch sử luyện tập thất bại");
}