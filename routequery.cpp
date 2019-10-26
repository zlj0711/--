#include "routequery.h"
#include "ui_routequery.h"


routequery::routequery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::routequery)
{
    ui->setupUi(this);
    setFixedSize(500,700);
}

routequery::~routequery()
{
    delete ui;
}


//判断输入是否合法
int judge_input(user_infor user){
    //出发地 目的地 策略未选
    if (user.city_from == 0 || user.city_to == 0||user.strategy==0)
        return 0;
    //出发地与目的地相同
    else if (user.city_from == user.city_to)
        return 0;
    //途径城市有且相同
    else if (user.city_pass[0] == user.city_pass[1] && user.city_pass[0] != 0)
        return 0;
    //途径城市与出发地目的地相同
    else if (user.city_pass[0] == user.city_from || user.city_pass[0] == user.city_to ||user.city_pass[1] == user.city_from || user.city_pass[1] == user.city_to)
        return 0;
    //合法
    else return 1;
}


//pass2是否可选
void routequery::on_pass1_currentTextChanged(const QString &arg1)
{
    if(ui->pass1->currentIndex()!=0)
        ui->pass2->setEnabled(true);
    else
        ui->pass2->setEnabled(false);
}


//限时是否可选
void routequery::on_strategy_currentTextChanged(const QString &arg1)
{
    if(ui->strategy->currentIndex()==3)
        ui->time->setEnabled(true);
    else
        ui->time->setEnabled(false);
}


//查询
void routequery::on_search_clicked()
{
    //清空数据
    u_i[counter].city_from=0;
    u_i[counter].city_to=0;
    memset(u_i[counter].city_pass,'\0',sizeof(u_i[counter].city_pass));
    u_i[counter].strategy=0;
    memset(u_i[counter].path,'\0',sizeof(u_i[counter].path));
    u_i[counter].limited_time=0;
    u_i[counter].num=0;
    u_i[counter].sum_fee=0;
    u_i[counter].sum_time=0;

    get_time(year,month,day,hour,minute);
    u_i[counter].city_from=ui->start->currentIndex();
    u_i[counter].city_to=ui->end->currentIndex();

    //途径城市1
    u_i[counter].city_pass[0]=ui->pass1->currentIndex();

    //途径城市2
    if(ui->pass1->currentIndex()!=0)
        u_i[counter].city_pass[1]=ui->pass2->currentIndex();

    //旅行策略
    u_i[counter].strategy=ui->strategy->currentIndex();
    if(ui->strategy->currentIndex()==3)
        u_i[counter].limited_time=ui->time->text().toDouble();

    //查询
    if(judge_input(u_i[counter])==1){
        switch (u_i[counter].strategy){
            case 1:
                least_fee(u_i[counter]); break;
            case 2:
                least_time(u_i[counter]); break;
            case 3:
                limited_least_fee(u_i[counter]); break;
            default:
                break;
        }

        //建立状态表，计算总时间
        state_build(counter);
        //输出路线
        if(u_i[counter].path[0]!=0||flag_strategy3!=1){
            print_route();
        }
        else
            QMessageBox::information(nullptr,"Backpacker：Warning","无有效路径");
    }
    else {
        QMessageBox::information(nullptr,"Backpacker：Warning","请输入正确信息");
    }
}


//输出结果
void routequery::print_route(){
    path_infor temp_path;
    int s_fee=0;
    QString text;
        ui->textBrowser->clear();
        ui->textBrowser->insertPlainText(tr("您的旅客编号为：%1\n\n").arg(counter));
        ui->textBrowser->moveCursor(QTextCursor::End);
        for(int i=1;i<=u_i[counter].path[0];i++)
        {
            search(u_i[counter].path[i], temp_path);
            text=QString("%1->%2    ").arg(node_to_city(temp_path.city_start)).arg(node_to_city(temp_path.city_end))
                    +QString("%3：%4\n").arg(node_to_type(temp_path.travel_type)).arg(temp_path.t_f_num)
                    +QString("出发：%4    到站：%5\n").arg(out_time(temp_path.time_begin)).arg(out_time(temp_path.time_end))
                    +QString("票价：￥%6\n\n").arg(temp_path.fee);
            ui->textBrowser->insertPlainText(text);
            ui->textBrowser->moveCursor(QTextCursor::End);
            s_fee+=temp_path.fee;
        }
        u_i[counter].sum_fee=s_fee;
        text=QString("总时间：%1\n").arg(out_time2(u_i[counter].sum_time))
                +QString("总票价：￥%2\n").arg(u_i[counter].sum_fee);
        ui->textBrowser->insertPlainText(text);
}


//返回主界面
void routequery::on_back_clicked()
{
    //清空数据
    u_i[counter].city_from=0;
    u_i[counter].city_to=0;
    memset(u_i[counter].city_pass,'\0',sizeof(u_i[counter].city_pass));
    u_i[counter].strategy=0;
    memset(u_i[counter].path,'\0',sizeof(u_i[counter].path));
    u_i[counter].limited_time=0;
    u_i[counter].num=0;
    u_i[counter].sum_fee=0;
    u_i[counter].sum_time=0;
    //选项恢复初始化
    ui->start->setCurrentIndex(0);
    ui->end->setCurrentIndex(0);
    ui->pass1->setCurrentIndex(0);
    ui->pass2->setCurrentIndex(0);
    ui->strategy->setCurrentIndex(0);
    ui->time->setValue(1);
    ui->textBrowser->clear();
    //返回
    emit backtomain();
}


//开始旅行
void routequery::on_go_clicked()
{
    //检查是否已经查询
    if(u_i[counter].path[0]==0){
        QMessageBox::information(nullptr,"Backpacker：Warning","您尚未查询路线！");
        return;
    }
    get_time(year,month,day,hour,minute);
    u_i[counter].num=counter;
    state_build(counter);
    s_i[counter].num=counter;
    outfile_user(u_i[counter]);
    outfile_state(s_i[counter]);
    counter++;  
    //选项恢复初始化
    ui->start->setCurrentIndex(0);
    ui->end->setCurrentIndex(0);
    ui->pass1->setCurrentIndex(0);
    ui->pass2->setCurrentIndex(0);
    ui->strategy->setCurrentIndex(0);
    ui->time->setValue(1);
    ui->textBrowser->clear();
    //返回主界面
    emit backtomain();
}
