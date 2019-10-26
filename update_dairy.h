#ifndef UPDATE_DAIRY_H
#define UPDATE_DAIRY_H
#include "common.h"
#include "declare.h"
#include "conversion.h"
#include "timer.h"


extern user_infor u_i[MAX_user];
extern state_infor s_i[MAX_user];


void outfile_user(user_infor &user);
void outfile_state(state_infor &user);
void outfile_query(int num);


#endif // UPDATE_DAIRY_H
