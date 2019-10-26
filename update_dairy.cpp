#include "update_dairy.h"


//将旅客每一段的路径、乘坐的交通工具、车次、时间、费用输出到日志中
void find_path(ofstream &fout, int num) {
    ifstream fin;
    fin.open("transport_list.csv", ios::in);
    if(!fin){
        cout<<"open wrong"<<endl;
    }
    string linestr;
    int i;
    for (i = 1; i <= num; i++) {
        getline(fin, linestr);
    }
    getline(fin, linestr, ',');
    getline(fin, linestr, ',');//读取出发地
    char c[20];
    strcpy_s(c, linestr.c_str());//将string类型转换为char[]类型
    find_city(atoi(c), fout);//将char[]类型转换为int类型，并根据出发地代号找到出发地
    fout << " ";
    getline(fin, linestr, ',');//读取目的地
    memset(c, '\0', sizeof(c));
    strcpy_s(c, linestr.c_str());
    find_city(atoi(c), fout);//根据目的地代号找到目的地
    fout << " ";
    getline(fin, linestr, ',');//读取乘坐的交通工具
    memset(c, '\0', sizeof(c));
    strcpy_s(c, linestr.c_str());
    find_type(atoi(c), fout);//根据交通工具代号找到乘坐的交通工具 分为：飞机、火车、汽车
    fout << " ";
    getline(fin, linestr, ',');//读取车次
    fout << linestr << " ";
    getline(fin, linestr, ',');//读取出发时间
    fout << linestr << "-";
    getline(fin, linestr, ',');//读取到达时间
    fout << linestr << " ";
    getline(fin, linestr);//读取票价
    fout << "￥" << linestr << endl;
    fin.close();
}


//将旅客的出行信息等写入日志中
void outfile_user(user_infor &user) {
    ofstream fout;
    fout.open("Dairy.txt", ios::app);
    get_time(year,month,day,hour,minute);//获取当前时间

    if(minute>=10){
        fout <<"("<<year<<"/"<< month<<"/"<<day<<" "<<hour<<":"<<minute<<")";
    }
    else{
        fout <<"("<<year<<"/"<< month<<"/"<<day<<" "<<hour<<":0"<<minute<<")";
    }
    fout<<endl;
    fout << "旅客" << user.num <<"的出行信息为："<< endl;
    fout << "出发地:";
    find_city(user.city_from, fout);//根据出发地的代号找到出发地的名称
    fout << endl;
    fout << "目的地:";
    find_city(user.city_to, fout);//根据目的地的代号找到目的地的名称
    fout << endl;
    fout << "途径城市:";
    for (int i = 0; i < 2; i++) {//根据途径城市的代号找到途径城市的名称
        if (user.city_pass[i] == 0) {
            break;
        }
        find_city(user.city_pass[i], fout);
        fout << " ";
    }
    fout << endl;
    fout << "旅行策略:";
    find_strategy(user.strategy, fout);//根据旅行策略的代号找到旅行策略
    fout << endl;
    fout << "路线及费用:" << endl;
    for (int i = 1; i <= user.path[0]; i++) {//将旅客的路径输出到日志中
        find_path(fout, user.path[i]);
    }
    fout << "总时间:";
    fout_time2(user.sum_time,fout);//将旅客的总用时输出到日志中
    fout<<endl;
    fout << "总费用:" << "￥" << user.sum_fee << endl << endl;//将旅客要花费的金额输出到日志中
    fout.close();
}

