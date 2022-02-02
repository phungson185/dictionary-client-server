#ifndef _PRACTICECONTROLLER_H_
#define _PRACTICECONTROLLER_H_

#include "server/server.h"
#include "server/utils.h"

void practice();
void new_question();
void save_record_result_of_game(long correct_num, long num_of_ques);
void exit_game();
void get_game_his();
void del_game_his();

#endif
