#include "routechange.h"
#include "ui_routechange.h"


routechange::routechange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::routechange)
{
    ui->setupUi(this);
    setFixedSize(500,700);
}

routechange::~routechange()
{
    delete ui;
}


void routechange::on_back_clicked()
{
    //初始化
    ui->id->clear();
    ui->end->setCurrentIndex(0);
    ui->pass->setCurrentIndex(0);
    ui->strategy->setCurrentIndex(0);
    ui->time->setValue(1);
    ui->textBrowser->clear();
    //返回
    emit backtomain();
}


//旅客编号
int c_num;


//旅客编号是否输入合法
void routechange::on_id_textChanged(const QString &arg1)
{
    QRegExp regExp("^[0-9]*[1-9][0-9]*$");//只能输入数字
    ui->id->setValidator(new QRegExpValidator(regExp,this));
    ui->search->setEnabled(ui->id->hasAcceptableInput());
}


//限时是否可选
void routechange::on_strategy_currentIndexChanged(const QString &arg1)
{
    if(ui->strategy->currentIndex()==3)
        ui->time->setEnabled(true);
    else
        ui->time->setEnabled(false);
}


//路线更改
int change_path(int num){
    //获取修改路线请求时间
    get_time(year,month,day,hour,minute);
    int state_check;
    int i = 1;
    int d_check=day;
    double t_check=hour+(double)(minute)/100;
    //对当前时刻该旅客进行状态查询
    if(d_check>=u_i[num].first_day&&d_check<=u_i[num].first_day+s_i[num].day[s_i[num].day[0]]){
        //在旅行途中
        while (1){
            //找到对应天的位置
            if((month>u_i[num].tra_month)||((month==u_i[num].tra_month) && (d_check > (s_i[num].day[i]+u_i[num].first_day)) && (i<(s_i[num].place[0]+1)))){
                i++;
            }
            else break;
        }

        while (1){
            //找到时间节点的位置
            if((t_check >= s_i[num].time_cut[i+1]) && (d_check>=(s_i[num].day[i+2]+u_i[num].first_day)) && (i<(s_i[num].place[0]+1))){
                i=i+2;
                //同一天则循环判断
            }
            else if((t_check >= s_i[num].time_cut[i+1]) && (d_check<(s_i[num].day[i+2]+u_i[num].first_day)) && (i<(s_i[num].place[0]+1))){
                i=i+2;
                break;
                //隔天跳到下一条路线后跳出循环
            }
            else if(t_check >= s_i[num].time_cut[i] && t_check < s_i[num].time_cut[i + 1] && i<(s_i[num].place[0]+1)){
                i=i+1;
                break;
                //在途中两个时间节点之间
            }
            else break;
        }
        state_check = s_i[num].state[i - 1];
        //得到状态值
    }
    else if(d_check>u_i[num].first_day+s_i[num].day[s_i[num].day[0]]){
        state_check=0;
        //已到达
    }
    else{
        state_check=-2;
        //未出发
    }

    if (state_check > 0) {
        //若在途中，则以即将到达的城市为新路线的起始城市
        u_i[change_num].city_from = s_i[num].place[i];
    }
    else if(state_check==-1){
        //恰好在某地停留
        u_i[change_num].city_from = s_i[num].place[i-1];
    }
    else{
        //否则不能更改路线，返回-1
        return -1;
    }

    //将新的出发地作为原用户的一个途经城市存储
    u_i[num].city_pass[0] = u_i[change_num].city_from;

    //得到原用户路线及状态表已完成的部分（索引值）
    if (i % 2) {
        u_i[num].path[0] = (i - 1) / 2;
    }
    else {
        u_i[num].path[0] = i / 2;
    }
    s_i[num].day[0]=2*u_i[num].path[0];

    //根据新的旅行策略，规划新路线
    if (judge_input(u_i[change_num]) == 1) {
        switch (u_i[change_num].strategy) {
        case 1:
            least_fee(u_i[change_num]); break;
        case 2:
            least_time(u_i[change_num]); break;
        case 3:
            limited_least_fee(u_i[change_num]); break;
        default:break;
        }
    }
    else {
        QMessageBox::information(nullptr,"Backpacker：Warning","请输入正确信息");
    }
    //对新路线建立状态表
    state_build(change_num);

    //更新路线
    int j = 1;
    for (int k = u_i[num].path[0] + 1; k <= u_i[num].path[0] + u_i[change_num].path[0]; k++) {
        u_i[num].path[k] = u_i[change_num].path[j++];
        //直接将新路线添加到已完成路线的后边
    }
    u_i[num].path[0] += u_i[change_num].path[0];
    //更新状态表
    int m=1;
    for(int n=s_i[num].day[0]+1;n<=s_i[num].day[0]+s_i[change_num].day[0];n++){
        //旅行的day属性需要保持以原用户的出发日期为准，故需要加上出发日期的差值
        s_i[num].day[n]=u_i[change_num].first_day-u_i[num].first_day+s_i[change_num].day[m];
        //其余信息直接添加到后边
        s_i[num].place[n]=s_i[change_num].place[m];
        s_i[num].time_cut[n]=s_i[change_num].time_cut[m];
        s_i[num].state[n]=s_i[change_num].state[m];
        s_i[num].t_f[n]=s_i[change_num].t_f[m];
        m++;
    }
    s_i[num].day[0]+=s_i[change_num].day[0];
    s_i[num].place[0]=s_i[num].day[0];

    //更新总时间，根据新的状态表计算总时间
    double sum_t;
    int k=s_i[num].day[0];
    sum_t = s_i[num].day[k] * 24;
    if (s_i[num].time_cut[1] <= s_i[num].time_cut[k]) {
        sum_t += time_length(s_i[num].time_cut[1], s_i[num].time_cut[k]);
    }
    else{
        sum_t = time_length(time_length(s_i[num].time_cut[k], s_i[num].time_cut[1]),sum_t);
    }
    u_i[num].sum_time = sum_t;

    //更改路线完成，返回0
    return 0;
}


