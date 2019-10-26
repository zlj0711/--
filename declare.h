#ifndef DECLARE_H
#define DECLARE_H

#include "common.h"

#define MAX_user 101
#define MAX_path 201
#define MAX_state 401

const int change_num=0;


class path_infor {
private:

public:
    int node;//编号
    int city_start, city_end;//起发地、目的地
    int travel_type;//类型
    double time_begin, time_end;//出发时间、到达时间    
    QString t_f_num;//车次号、航班号
    int fee;//费用
    path_infor() {}
    ~path_infor() {}
};


class user_infor {
private:

public:
    int num;//旅客编号
    int city_from, city_to, city_pass[2] = { 0 };//出发地、目的地、途径城市
    int strategy;//旅行策略
    double limited_time;//限时
    int path[MAX_path];//路线,path[0]存放使用的列车总数
    double sum_time;
    int sum_fee;//总时间、总费用
    int first_day=0;//旅行第一天
    int tra_month;//旅行起始月
    user_infor() {}
    ~user_infor() {}
};


class state_infor {
private:

public:
    int num;//旅客编号
    double time_cut[MAX_state];//时间节点
    int day[MAX_state];//天数
    int place[MAX_state], state[MAX_state];//对应地点、对应状态
    QString t_f[MAX_state];//对应航班车次信息
    state_infor() {}
    ~state_infor() {}
};


extern user_infor u_i[MAX_user];
extern state_infor s_i[MAX_user];
extern QTime time_counter;
extern int year,month,day,hour,minute;
extern int counter;


#endif // DECLARE_H
