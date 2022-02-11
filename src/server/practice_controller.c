#include "server/practice_controller.h"
#include "server/utils.h"

void practice()
{
    note_size = next_question = 0;
    strcpy(start_practice_protocol_str, "OKE");
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    int rsize = 0;
    int correct_answer_position;
    int wrong_answer_id1 = 0, wrong_answer_id2 = 0, wrong_answer_id3 = 0;
    note = make_jrb();
    user_note = btopn(make_note_path(username), 0, 0);
    // scan user note to make note tree
    btpos(user_note, ZSTART);

    while (!btseln(user_note, eng, vie, MAX, &rsize))
    {
        printf("%s-%s\n", eng, vie);
        jrb_insert_int(note, note_size, (Jval){.v = make_word(eng, vie)});
        note_size++;
    }

    if (note_size < 4)
        server_make_protocol("NOKE", "Danh sách từ ghi chú cần có 4 từ trở lên!");
    else
    {
        // insert note size to protocol
        strcat(start_practice_protocol_str, "|");
        strcat(start_practice_protocol_str, convert_int_to_string(note_size));
        send(connfd, start_practice_protocol_str, MAX, 0);
    }

    note_id_arr = (int *)malloc(sizeof(int) * note_size);
    for (int i = 0; i < note_size; i++)
        note_id_arr[i] = i;
    randomize(note_id_arr, note_size);

    free(eng);
    free(vie);
    btcls(user_note);
}
void new_question()
{
    strcpy(practice_protocol_str, "OKE");
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    // int rsize, note_size = 0;
    int correct_answer_position;
    int wrong_answer_id1 = 0, wrong_answer_id2 = 0, wrong_answer_id3 = 0;

    // get word of question
    NewWord *w = (NewWord *)(jrb_find_int(note, note_id_arr[next_question])->val.v);

    // insert correct answer and position to protocol
    protocol_str_cat(w->eng);
    protocol_str_cat(convert_int_to_string(1 + rand() % 4));
    protocol_str_cat(w->vie);

    // generate wrong answer id
    srand(time(NULL));
    wrong_answer_id1 = wrong_answer_id2 = wrong_answer_id3 = note_id_arr[next_question];
    while (wrong_answer_id1 == note_id_arr[next_question])
        wrong_answer_id1 = rand() % note_size;
    while (wrong_answer_id2 == note_id_arr[next_question] || wrong_answer_id2 == wrong_answer_id1)
        wrong_answer_id2 = rand() % note_size;
    while (wrong_answer_id3 == note_id_arr[next_question] || wrong_answer_id3 == wrong_answer_id1 || wrong_answer_id3 == wrong_answer_id2)
        wrong_answer_id3 = rand() % note_size;

    // insert wrong answer to protocol
    wrong_word_str_cat(wrong_answer_id1);
    wrong_word_str_cat(wrong_answer_id2);
    wrong_word_str_cat(wrong_answer_id3);
    send(connfd, practice_protocol_str, MAX, 0);
    next_question++;
    free(eng);
    free(vie);
}
void save_record_result_of_game(long correct_num, long num_of_ques)
{
    FILE *f;
    char end_time[30];
    char *buf = (char *)malloc(sizeof(char) * MAX);

    if ((f = fopen(make_game_his_path(username), "a")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strcpy(end_time, asctime(tm));
    end_time[strlen(end_time) - 1] = '\0';
    sprintf(buf, "%s-%ld-%ld", end_time, correct_num, num_of_ques);
    fprintf(f, "%s\n", buf);
    fclose(f);
}

void exit_game()
{
    free(note_id_arr);
    free_id_word(note);
    save_record_result_of_game(atoi(info1), atoi(info2));
    send(connfd, "OKE", MAX, 0);
}

void get_game_his()
{
    char buffer[MAX];
    FILE *fd;
    int ch;
    int count;
    char len[100];
    char buf[100];
    int index = 0;
    int i;

    if ((fd = fopen(make_game_his_path(username), "r")) == NULL)
    {
        printf("Lỗi không thể mở file với game_history_path.\n");
        return;
    }
    strcpy(buffer, "OKE");
    fseek(fd, 0, SEEK_END);
    while (ftell(fd) > 1)
    {
        fseek(fd, -2, SEEK_CUR);
        if (ftell(fd) <= 2)
            break;
        ch = fgetc(fd);
        count = 0;
        while (ch != '\n')
        {
            len[count++] = ch;
            if (ftell(fd) < 2)
                break;
            fseek(fd, -2, SEEK_CUR);
            ch = fgetc(fd);
        }
        for (i = count - 1; i >= 0 && count > 0; i--)
            buf[index++] = len[i];
        buf[index] = '\0';
        strcat(buffer, "|");
        strcat(buffer, buf);
        index = 0;
        memset(buf, '\0', 100);
    }

    fclose(fd);

    send(connfd, buffer, MAX, 0);
}

void del_game_his()
{
    int i;
    if ((i = fclose(fopen(make_game_his_path(info1), "w"))) != 0)
        server_make_protocol("NOKE", NULL);
    else
        server_make_protocol("OKE", NULL);
}

void add_game_word_to_note(){
    
}

void delete_game_word_to_note(){
    
}