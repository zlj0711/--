#include "conversion.h"


//序号->城市
QString node_to_city(int n){
    QString city;
    switch (n) {
    case 0:
        city="None";
        break;
    case 1:
        city="北京";
        break;
    case 2:
        city="天津";
        break;
    case 3:
        city="上海";
        break;
    case 4:
        city="重庆";
        break;
    case 5:
        city="哈尔滨";
        break;
    case 6:
        city="沈阳";
        break;
    case 7:
        city="石家庄";
        break;
    case 8:
        city="西安";
        break;
    case 9:
        city="济南";
        break;
    case 10:
        city="广州";
        break;
    case 11:
        city="长沙";
        break;
    case 12:
        city="南京";
        break;
    case 13:
        city="武汉";
        break;
    case 14:
        city="成都";
        break;
    case 15:
        city="深圳";
        break;
    default:
        break;
    }
    return city;
}


//序号->交通工具
QString node_to_type(int t) {
    QString type;
    switch (t) {
    case 1:
        type="飞机";
        break;
    case 2:
        type="火车";
        break;
    case 3:
        type="汽车";
        break;
    default:
        break;
    }
    return type;
}


//编号对应城市->输出到文件
void find_city(int c, ofstream &fout) {
    switch (c) {
    case 1:fout << "北京"; break;
    case 2:fout << "天津"; break;
    case 3:fout << "上海"; break;
    case 4:fout << "重庆"; break;
    case 5:fout << "哈尔滨"; break;
    case 6:fout << "沈阳"; break;
    case 7:fout << "石家庄"; break;
    case 8:fout << "西安"; break;
    case 9:fout << "济南"; break;
    case 10:fout << "广州"; break;
    case 11:fout << "长沙"; break;
    case 12:fout << "南京"; break;
    case 13:fout << "武汉"; break;
    case 14:fout << "成都"; break;
    case 15:fout << "深圳"; break;
    default:break;
    }
}


//编号对应旅行策略->输出到文件
void find_strategy(int s, ofstream &fout) {
    switch (s) {
    case 1:fout << "最少费用" << endl; break;
    case 2:fout << "最短时间" << endl; break;
    case 3:fout << "限时最少费用" << endl; break;
    default:break;
    }
}


//编号对应出行方式->输出到文件
void find_type(int t, ofstream &fout) {
    switch (t) {
    case 1:fout << "飞机"; break;
    case 2:fout << "火车"; break;
    case 3:fout << "汽车"; break;
    default:break;
    }
}


//出发、到站时间显示
QString out_time(double t) {
    QString temp,text;
    temp=QString::number(t);
    QStringList v;
    v=temp.split(".");

    if(v.size()==1)
        text=v[0]+":00";
    else if(v[1].size()==1)
        text=v[0]+":"+v[1]+"0";
    else
        text=v[0]+":"+v[1];
    return text;
}


//行程总时间显示
QString out_time2(double t) {
    QString temp,text;
    temp=QString::number(t);
    QStringList v;
    v=temp.split(".");

    if(v.size()==1)
        text=v[0]+"小时";
    else if(v[1].size()==1)
        text=v[0]+"小时"+v[1]+"0分钟";
    else
        text=v[0]+"小时"+v[1]+"分钟";
    return text;
}


//出发、到站时间输出到文件
void fout_time(double t, ofstream &fout) {
    QString temp,text;
    temp=QString::number(t);
    QStringList v;
    v=temp.split(".");

    if(v.size()==1)
        text=v[0]+":00";
    else if(v[1].size()==1)
        text=v[0]+":"+v[1]+"0";
    else
        text=v[0]+":"+v[1];
    fout<<text.toStdString();
}


//行程总时间输出到文件
void fout_time2(double t, ofstream &fout) {
    QString temp,text;
    temp=QString::number(t);
    QStringList v;
    v=temp.split(".");

    if(v.size()==1)
        text=v[0]+":00";
    else if(v[1].size()==1)
        text=v[0]+"小时"+v[1]+"0分钟";
    else
        text=v[0]+"小时"+v[1]+"分钟";
    fout<<text.toStdString();
}


//把序号对应的路线存入path
void search(int num, path_infor &path){
    QFile file("transport_list.csv");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"文件打开失败";
    }
    else {
        for(int count=1;count<=num;count++) {
            file.readLine();
        }
        file.seek(file.pos());
        QString text=file.readLine();
        QStringList data=text.split(",");

        //存入path
        path.node=data[0].toInt();//序号
        path.city_start=data[1].toInt();//出发地
        path.city_end=data[2].toInt();//目的地
        path.travel_type=data[3].toInt();//交通工具类型
        path.t_f_num=data[4];//车次

        QStringList time;
        QString data_time;

        time=data[5].split(":");
        data_time=time[0]+"."+time[1];
        path.time_begin=data_time.toDouble();//出发时间
        time=data[6].split(":");
        data_time=time[0]+"."+time[1];
        path.time_end=data_time.toDouble();//到站时间

        path.fee=data[7].toInt();//费用
    }
    file.close();
}


