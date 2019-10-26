#include "statequery.h"
#include "ui_statequery.h"


int check_num;//当前旅客序号
int pre_check_num;//上一个查询的序号
int state_check=-2;//初始化状态为未出发
QString text4query;
QString text4allroute;
static QPointF pointPos;//当前位置
QPointF A(0,0);//起始城市
QPointF B(0,0);//到达城市
QPointF add(0,0);//每次移动的距离
double time_AB;//AB的时间差
double time_start2c;//起始时间与点击查询时刻的时间差
double time_c2next;//点击查询时刻与到达时间差
QTime gotime,waittime;


statequery::statequery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::statequery)
{
    ui->setupUi(this);
    setFixedSize(1400,900);
}

statequery::~statequery()
{
    delete ui;
}


//返回主界面
void statequery::on_back_clicked()
{
    emit backtomain();
}


//建立状态
void state_build(int num){

    int k = 1;
    s_i[num].place[0] = 2 * u_i[num].path[0];//城市数为路线数的2倍
    s_i[num].day[0] = 0;//旅行天数0
    s_i[num].state[0] = -2;//初始化状态-2
    s_i[num].t_f[0] = '0';
    for (int i = 1; i <= u_i[num].path[0]; i++) {
        //读取路线编号，并将路线信息存到temp_path中
        path_infor temp_path;
        search(u_i[num].path[i], temp_path);

        //每条路线对应状态表的两列

        //时间节点为出发时间
        s_i[num].time_cut[k] = temp_path.time_begin;
        if (s_i[num].time_cut[k] < s_i[num].time_cut[k - 1]) {
            //若出发时间比上一个到达时间小，则天数加一
            s_i[num].day[k] = s_i[num].day[k-1] + 1;
        }
        else {
            //否则当天进行
            s_i[num].day[k] = s_i[num].day[k-1];
        }
        //地点为出发城市
        s_i[num].place[k] = temp_path.city_start;
        //状态为出行方式
        s_i[num].state[k] = temp_path.travel_type;
        //车次号为对应车次号
        s_i[num].t_f[k] = temp_path.t_f_num;
        k++;

        //时间节点为到达时间
        s_i[num].time_cut[k] = temp_path.time_end;
        //所有车次不隔天，故天数与前一位一致
        s_i[num].day[k] = s_i[num].day[k-1];
        //地点为达到城市
        s_i[num].place[k] = temp_path.city_end;
        //状态为停留
        s_i[num].state[k] = -1;
        //车次号为空
        s_i[num].t_f[k] = '0';
        k++;
    }
    k--;
    //将天数[0]置为最后一个索引值
    s_i[num].day[0]=k;
    //状态最后一位置0
    s_i[num].state[s_i[num].place[0]] = 0;

    //计算总时间
    double sum_t;
    //计算总天数*24，得到旅行整天的小时数
    sum_t = s_i[num].day[k] * 24;
    //比较第一个出发地出发时间和目的地到达时间
    if (s_i[num].time_cut[1] <= s_i[num].time_cut[k]) {
        //用时大于整天，加上超出整天的部分
        sum_t += time_length(s_i[num].time_cut[1], s_i[num].time_cut[k]);
    }
    else{
        //否则减
        sum_t = time_length(time_length(s_i[num].time_cut[k], s_i[num].time_cut[1]),sum_t);
    }
    u_i[num].sum_time = sum_t;

    //确定出发日期和月份
    double s_t=hour+(double)(minute)/100;
    if(s_i[num].time_cut[1]<s_t){
        //若当前已错过出发时间，则后推一天
        u_i[num].first_day=inc_d(day,u_i[num].tra_month);
    }
    else{
        //否则当天即可出发
        u_i[num].first_day=day;
        u_i[num].tra_month=month;
    }
}


