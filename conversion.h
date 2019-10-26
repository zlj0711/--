#pragma once
#include "common.h"
#include "declare.h"


class conversion
{
public:
    conversion();
};


QString node_to_city(int n);
QString node_to_type(int t);
void find_city(int c, ofstream &fout);
void find_strategy(int s, ofstream &fout);
void find_type(int t, ofstream &fout);
QString out_time(double t);
QString out_time2(double t);
void fout_time(double t, ofstream &fout);
void fout_time2(double t, ofstream &fout);
double time_length(double begin, double end);
double time_sum(double begin, double end);
void search(int num, path_infor &path);
int inc_d(int a,int &m);
QPointF getcityPos(int city);
QPixmap setpointGraph(int state);
double time2second(double time);