//计算时间长度
double time_length(double begin, double end){
    QString result, temp1, temp2;
    //begin
    temp1 = QString::number(begin);
    QStringList vbegin;
    vbegin=temp1.split(".");
    if(vbegin.size()!=1&&vbegin[1].size()==1){
        vbegin[1]+="0";
    }
    //end
    temp2 = QString::number(end);
    QStringList vend;
    vend=temp2.split(".");
    if(vend.size()!=1&&vend[1].size()==1){
        vend[1]+="0";
    }
    //转化成int
    int h,h1,h2,m,m1,m2,hm;
    h1=vbegin[0].toInt();
    h2=vend[0].toInt();
    if(vbegin.size()==1) m1=0;
    else m1=vbegin[1].toInt();
    if(vend.size()==1) m2=0;
    else m2=vend[1].toInt();
    //计算
    if(m2<m1){
        h2-=1;
        m2+=60;
    }
    h=h2-h1;
    m=m2-m1;
    hm = h * 100 + m;
    return (double)hm / 100;
}


//计算时间总和
double time_sum(double begin, double end){
    QString result, temp1, temp2;
    //begin
    temp1 = QString::number(begin);
    QStringList vbegin;
    vbegin=temp1.split(".");
    if(vbegin.size()!=1&&vbegin[1].size()==1){
        vbegin[1]+="0";
    }
    //end
    temp2 = QString::number(end);
    QStringList vend;
    vend=temp2.split(".");
    if(vend.size()!=1&&vend[1].size()==1){
        vend[1]+="0";
    }
    //转化成int
    int h,h1,h2,m,m1,m2,hm;
    h1=vbegin[0].toInt();
    h2=vend[0].toInt();
    if(vbegin.size()==1) m1=0;
    else m1=vbegin[1].toInt();
    if(vend.size()==1) m2=0;
    else m2=vend[1].toInt();
    //计算
    h=h1+h2;
    m=m1+m2;
    if(m>60){
        h+=m/60;
        m=m%60;
    }
    hm = h * 100 + m;
    return (double)hm / 100;
}


//将时间转化成秒 1h->10s
double time2second(double time){
    QString temp=QString::number(time);
    QStringList temp_time=temp.split(".");
    if(temp_time.size()==1){
        return 10*temp_time[0].toInt();
    }
    else if(temp_time[1].size()==1){
        double tmp_min=(temp_time[1]+"0").toDouble();
        return 10*temp_time[0].toInt()+tmp_min/6;
    }
    else if(temp_time[1].size()==2){
        return 10*temp_time[0].toInt()+temp_time[1].toDouble()/6;
    }
}


//天数+1
int inc_d(int a,int &m){
    a=day+1;
    m=month;
    switch(month){
    case 1:case 3:case 5:case 7:case 8:case 10:case 12:{
        if(a>31){
            m=month+1;
            a=(a+31)%31;
        }
    }break;
    case 2:{
        //闰年
        if(year/4==0&&year/100!=0){
            if(a>29){
                m=month+1;
                a=(a+29)%29;
            }
        }
        else{
            if(a>28){
                m=month+1;
                a=(a+28)%28;
            }
        }
    }break;
    case 4:case 6:case 9:case 11:{
        if(a>30){
            m=month+1;
            a=(a+30)%30;
        }
    }break;
    default:break;
    }
    return a;
}


//城市->坐标
QPointF getcityPos(int city)
{
    int x, y;
    switch (city){
    case 0:
        x=16;
        y=16;
        break;
    case 1:
        x = 778-38;
        y = 356-38;
        break;
    case 2:
        x = 799-38;
        y = 375-38;
        break;
    case 3:
        x = 887-38;
        y = 550-38;
        break;
    case 4:
        x = 600-38;
        y = 601-38;
        break;
    case 5:
        x = 927-38;
        y = 206-38;
        break;
    case 6:
        x = 894-38;
        y = 304-38;
        break;
    case 7:
        x = 756-38;
        y = 399-38;
        break;
    case 8:
        x = 651-38;
        y = 486-38;
        break;
    case 9:
        x = 788-38;
        y = 443-38;
        break;
    case 10:
        x = 736-38;
        y = 742-38;
        break;
    case 11:
        x = 725-38;
        y = 632-38;
        break;
    case 12:
        x = 840-38;
        y = 535-38;
        break;
    case 13:
        x = 749-38;
        y = 566-38;
        break;
    case 14:
        x = 540-38;
        y = 599-38;
        break;
    case 15:
        x = 760-38;
        y = 753-38;
        break;
    }
    return QPointF(x, y);
}


//状态->图标
QPixmap setpointGraph(int state)
{
    QPixmap pointGraph;
    switch(state)
    {
    case -1:case -2://停留或未出发
        pointGraph = QPixmap(":/wait.ico");
        break;
    case 0://到达
        pointGraph = QPixmap(":/ok.ico");
        break;
    case 1://飞机
        pointGraph = QPixmap(":/plane.ico");
        break;
    case 2://火车
        pointGraph = QPixmap(":/train.ico");
        break;
    case 3://汽车
        pointGraph = QPixmap(":/bus.ico");
        break;
    }
    return pointGraph;
}