//用于文本形式的状态查询
void state_query(int num){
    //获取查询时刻的当前时间
    get_time(year,month,day,hour,minute);

    int i = 1;
    int d_check=day;
    double t_check=hour+(double)(minute)/100;
    //对当前时刻该旅客进行状态查询
    if(d_check>=u_i[num].first_day&&d_check<=u_i[num].first_day+s_i[num].day[s_i[num].day[0]]){
        //仍在旅行途中
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
        //得到状态值
        state_check = s_i[num].state[i - 1];
    }
    else if(d_check>u_i[num].first_day+s_i[num].day[s_i[num].day[0]]){
        state_check=0;
        //已到达
    }
    else{
        state_check=-2;
        //未出发
    }

    //输出文本信息
    text4query="查询时刻：\n"+QString::number(month)+"/"+QString::number(day)+" "+QString::number(hour)+":";
    //首先输出查询时间
    if(minute<10){
        text4query+="0"+QString::number(minute)+"\n";
    }
    else{
        text4query+=QString::number(minute)+"\n";
    }
    if (state_check > 0) {
        text4query+="在途\n"+node_to_city(s_i[num].place[i - 1])+"->"+node_to_city(s_i[num].place[i])+"\n"+node_to_type(state_check)+":"+s_i[num].t_f[i - 1]+"\n";
    }
    //到达目的地
    else if (state_check == 0) {
        text4query+="您已到达目的地："+node_to_city(s_i[num].place[s_i[num].place[0]])+"\n";
    }
    //未出发
    else if(state_check==-2){
        text4query+="您的旅程尚未开始，出发地："+node_to_city(s_i[num].place[1])+"\n";
    }
    //停留
    else {
        text4query+="您已到达途径城市："+node_to_city(s_i[num].place[i - 1])+"\n";
    }

}


//用于地图更新的状态查询
void state_query4map(int num){
    //获取查询时刻的当前时间
    get_time(year, month, day, hour, minute);

    int i = 1;
    int d_check = day;
    double t_check = hour + (double)(minute) / 100;
    //对当前时刻该旅客进行状态查询
    if (d_check >= u_i[num].first_day&&d_check <= u_i[num].first_day + s_i[num].day[s_i[num].day[0]]) {
        //仍在旅行途中
        while (1) {
            //找到对应天的位置
            if ((month > u_i[num].tra_month) || ((month == u_i[num].tra_month) && (d_check > (s_i[num].day[i] + u_i[num].first_day)) && (i < (s_i[num].place[0] + 1)))) {
                i++;
            }
            else break;
        }

        while (1) {
            //找到时间节点的位置
            if ((t_check >= s_i[num].time_cut[i + 1]) && (d_check >= (s_i[num].day[i + 2] + u_i[num].first_day)) && (i < (s_i[num].place[0] + 1))) {
                i = i + 2;
                //同一天则循环判断
            }
            else if ((t_check >= s_i[num].time_cut[i + 1]) && (d_check < (s_i[num].day[i + 2] + u_i[num].first_day)) && (i < (s_i[num].place[0] + 1))) {
                i = i + 2;
                break;
                //隔天跳到下一条路线后跳出循环
            }
            else if (t_check >= s_i[num].time_cut[i] && t_check < s_i[num].time_cut[i + 1] && i < (s_i[num].place[0] + 1)) {
                i = i + 1;
                break;
                //在途中两个时间节点之间
            }
            else break;
        }
        //得到状态值
        state_check = s_i[num].state[i - 1];
    }
    else if (d_check > u_i[num].first_day + s_i[num].day[s_i[num].day[0]]) {
        state_check = 0;
        //已到达
    }
    else {
        state_check = -2;
        //未出发
    }

    //途中
    if (state_check > 0) {
        A=getcityPos(s_i[num].place[i - 1]);
        B=getcityPos(s_i[num].place[i]);
        time_AB=time2second(time_length(s_i[num].time_cut[i-1],s_i[num].time_cut[i]));
        time_start2c=time2second(time_length(s_i[num].time_cut[i-1],t_check));
        time_c2next=time_AB-time_start2c;
        gotime.start();
        add=getmoveDistance();
        pointPos=getbeginPos();
    }
    //到达目的地
    else if (state_check == 0) {
        B=getcityPos(s_i[num].place[s_i[num].place[0]]);
        pointPos=B;
    }
    //未出发
    else if(state_check==-2){
        B=getcityPos(s_i[num].place[i]);
        pointPos=B;
        if(t_check<s_i[num].time_cut[i]){
            time_c2next=time2second(time_length(t_check,s_i[num].time_cut[i]));
        }
        else{
            time_c2next=time2second(time_sum(time_length(t_check,24),s_i[num].time_cut[i]));
        }
        waittime.start();
    }
    //停留
    else {
        B=getcityPos(s_i[num].place[i - 1]);
        pointPos=B;
        if(t_check<s_i[num].time_cut[i]){
            time_c2next=time2second(time_length(t_check,s_i[num].time_cut[i]));
        }
        else{
            time_c2next=time2second(time_sum(time_length(t_check,24),s_i[num].time_cut[i]));
        }
        waittime.start();
    }
}