//将旅客的状态记录表输出到日志中
void outfile_state(state_infor &user) {
    ofstream fout;
    fout.open("Dairy.txt", ios::app);

    get_time(year,month,day,hour,minute);//获取当前时间

    if(minute>=10){
        fout <<"("<<year<<"/"<< month<<"/"<<day<<" "<<hour<<":"<<minute<<")";
    }
    else{
        fout <<"("<<year<<"/"<< month<<"/"<<day<<" "<<hour<<":0"<<minute<<")";
    }
    fout<<endl;

    fout << "旅客"<<user.num<<"的状态记录表为:" << endl;
    int i;
    for (i = 0; i < user.place[0] + 1; i++) {
        //如果前后两个时间点相同，即两段路程之间无需等待，前一段路程的到达时间=后一段路程的出发时间，这种情况下只输出一条信息即可
        if (i != user.place[0]) {
            if (user.time_cut[i] == user.time_cut[i + 1]) {
                continue;
            }
        }
        switch (user.state[i]) {//根据旅客的状态决定输出格式
        case -2:{//旅客未出发
            fout << u_i[user.num].tra_month<<"."<<u_i[user.num].first_day<<" ";//输出日期
            fout_time(user.time_cut[i + 1],fout);//第一个时间节点
            fout << " 前 ";
            find_city(user.place[i + 1], fout);//即将出发的城市
            fout << " 未出发" << endl;
        }break;
        case -1:{//旅客在途中停留/等待

            fout << "第" << user.day[i] << "天 ";
            fout << u_i[user.num].tra_month<<"."<<u_i[user.num].first_day+user.day[i]<<" ";//输出日期
            fout_time(user.time_cut[i],fout);
            fout << " - ";
            fout << u_i[user.num].tra_month<<"."<<u_i[user.num].first_day+user.day[i+1]<<" ";
            fout << "第" << user.day[i+1] << "天 ";
            fout_time(user.time_cut[i + 1],fout);
            fout << " 在途径城市 ";
            find_city(user.place[i], fout);
            fout << " 停留" << endl;

        }break;
        case 0:{//旅客到达目的地，旅途结束
            fout << "第" << user.day[i] << "天 ";
            fout << u_i[user.num].tra_month<<"."<<u_i[user.num].first_day+user.day[i]<<" ";
            fout_time(user.time_cut[i],fout);
            fout << " 后 ";
            fout<<"到达目的地：";
            find_city(user.place[i], fout);
            fout << "  旅程结束"<<endl<<endl;
        }break;
        case 1:{//旅客正在乘坐飞机
            fout << "第" << user.day[i] << "天 ";
            fout << u_i[user.num].tra_month<<"."<<u_i[user.num].first_day+user.day[i]<<" ";
            fout_time(user.time_cut[i],fout);
            fout << " - ";
            fout_time(user.time_cut[i + 1],fout);
            fout << " ";
            find_city(user.place[i], fout);
            fout << " -> ";
            find_city(user.place[i + 1], fout);
            fout << "  飞机:";
            fout << user.t_f[i].toStdString() << endl;
        }break;
        case 2:{//旅客正在乘坐火车
            fout << "第" << user.day[i] << "天 ";
            fout << u_i[user.num].tra_month<<"."<<u_i[user.num].first_day+user.day[i]<<" ";
            fout_time(user.time_cut[i],fout);
            fout << " - ";
            fout_time(user.time_cut[i + 1],fout);
            fout << " ";
            find_city(user.place[i], fout);
            fout << " -> ";
            find_city(user.place[i + 1], fout);
            fout << "  火车:";
            fout << user.t_f[i].toStdString() << endl;
        }break;
        case 3:{//旅客正在乘坐汽车
            fout << "第" << user.day[i] << "天 ";
            fout << u_i[user.num].tra_month<<"."<<u_i[user.num].first_day+user.day[i]<<" ";
            fout_time(user.time_cut[i],fout);
            fout << " - ";
            fout_time(user.time_cut[i + 1],fout);
            fout << " ";
            find_city(user.place[i], fout);
            fout << " -> ";
            find_city(user.place[i + 1], fout);
            fout << "  汽车:";
            fout << user.t_f[i].toStdString() << endl;
        }break;
        default:break;
        }
    }
    fout.close();
}

//旅客状态查询记录
void outfile_query(int num) {
    ofstream fout;
    fout.open("Dairy_query.txt", ios::app);
    fout << "旅客"<<num<<"状态查询记录：" << endl;
    get_time(year,month,day,hour,minute);//获取当前时间
    int state_check;
    int i = 1;
    int d_check=day;
    double t_check=hour+(double)(minute)/100;//输出当前时间

    fout << "查询时间：" ;
    if(minute>=10){
        fout << month<<"/"<<day<<" "<<hour<<":"<<minute;
    }
    else{
        fout << month<<"/"<<day<<" "<<hour<<":0"<<minute;
    }
    fout << endl;

    fout << "状态查询结果为：" << endl;

    if(d_check>=u_i[num].first_day&&d_check<=u_i[num].first_day+s_i[num].day[s_i[num].day[0]]){
        //查询当天为旅途中
        while (1){//找到查询当天对应的日期在状态表中的位置
            if((month>u_i[num].tra_month)||((month==u_i[num].tra_month) && (d_check > (s_i[num].day[i]+u_i[num].first_day)) && (i<(s_i[num].place[0]+1)))){
                i++;
            }
            else break;
        }

        while (1){//确定查询时刻对应状态表中的位置
            if((t_check >= s_i[num].time_cut[i+1]) && (d_check>=(s_i[num].day[i+2]+u_i[num].first_day)) && (i<(s_i[num].place[0]+1))){
                i=i+2;//同一天只要大于结束时间则跳到下一路线
            }
            else if((t_check >= s_i[num].time_cut[i+1]) && (d_check<(s_i[num].day[i+2]+u_i[num].first_day)) && (i<(s_i[num].place[0]+1))){
                i=i+2;//与下一条路线隔天则只跳一次
                break;
            }
            else if(t_check >= s_i[num].time_cut[i] && t_check < s_i[num].time_cut[i + 1] && i<(s_i[num].place[0]+1)){
                i=i+1;//在路线途中，后移到下一位置
                break;
            }
            else break;
        }
        state_check = s_i[num].state[i - 1];
    }
    else if(d_check>u_i[num].first_day+s_i[num].day[s_i[num].day[0]]){
        //查询当天旅程已结束
        state_check=0;
    }
    else{
        //旅程尚未开始
        state_check=-2;
    }

    if (state_check > 0) {//状态为在途中
        find_city(s_i[num].place[i - 1], fout);
        fout << " -> ";
        find_city(s_i[num].place[i], fout);
        find_type(state_check, fout);
        fout << " ";
        fout << s_i[num].t_f[i - 1].toStdString() << endl;
    }
    else if (state_check == 0) {
        //状态为达到目的地
        fout << "已到达目的地:";
        find_city(s_i[num].place[i - 1], fout);
    }
    else if(state_check==-2){
        //状态为未出发
         fout<<"旅行尚未开始，出发地：";
         find_city(s_i[num].place[1], fout);
    }
    else {
        //状态为到达途径城市
        fout << "到达途径城市：";
        find_city(s_i[num].place[i - 1], fout);
    }
    fout << endl << endl;
    fout.close();
}
