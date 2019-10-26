#include "mainwindow.h"
#include <QApplication>


user_infor u_i[MAX_user];
state_infor s_i[MAX_user];

//当前旅客编号
int counter=1;
QTime time_counter;
int year,month,day,hour,minute;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDateTime date;

    QTime current_time =QTime::currentTime();

    year=date.currentDateTime().date().year();
    month=date.currentDateTime().date().month();
    day=date.currentDateTime().date().day();
    hour=current_time.hour();//当前的小时
    minute=current_time.minute();//当前的分
    time_counter.start();

    //支持中文编码
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    MainWindow w;
    w.show();

    return a.exec();
}
