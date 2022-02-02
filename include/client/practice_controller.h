#ifndef _PRACTICECONTROLLER_H_
#define _PRACTICECONTROLLER_H_

#include "client/client.h"
#include "client/utils.h"

void practice();
void set_answers(char *vie1, char *vie2, char *vie3, char *vie4);
void set_lbl_info_game();
void create_question(char *question_str);
void new_record_result_of_game();
void start();
void new_question();
void set_disable_button(bool b);
void reset_color_of_button();
void set_color_for_correct_answer();
void choose_1();
void choose_2();
void choose_3();
void choose_4();
void exit_game();
void show_game_his();
void del_game_his();

#endif