//输出路线
void routechange::print_route(){
    path_infor temp_path;
    int s_fee=0;
    QString text;
        ui->textBrowser->clear();
        ui->textBrowser->insertPlainText(tr("旅客编号%1的旅客，您的新路线为：\n\n").arg(c_num));
        ui->textBrowser->moveCursor(QTextCursor::End);
        for(int i=1;i<=u_i[c_num].path[0];i++)
        {
            search(u_i[c_num].path[i], temp_path);
            text=QString("%1->%2    ").arg(node_to_city(temp_path.city_start)).arg(node_to_city(temp_path.city_end))
                    +QString("%3：%4\n").arg(node_to_type(temp_path.travel_type)).arg(temp_path.t_f_num)
                    +QString("出发：%4    到站：%5\n").arg(out_time(temp_path.time_begin)).arg(out_time(temp_path.time_end))
                    +QString("票价：￥%6\n\n").arg(temp_path.fee);
            ui->textBrowser->insertPlainText(text);
            ui->textBrowser->moveCursor(QTextCursor::End);
            s_fee+=temp_path.fee;
        }
        u_i[c_num].sum_fee=s_fee;
        text=QString("总时间：%1\n").arg(out_time2(u_i[c_num].sum_time))
                +QString("总票价：￥%2\n").arg(u_i[c_num].sum_fee)
                +QString("从当前城市出发，到达新目的地仍需：%3\n").arg(out_time2(u_i[change_num].sum_time));
        ui->textBrowser->insertPlainText(text);
}


//查询
void routechange::on_search_clicked()
{
    //旅客编号是否输入正确
    if(ui->id->text().toInt()>=counter){
        QMessageBox::information(nullptr,"Backpacker：Warning","请输入正确的旅客编号");
        return;
    }

    c_num=ui->id->text().toInt();
    u_i[change_num].city_to=ui->end->currentIndex();

    //途径城市
    u_i[change_num].city_pass[0]=ui->pass->currentIndex();
    //获取策略
    u_i[change_num].strategy=ui->strategy->currentIndex();
    //第三种策略需输入限时
    if(ui->strategy->currentIndex()==3)
        u_i[change_num].limited_time=ui->time->text().toDouble();
    //更改路线
    change_path(c_num);
    if(change_path(c_num)==0){
        //路线已更改
        if(u_i[c_num].path[0]!=0||flag_strategy3!=1){
            print_route();
            ui->go->setEnabled(true);
        }
        else{
            QMessageBox::information(nullptr,"Backpacker：Warning","无有效路径");
        }
    }
    else{
        //路线不可更改
        QMessageBox::information(nullptr,"Backpacker：Warning","您当前的状态不可更改路线");
        ui->go->setEnabled(false);
    }

}


//继续旅行
void routechange::on_go_clicked()
{
    //输出到日志
    outfile_user(u_i[c_num]);
    outfile_state(s_i[c_num]);
    //初始化
    ui->id->clear();
    ui->end->setCurrentIndex(0);
    ui->pass->setCurrentIndex(0);
    ui->strategy->setCurrentIndex(0);
    ui->time->setValue(1);
    ui->textBrowser->clear();
    //返回
    emit backtomain();
}
