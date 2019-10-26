#include "timer.h"


timer::timer(){}


//获取时间并通过引用返回
void get_time(int &year,int &month,int &day,int &hour,int &minute)
{
    //利用elapsed()函数获取start()、restart()的时间间隔毫秒数
    int t_used_ms=time_counter.elapsed();
    //转换为秒
    int t_used=t_used_ms/1000;

    //以10s模拟一小时，小时增加，分钟比例增加
    hour=hour+t_used/10;
    minute=minute+(t_used%10)*6;

    //分钟进位
    if(minute>=60){
        hour=hour+minute/60;
        minute=(minute+60)%60;
    }

    //小时进位
    if(hour>=24){
        day=day+hour/24;
        hour=(hour+24)%24;
    }

    //月份进位
    switch(month){
    case 1:case 3:case 5:case 7:case 8:case 10:case 12:{
        //31天进位
        if(day>31){
            month=month+day/31;
            day=(day+31)%31;
        }
    }break;
    case 2:{
        //2月单独讨论
        if(year/4==0&&year/100!=0){
            //闰年29天
            if(day>29){
                month=month+day/29;
                day=(day+29)%29;
            }
        }
        else{
            //平年28天
            if(day>28){
                month=month+day/28;
                day=(day+28)%28;
            }
        }
    }break;
    case 4:case 6:case 9:case 11:{
        //30天进位
        if(day>30){
            month=month+day/30;
            day=(day+30)%30;
        }
    }break;
    default:break;
    }
    //年进位
    if(month>12){
        year=year+month/12;
        month=(month+12)%12;
    }

    //重启计时器
    time_counter.restart();
    return;
}