//获得该旅客全部路线
QString statequery::getroute(int num){
    path_infor temp_path;
    int s_fee=0;
    QString text;
    ui->textBrowser_2->clear();
    for(int i=1;i<=u_i[num].path[0];i++)
    {
        search(u_i[num].path[i], temp_path);
        text+=QString("%1->%2    ").arg(node_to_city(temp_path.city_start)).arg(node_to_city(temp_path.city_end))
                +QString("%3：%4\n").arg(node_to_type(temp_path.travel_type)).arg(temp_path.t_f_num)
                +QString("出发：%4    到站：%5\n").arg(out_time(temp_path.time_begin)).arg(out_time(temp_path.time_end))
                +QString("票价：￥%6\n\n").arg(temp_path.fee);
        s_fee+=temp_path.fee;
    }

    u_i[num].sum_fee=s_fee;
    text+=QString("总时间：%1\n").arg(out_time2(u_i[num].sum_time))
            +QString("总票价：￥%2\n").arg(u_i[num].sum_fee);
    return text;
}


//判断id输入是否合法
void statequery::on_id_textChanged(const QString &arg1)
{
    QRegExp regExp("^[0-9]*[1-9][0-9]*$");//只能输入数字
    ui->id->setValidator(new QRegExpValidator(regExp,this));
    ui->search->setEnabled(ui->id->hasAcceptableInput());
}


//查询按钮
void statequery::on_search_clicked()
{
    ui->textBrowser->clear();
    pre_check_num=check_num;
    check_num=ui->id->text().toInt();
    //检查旅客编号
    if(check_num>=counter){
        QMessageBox::information(0,"Backpacker：Warning","旅客编号不存在");
        return;
    }

    //当前状态更新
    state_query(check_num);
    ui->textBrowser->insertPlainText(text4query);

    //全部路线更新
    text4allroute=getroute(check_num);
    ui->textBrowser_2->setPlainText(text4allroute);

    //若切换旅客，进行地图状态的更新
    if(check_num!=pre_check_num)
        state_query4map(check_num);

    //输出到日志
    outfile_query(check_num);
}


//更新画面
PaintLabel::PaintLabel(QWidget *parent):QLabel(parent){
    //背景
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/map.png")));
    this->setPalette(palette);
    //定时器用于更新画面
    paintmstimer = new QTimer;
    paintmstimer->start(1000);
    QObject::connect(paintmstimer, SIGNAL(timeout()), this, SLOT(update()));
}


//画图
void PaintLabel::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawPixmap((setpointPos()), setpointGraph(state_check));
}


//析构
PaintLabel::~PaintLabel(){}


//确定当前位置
QPointF setpointPos(){
    //在途
    if(state_check>0){
        if(gotime.elapsed()<=time_c2next*1000)
            pointPos+=add;
        else//查询下一状态
            state_query4map(check_num);
    }
    //到达
    else if(state_check==0)
        pointPos=B;
    //停留或未出发
    else {
        if(check_num==0||waittime.elapsed()<=time_c2next*1000)
            pointPos=B;
        else//查询下一状态
            state_query4map(check_num);
    }
    return pointPos;
}


//若在途，获得查询时刻的位置
QPointF getbeginPos(){
    double distance_x=B.x()-A.x();
    double distance_y=B.y()-A.y();

    double xIncrease=A.x()+(distance_x*(time_start2c/time_AB));
    double yIncrease=A.y()+(distance_y*(time_start2c/time_AB));

    return QPointF(xIncrease, yIncrease);
}


//每次移动的距离
QPointF getmoveDistance(){
    double distance_x=B.x()-A.x();
    double distance_y=B.y()-A.y();

    double xIncrease=distance_x/(time_AB);
    double yIncrease=distance_y/(time_AB);

    return QPointF(xIncrease, yIncrease);
}
