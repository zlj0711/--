#include "path_design.h"


path_design::path_design(){}


//*******************三个算法的共同全局变量*********************
const int maxnum = 100;
const int maxint = INT_MAX;
const int all_lines = 1023;//列车信息总行数

//*******************最小费用算法的全局变量**********************
const int vertex = 15;//顶点数 分别代表15个城市
int dist[maxnum];// 表示当前点到源点的最短路径长度
int prevx[maxnum];// 记录当前点的前一个结点
int weight[20][20];//迪杰斯特拉算法中有向图中边的权值
int tempp[20][20];//用于暂存weight[][]中的数据
int num[20][20];//从一个点到另一个点在信息表中对应的编号
int que[maxnum];//用于暂存从起点到终点费用最少的路径 倒序存储
int pathh[maxnum];//用于存储最终路径
int tot;//用于记录路径中城市的个数
int te_path[maxnum];//当途径城市为2个时，由于途径城市顺序可变，故要分两种情况讨论，te_path用于暂存第一种情况的路径

//*******************最短时间算法与限时最小费用算法共同的全局变量**********************
int result[maxnum][maxnum];//暂存test条路径结果
int final_result[maxnum][maxnum];//最终amount条路径结果
double arrive[maxnum];//存储每条路径从起点开始到达某一个城市后所用的总短时间 最终结果为每条路径的总用时
int trans[maxnum][maxnum];//暂存每条路径对应的车次
int final_path[maxnum];//最优路径
int final_trans[maxnum];//最优路径对应的车次

//*******************最短时间算法的全局变量**********************
const int test = 50;//随机出的总路径条数
int amount;//互不重复的路径条数
double times[maxnum];//每条路径的最短用时
double depart[maxnum];//每条路径最开始出发的时间
double temp_depart[maxnum];//暂存每条路径最开始出发的时间
double temp_arrive[maxnum];//暂存每条路径从起点开始到达某一城市后所用的总时间
double t_last_end_time;//记录上一小段路径的到达时间

//*******************限时最短费用算法的全局变量********************
const int te_ = 20;//随机出的总路径条数
int t_amount;//互不重复的路径条数
double last_end_time[maxnum] ;//上一段路径的到达时间
double te_arrive[maxnum][maxnum];//暂存每条路径从起点开始到达某一城市后所用的总时间
int fees[maxnum];//每条路径限定时间内的最优解
int temp_fees[maxnum][maxnum]; //暂存每条路径的总费用
int temp_trans[maxnum][maxnum];//暂存每条路径的车次
int path_num[maxnum][maxnum][maxnum];//从一个城市到另一个城市的所有车次
int flag_strategy3=0;//判断是否有符合要求的有效路径


//**************************最少费用算法***************************
//计算从一个城市到另一个城市所需的最小费用
void calcu_weight(int weight[][20], int num[][20]) {
    ifstream fin;
    fin.open("transport_list.csv", ios::in);
    if(!fin.is_open())
       std::cout<<"can't open file"<<std::endl;
    else{
        string linestr;
        getline(fin, linestr);//读取标题
        int i, j, start, end, cost;
        char c[10];
        for (i = 1; i <= all_lines; i++) {//遍历表中的每一行信息
            getline(fin, linestr, ',');
            getline(fin, linestr, ',');//读取出发地
            strcpy_s(c, linestr.c_str());//将string类型转换为char[]类型
            start = atoi(c);//将char[]类型转换为int类型
            memset(c, '\0', sizeof(c));//清空数组
            getline(fin, linestr, ',');//读取目的地
            strcpy_s(c, linestr.c_str());
            end = atoi(c);
            memset(c, '\0', sizeof(c));
            for (j = 1; j <= 4; j++) {
                getline(fin, linestr, ',');
            }
            getline(fin, linestr);//读取票价
            strcpy_s(c, linestr.c_str());
            cost = atoi(c);
            memset(c, '\0', sizeof(c));
            if (weight[start][end] > cost) {//更新数组，使weight[start][end]最终为从start到end的最小费用
                weight[start][end] = cost;
                num[start][end] = i;//最终num[start][end]为从start到end最小费用的车次信息对应列表中的行数
            }
        }
    }

}

//迪杰斯特拉算法
void Dijkstra(int end, int start, int *dist, int *prevx, int weight[][20], int special1, int special2) {
    bool s[maxnum];
    int i, j;
    for (i = 1; i <= 20; i++) {
        for (j = 1; j <= 20; j++) {
            tempp[i][j] = weight[i][j];
        }
    }//将weight[][]中的数据复制给tempp[][]
    for (i = 1; i <= vertex; i++) {
        tempp[i][special1] = maxint;
    }//任何一个点到special1的权值均为maxint，可以认为将special1从有向图中移除与任何一个点都没有连接
    for (i = 1; i <= vertex; i++) {
        tempp[i][special2] = maxint;
    }//任何一个点到special2的权值均为maxint，可以认为将special2从有向图中移除与任何一个点都没有连接
    for (i = 1; i <= end; i++) {//初始化dist和prevx
        dist[i] = tempp[start][i];//15个点到start的距离（费用）
        s[i] = 0;
        if (dist[i] == maxint) {
            prevx[i] = 0;//第i个点没有前一个点
        }
        else {
            prevx[i] = start;//第i个点的前一个点是起点
        }
    }
    dist[start] = 0;
    s[start] = 1;
    for (i = 2; i <= end; i++) {//共循环14次，计算从start到每个点的最短路径（如果存在的话）
        int temp = maxint, u = start, j, newdist;
        for (j = 1; j <= end; j++) {
            if (!s[j] && dist[j] < temp) {
                u = j;
                temp = dist[j];
            }
        }//u为与start距离最近的点，temp为start到u的权值
        s[u] = 1;
        for (j = 1; j <= end; j++) {//不断更新dist 不断更新路径
            if (!s[j] && tempp[u][j] < maxint) {
                newdist = dist[u] + tempp[u][j];//从start到j的距离
                if (newdist < dist[j]) {
                    dist[j] = newdist;
                    prevx[j] = u;//j的前一个点是u
                }
            }
        }
    }
}

//寻找使从start到end最少费用的路径
void searchpath(int *prevx, int start, int end) {
    tot = 1;
    que[tot] = end;//用倒序存储
    tot++;
    int temp = prevx[end];//找end的前一个点
    while (temp != start) {//直到找到起点为止
        que[tot] = temp;
        tot++;
        temp = prevx[temp];//依次找前一个点
    }
    que[tot] = start;
}

//最少费用算法
void least_fee(user_infor &user) {
    int start, end, city_num=0, city1, city2, special1 = 0, special2 = 0, fees, rem, temp_rem;
    /*start为起点城市 end为终点城市 city_num为途径城市的个数 city1为途径的第一个城市 city2为途径的第二个城市 fees为从起点
     * 到终点的最少费用
     */
    for (int i = 1; i <= vertex; i++) {
        for (int j = 1; j <= vertex; j++) {
            weight[i][j] = maxint;
        }
    }//将有向图中每条边的权值初始化，置为最大值
    calcu_weight(weight, num);//读列车信息，找出从一个城市到另一个城市所需的最小费用，并将其赋给有向图中对应边的权值
    for (int i = 1; i <= vertex; i++) {
        dist[i] = maxint;
    }//将当前点到源点的距离初始化，置为最大值
    start = user.city_from;
    end = user.city_to;
    for (int i = 0; i < 2; i++) {
        if (user.city_pass[i] != 0) city_num++;
    }//计算旅客途径城市个数
    if (city_num == 0) {//如果不途径城市
        Dijkstra(vertex, start, dist, prevx, weight, special1, special2);//直接用迪杰斯特拉算法计算从start点到其他点所需的最小费用及路径
        fees = dist[end];
        searchpath(prevx, start, end);//找从start到end最少费用的路径
        for (int i = tot; i >= 1; i--) {
            pathh[tot - i + 1] = que[i];
        }//按顺序将路径存入到pathh中
        pathh[0] = tot;//tot为路径中的城市数
        user.path[0] = pathh[0] - 1;//路径中的车次个数
        for (int i = 1; i < tot; i++) {
            user.path[i] = num[pathh[i]][pathh[i+1]];//将从pathh[i]到pathh[i+1]乘坐车次对应列表中的序号存储起来
        }
    }
    if (city_num == 1) {//途径城市个数为1
        city1 = user.city_pass[0];
        Dijkstra(vertex, start, dist, prevx, weight, end, special2);//计算从起点到city1的最少费用 途中不可以经过终点
        fees = dist[city1];
        searchpath(prevx, start, city1);//找出从start到city1最少费用的路径
        for (int i = tot; i >= 1; i--) {
            pathh[tot - i + 1] = que[i];
        }//按顺序将路径存入到pathh中
        memset(que, '\0', sizeof(que));
        rem = tot;//记录已有路径中的城市个数
        Dijkstra(vertex, city1, dist, prevx, weight, start, special2);//计算从city1到end的最少费用 途中不可以经过起点
        fees = fees + dist[end];//将两段的费用相加
        searchpath(prevx, city1, end);//找出从city1到end最少费用的路径
        for (int i = tot - 1; i >= 1; i--) {
            pathh[rem + tot - i] = que[i];
        }//将第二段路径按顺序写入pathh中
        pathh[0] = tot + rem - 1;//更新路径中已有城市个数
        user.path[0] = pathh[0] - 1;
        for (int i = 1; i < tot + rem-1; i++) {
            user.path[i] = num[pathh[i]][pathh[i + 1]];
        }
    }
    if (city_num == 2) {//途径城市个数为2
        city1 = user.city_pass[0];
        city2 = user.city_pass[1];
        //**************第一种情况：先途径city1再途径city2*****************
        Dijkstra(vertex, start, dist, prevx, weight, end, special2);//计算从start到city1的最少费用 途中不可以经过终点
        fees = dist[city1];
        searchpath(prevx, start, city1);
        int flag = 0;
        for (int i = tot; i >= 1; i--) {
            pathh[tot - i + 1] = que[i];
            if (que[i] == city2) {//如果从start到city1的路径中已经含有city2 则flag置1
                flag = 1;
            }
        }
        memset(que, '\0', sizeof(que));
        rem = tot;
        if (flag == 0) {//如果已有路径中没有city2
            Dijkstra(vertex, city1, dist, prevx, weight, start, end);//计算从city1到city2的最少费用 途中不可以经过起点和终点
            fees = fees + dist[city2];
            searchpath(prevx, city1, city2);
            for (int i = tot - 1; i >= 1; i--) {
                pathh[rem + tot - i] = que[i];
            }
            rem = rem + tot - 1;
            memset(que, '\0', sizeof(que));
            Dijkstra(vertex, city2, dist, prevx, weight, city1, start);//计算从city2到end的最少费用 途中不可以经过city1和起点
            fees = fees + dist[end];
            searchpath(prevx, city2, end);
            for (int i= tot - 1; i >= 1; i--) {
                pathh[rem + tot - i] = que[i];
            }
        }
        if (flag == 1) {//如果已有路径中包含city2
            Dijkstra(vertex, city1, dist, prevx, weight, start, city2);//计算从city1到end的最少费用 途中不可以经过起点和city2
            fees = fees + dist[end];
            searchpath(prevx, city1, end);
            for (int i = tot - 1; i >= 1; i--) {
                pathh[rem + tot - i] = que[i];
            }
        }
        te_path[0] = fees;//暂存先途径city1后途径city2这种情况下的最小费用
        temp_rem = tot + rem - 1;//暂存先途径city1后途径city2这种情况下路径中的城市个数
        for (int i = 1; i <= tot + rem - 1; i++) {
            te_path[i] = pathh[i];
        }//暂存先途径city1后途径city2这种情况下的路径
        memset(pathh, '\0', sizeof(pathh));
        memset(que, '\0', sizeof(que));
        fees = 0;
        //**************第二种情况：先途径city2再途径city1*****************
        Dijkstra(vertex, start, dist, prevx, weight, end, special2);//计算从start到city2的最少费用 途中不可以经过终点
        fees = dist[city2];
        searchpath(prevx, start, city2);
        flag = 0;
        for (int i = tot; i >= 1; i--) {
            pathh[tot - i + 1] = que[i];
            if (que[i] == city1) {//如果从start到city2的路径中已经含有city1 则flag置1
                flag = 1;
            }
        }
        memset(que, '\0', sizeof(que));
        rem = tot;
        if (flag == 0) {//如果已有路径中不含city1
            Dijkstra(vertex, city2, dist, prevx, weight, start, end);//计算从city2到city1的最少费用 途中不可以经过起点和终点
            fees = fees + dist[city1];
            searchpath(prevx, city2, city1);
            for (int i = tot - 1; i >= 1; i--) {
                pathh[rem + tot - i] = que[i];
            }
            rem = rem + tot - 1;
            memset(que, '\0', sizeof(que));
            Dijkstra(vertex, city1, dist, prevx, weight, city2, start);//计算从city1到end的最少费用 途中不可以经过起点和city2
            fees = fees + dist[end];
            searchpath(prevx, city1, end);
            for (int i = tot - 1; i >= 1; i--) {
                pathh[rem + tot - i] = que[i];
            }
        }
        if (flag == 1) {//如果已有路径中含有city1
            Dijkstra(vertex, city2, dist, prevx, weight, start, city1);//计算从city2到end的最少费用 途中不可以经过起点和city1
            fees = fees + dist[end];
            searchpath(prevx, city2, end);
            for (int i = tot - 1; i >= 1; i--) {
                pathh[rem + tot - i] = que[i];
            }
        }
        if (fees > te_path[0]) {//如果第一种情况的最少费用小于第二种情况的最少费用 则选择第一种情况的结果 将结果重新赋给pathh
            fees = te_path[0];
            memset(pathh, '\0', sizeof(pathh));
            pathh[0] = temp_rem;
            for (int i = 1; i <= temp_rem; i++) {
                pathh[i] = te_path[i];
            }
        }
        else {//如果第二种情况的费用最少
            pathh[0] = tot + rem - 1;
        }
        user.path[0] = pathh[0] - 1;
        for (int i = 1; i < pathh[0]; i++) {
            user.path[i] = num[pathh[i]][pathh[i + 1]];
        }
    }
}


//**************************最短时间算法***************************
//random_one()函数和extern_num()函数为最短时间算法与限时最少费用算法共用
//随机产生一条从start到end最大长度不超过limit_num的路径
void random_one(int num, int start, int end, int limit_num) {
    int temp_num, i;
    result[num][0] = 1;//路径中的城市个数
    result[num][1] = start;
    while (1) {//直至产生符合要求的路径为止
        while (1) {//产生一个随机的城市
            temp_num = rand() % 14 + 2;//在15个城市中选择一个
            int flag = 0;
            for (i = 1; i <= result[num][0]; i++) {
                if (result[num][i] == temp_num) {
                    flag = 1;
                }//如果随机数对应的城市在已有路径中已经存在，将flag置1，重新产生随机数
            }
            if (flag == 0) {//如果随机数对应的城市未出现在已有路径中，则跳出循环
                break;
            }
        }
        result[num][result[num][0] + 1] = temp_num;
        result[num][0]++;
        if (result[num][0] == limit_num && temp_num != end) {
            //如果路径长度已经达到最大值，且最后一个城市不是终点城市，则将最后一个城市改写为终点城市
            result[num][limit_num] = end;
            break;
        }
        if (temp_num == end) {//如果已经随机到了终点城市 则代表已生成了一条成功的路径 跳出
            break;
        }
    }
}

//将一个小数分成一个整数和一个小数 例如9.40将分成9和0.40
void extern_num(double num, int &i, double &f) {
    stringstream ss;
    string temp;
    ss << num;
    ss >> temp;//将double类型转换为string类型
    string temp1;
    char c[20];
    int lenth = temp.length();
    if (lenth == 4) {//如果temp的长度为4 即temp的格式为 ab.c
        temp1 = temp.substr(0, 2);//取出temp中代表整数的部分 即ab
        strcpy_s(c, temp1.c_str());//将string类型转换为char[]类型
        i = atoi(c);//将char[]类型转换为int类型
        memset(c, '\0', sizeof(c));
        temp = temp.substr(1, 3);//取出temp中代表小数的部分 即b.c
        temp[0] = '0';//将b.c改为0.c
        strcpy_s(c, temp.c_str());//将string类型转换为char[]类型
        f = atof(c);//将char[]类型转换为double类型
    }
    if (lenth == 3) {//如果temp的长度为3 即temp的格式为 a.b
        temp1 = temp.substr(0, 1);//取出temp中代表整数的部分 即a
        strcpy_s(c, temp1.c_str());
        i = atoi(c);
        memset(c, '\0', sizeof(c));
        temp[0] = '0';//将a.b改为0.b
        strcpy_s(c, temp.c_str());
        f = atof(c);
    }
    if (lenth == 2 || lenth == 1) {//如果temp的长度为2 即temp的格式为ab 如果temp的长度为1 即temp的格式为a
        strcpy_s(c, temp.c_str());
        i = atoi(c);
        f = 0;
    }
}

//计算从一个城市到另一个城市的时间，并将其加到路径中的总时间里
double readfile(ifstream &fin, int num, int flag) {
    string start, end, linestr;
    double t_start, t_end, f_t_start, f_t_end, f_final_time = 0, final_time = 0;
    int i_t_start, i_t_end, i_final_time;
    getline(fin, linestr, ',');
    getline(fin, linestr, ',');
    getline(fin, start, ',');//读取出发时间
    getline(fin, end, ',');//读取到达时间
    size_t a = start.find_first_of(":");
    size_t b = end.find_first_of(":");
    char c[20];
    if (a == 1) {//如果出发时间的格式为 *：** 将其改为 *.**的小数形式
        c[0] = start[0];
        c[1] = '.';
        c[2] = start[2];
        c[3] = start[3];
    }
    if (a == 2) {//如果出发时间的格式为 **：** 将其改为 **.**的小数形式
        c[0] = start[0];
        c[1] = start[1];
        c[2] = '.';
        c[3] = start[3];
        c[4] = start[4];
    }
    t_start = atof(c);//将char[]类型转换为double类型
    memset(c, '\0', sizeof(c));
    if (b == 1) {//如果到达时间的格式为 *：** 将其改为 *.**的小数形式
        c[0] = end[0];
        c[1] = '.';
        c[2] = end[2];
        c[3] = end[3];
    }
    if (b == 2) {//如果到达时间的格式为 **：** 将其改为 **.**的小数形式
        c[0] = end[0];
        c[1] = end[1];
        c[2] = '.';
        c[3] = end[3];
        c[4] = end[4];
    }
    t_end = atof(c);//将char[]类型转换为double类型
    extern_num(t_start, i_t_start, f_t_start);//将一个小数分成整数和小数 例如9.40将分成9和0.40
    extern_num(t_end, i_t_end, f_t_end);
    if (flag == 1) {//如果所求的那一小段路径为整个路径的第一段
        if (f_t_end >= f_t_start) {//如果到达时间的小数部分大于出发时间的小数部分 例如：出发时间7：30 到达时间10：40
            f_final_time = f_t_end - f_t_start;
            i_final_time = i_t_end - i_t_start;
            final_time = i_final_time + f_final_time;
        }
        else {//如果到达时间的小数部分小于出发时间的小数部分 例如：出发时间7：30 到达时间10：10
            f_final_time = f_t_end + 0.60 - f_t_start;
            i_final_time = i_t_end - 1 - i_t_start;
            final_time = i_final_time + f_final_time;
        }
        temp_depart[num] = t_start;//记录暂时的出发地
        temp_arrive[num] = t_end;//记录暂时的目的地
    }
    else {//如果所求的那一小段路径不是整个路径的第一段
        int i_compare;
        double f_compare;
        extern_num(t_last_end_time, i_compare, f_compare);//将上一段路径中的到达时间分成整数部分和小数部分 用于比较
        double f_interval, i_interval, interval;
        if (i_compare <= i_t_start) {//上一段到达时间的整数部分小于等于此路径出发时间的整数部分
            if (f_t_end >= f_t_start) {//例如：此路径出发时间9：40 到达时间12：50
                f_final_time = f_t_end - f_t_start;
                i_final_time = i_t_end - i_t_start;
            }
            else {//例如：此路径出发时间9：40 到达时间12：20
                f_final_time = f_t_end + 0.60 - f_t_start;
                i_final_time = i_t_end - 1 - i_t_start;
            }
            final_time = f_final_time + i_final_time;
            //计算上一段路径的到达时间与此段路经出发时间之间的间隔
            if (f_t_start >= f_compare) {//例如：上一段路径的到达时间7：30 此段路经的出发时间10：40
                f_interval = f_t_start - f_compare;
                i_interval = i_t_start - i_compare;
                interval = f_interval + i_interval;
            }
            else {//例如：上一段路径的到达时间7：30 此段路经的出发时间10：20
                f_interval = f_t_start + 0.60 - f_compare;
                i_interval = i_t_start - 1 - i_compare;
                interval = f_interval + i_interval;
            }
            final_time = interval + final_time;//最终此段路程花费的总时间等于路上所用的时间加上等待出发的时间
            extern_num(final_time, i_final_time, f_final_time);
            if (f_final_time >= 0.60) {//如果两个数相加后小数部分大于等于0.6 则向整数部分进位一小时 例如：8.70->9.10
                i_final_time++;
                f_final_time = f_final_time - 0.60;
            }
            final_time = i_final_time + f_final_time;
            final_time = final_time + times[num];//加上之前已经计算过的路径所花费的总时间 就是从起点开始走完当前路径花费的总时间
            extern_num(final_time, i_final_time, f_final_time);
            if (f_final_time >= 0.60) {//同理 所有代表时间的小数相加后都要判断小数部分是不是超过了理论值
                i_final_time++;
                f_final_time = f_final_time - 0.60;
            }
            final_time = i_final_time + f_final_time;
        }
        else {//上一段到达时间的整数部分小于等于此路径出发时间的整数部分
            if (f_t_end >= f_t_start) {//例如：此路径出发时间9：40 到达时间12：50
                f_final_time = f_t_end - f_t_start;
                i_final_time = i_t_end - i_t_start;
            }
            else {//例如：此路径出发时间9：40 到达时间12：20
                f_final_time = f_t_end + 0.60 - f_t_start;
                i_final_time = i_t_end - 1 - i_t_start;
            }
            final_time = f_final_time + i_final_time;
            //计算上一段路径的到达时间与此段路经出发时间之间的间隔
            if ((f_t_start - f_compare) < 0) {//例如：上一段路径的到达时间10：30 此段路经的出发时间7：20
                f_interval = f_t_start + 0.60 - f_compare;
                i_interval = i_t_start + 23 - i_compare;
                if (f_interval >= 0.60) {
                    f_interval = f_interval - 0.60;
                    i_interval++;
                }
                interval = f_interval + i_interval;
            }
            else {//例如：上一段路径的到达时间10：30 此段路径的出发时间7：40
                f_interval = f_t_start - f_compare;
                i_interval = i_t_start + 24 - i_compare;
                if (f_interval >= 0.60) {
                    f_interval = f_interval - 0.60;
                    i_interval++;
                }
                interval = f_interval + i_interval;
            }
            final_time = interval + final_time;//最终此段路程花费的总时间等于路上所用的时间加上等待出发的时间
            extern_num(final_time, i_final_time, f_final_time);
            if (f_final_time >= 0.60) {
                i_final_time++;
                f_final_time = f_final_time - 0.60;
            }
            final_time = i_final_time + f_final_time;
            final_time = final_time + times[num];//加上之前已经计算过的路径所花费的总时间 就是从起点开始走完当前路径花费的总时间
            extern_num(final_time, i_final_time, f_final_time);
            if (f_final_time >= 0.60) {
                i_final_time++;
                f_final_time = f_final_time - 0.60;
            }
            final_time = i_final_time + f_final_time;
        }
        temp_arrive[num] = t_end;//记录暂时的目的地
    }
    return final_time;
}

//计算从start到end的最短时间
void min_time(int start, int end, int num, int flag) {
    ifstream fin;
    double temp, temp_time;
    fin.open("transport_list.csv", ios::in);
    if (!fin) {
        qDebug() << "文件未打开" << endl;
    }
    string linestr;
    char a[20], b[20];
    int i;
    getline(fin, linestr);
    temp_time = 1000;
    for (i = 1; i <= all_lines; i++) {//遍历整个列车信息
        memset(a, '\0', sizeof(a));
        memset(b, '\0', sizeof(b));
        getline(fin, linestr, ',');
        getline(fin, linestr, ',');//读取出发地
        strcpy_s(a, linestr.c_str());//将string类型转换为char[]类型
        getline(fin, linestr, ',');//读取目的地
        strcpy_s(b, linestr.c_str());
        if (atoi(a) == start && atoi(b) == end) {//如果这一行信息的出发地和目的地正好对应start和end
            if (flag == 1) {//如果是整条路经的第一段
                temp = readfile(fin, num, 1);//计算所需的最短时间
            }
            else {//如果不是整条路径的第一段
                temp = readfile(fin, num, 2);//计算所需的最短时间
            }
            if (temp < temp_time) {//逐渐更新最短时间、车次对应列表中的序号、出发地、目的地等
                temp_time = temp;
                trans[num][trans[num][0]] = i;
                depart[num] = temp_depart[num];
                arrive[num] = temp_arrive[num];
            }
        }
        getline(fin, linestr);
    }
    times[num] = temp_time;//更新从起点到当前城市所用的总时间
    t_last_end_time = arrive[num];//记录上一段路径的到达时间
    fin.close();
}

//计算第num条路径所需的最短时间
void find_time(int num) {
    int i, start, end;
    trans[num][0] = 0;
    start = final_result[num][1];
    end = final_result[num][2];
    trans[num][0]++;
    min_time(start, end, num, 1);//计算从start到end所需的最短时间
    for (i = 2; i < final_result[num][0]; i++) {//将整条路径分段，分别计算每一小段的最短时间
        start = final_result[num][i];
        end = final_result[num][i + 1];
        trans[num][0]++;
        min_time(start, end, num, 2);
    }
}

//最短时间算法
void least_time(user_infor &user) {
    amount=0;
    int start = 0, end = 0, i, j, k, num = 0, city1 = 0, city2 = 0, flag1, flag2;
    int limit_num;
    start = user.city_from;
    end = user.city_to;
    for (int i = 0; i < 2; i++) {
        if (user.city_pass[i] != 0) num++;
    }//计算途径城市的个数
    limit_num=num+3;//随机路线时路径的最大长度
    city1 = user.city_pass[0];
    city2 = user.city_pass[1];
    memset(result, '\0', sizeof(result));
    memset(final_result, '\0', sizeof(final_result));
    memset(temp_depart, '\0', sizeof(temp_depart));
    memset(temp_arrive, '\0', sizeof(temp_arrive));
    memset(trans,'\0',sizeof(trans));
    memset(final_path, '\0', sizeof(final_path));
    memset(final_trans, '\0', sizeof(final_trans));
    memset(times,'\0',sizeof(times));
    srand((unsigned)time(NULL));//设置随机数种子
    for (i = 1; i <= test; i++) {//找出50条随机路线
        if (num == 0) {//如果不途径城市
            random_one(i, start, end, limit_num-1);//产生1条从起点到终点最大长度不超过limit_num-1的路径
        }
        if (num == 1) {//如果途径城市个数为1
            while (1) {
                for (j = 1; j <= 100; j++) {//初始化result
                    result[i][j] = 0;
                }
                result[i][0] = 0;
                random_one(i, start, end, limit_num);//产生1条从起点到终点最大长度不超过limit_num-1的路径
                if (result[i][0] != 2) {//如果产生路径的长度大于等于3 则跳出 否则重新产生路径
                    break;
                }
            }
            flag1 = 0;
            for (j = 1; j <= result[i][0]; j++) {
                if (result[i][j] == city1) {
                    flag1 = 1;
                    break;
                }//判断路径中是否有city1
            }
            if (flag1 == 0) {//如果路径中没有city1 则随机选取出起点终点以外的点 将其换成city1
                int rand_min = 2;
                int rand_max = result[i][0] - 1;
                int temp_rand = rand() % (rand_max - rand_min + 1) + rand_min;
                result[i][temp_rand] = city1;
            }
        }
        if (num == 2) {//如果途径城市个数为2
            while (1) {
                for (j = 1; j <= 100; j++) {//初始化result
                    result[i][j] = 0;
                }
                result[i][0] = 0;
                random_one(i, start, end, limit_num);//产生1条从起点到终点最大长度不超过limit_num-1的路径
                if (result[i][0] == 4 || result[i][0] == 5) {//如果产生路径的长度等于4或等于5 则跳出 否则重新产生路径
                    break;
                }
            }
            flag1 = 0; flag2 = 0;
            for (j = 1; j <= result[i][0]; j++) {//判断产生的路径中是否有city1和city2 如果二者都有 则跳出
                if (result[i][j] == city1) {
                    flag1 = 1;
                }
                if (result[i][j] == city2) {
                    flag2 = 1;
                }
                if (flag1 == 1 && flag2 == 1) {
                    break;
                }
            }
            if (flag1 == 0) {//如果没有city1
                int rand_min = 2;
                int rand_max = result[i][0] - 1;
                int temp_rand;
                while (1) {
                    temp_rand = rand() % (rand_max - rand_min + 1) + rand_min;//随机选取除了起点和终点的点
                    if (result[i][temp_rand] != city2) {//如果随机选取的位置对应的城市不是city2 则将其换成city1
                        result[i][temp_rand] = city1;
                        break;
                    }
                }
            }
            if (flag2 == 0) {//如果没有city2
                int rand_min = 2;
                int rand_max = result[i][0] - 1;
                int temp_rand;
                while (1) {
                    temp_rand = rand() % (rand_max - rand_min + 1) + rand_min;//随机选取除了起点和终点的点
                    if (result[i][temp_rand] != city1) {//如果随机选取的位置对应的城市不是city1 则将其换成city2
                        result[i][temp_rand] = city2;
                        break;
                    }
                }
            }
        }
    }
    for (i = 1; i <= test; i++) {//找出50条中重复的路线
        for (j = 1; j <= i - 1; j++) {
            int flag = 0;
            if (result[j][0] == result[i][0]) {
                //如果路径j的路径长度与路径i的路径长度相同，且路线也完全相同 flag=0 否则flag=1
                for (k = 1; k <= result[j][0]; k++) {
                    if (result[j][k] != result[i][k]) {
                        flag = 1;
                        break;
                    }
                }
            }
            else {
                flag = 1;
            }
            if (flag == 0) {//如果发现了与路径i相同的路径
                result[i][0] = 0;
                break;
            }
        }
    }
    for (i = 1; i <= test; i++) {//将互相不重复的路径写入final_result中
        if (result[i][0] != 0) {
            amount++;//用于计算最终路径的条数
            final_result[amount][0] = result[i][0];
            for (j = 1; j <= result[i][0]; j++) {
                final_result[amount][j] = result[i][j];
            }
        }
    }
    for (i = 1; i <= amount; i++) {//计算amount条路线所需的最短时间
        find_time(i);
    }
    double minus = 999;
    int rem=0;
    for (i = 1; i <= amount; i++) {//从amount条路线中选取所用时间最少的一条
        if (times[i] < minus) {
            minus = times[i];
            rem = i;//rem用于记录用时最少的路线的序号
        }
    }
    for (i = 1; i <= final_result[rem][0]; i++) {//将最优的路径存入final_path中
        final_path[i] = final_result[rem][i];
    }
    final_path[0] = final_result[rem][0];
    for (i = 1; i <= trans[rem][0]; i++) {//将最优路径对应的车次存入final_trans中
        final_trans[i] = trans[rem][i];
    }
    final_trans[0] = trans[rem][0];
    user.path[0] = final_trans[0];
    for (i = 1; i <= final_trans[0]; i++) {
        user.path[i] = final_trans[i];
    }
}


//**************************限时最小费用算法************************
//将一个城市到另一个城市的全部车次存储起来
void find_path_num() {
    ifstream fin;
    fin.open("transport_list.csv", ios::in);
    if (!fin) {
        qDebug() << "Failed." << endl;
    }
    string linestr;
    char c[20];
    int start, end;
    getline(fin, linestr);
    for (int i = 1; i <= all_lines; i++) {//遍历列表中所有车次信息
        getline(fin, linestr, ',');
        getline(fin, linestr, ',');//读取出发地
        strcpy_s(c, linestr.c_str());//将string类型转换为char[]类型
        start = atoi(c);//将char[]类型转换为int类型
        memset(c, '\0', sizeof(c));
        getline(fin, linestr, ',');//读取目的地
        strcpy_s(c, linestr.c_str());
        end = atoi(c);
        path_num[start][end][0]++;
        path_num[start][end][path_num[start][end][0]] = i;//将其在表格中对应的序号存下来 方便之后查找
        getline(fin, linestr);
    }
    fin.close();
}

//两个代表时间的小数相减 b为减数 a为被减数
double sub(int i_a, double f_a, int i_b, double f_b) {
    int i_time;
    double f_time, time;
    if (i_b >i_a) {//如果减数的整数部分大于被减数的整数部分
        if (f_b >= f_a) {//例如：9.4-7.2
            f_time = f_b - f_a;
            i_time = i_b - i_a;
        }
        else {//例如：9.2-7.5
            f_time = f_b + 0.60 - f_a;
            i_time = i_b - 1 - i_a;
        }
    }
    if(i_b==i_a){//如果减数的整数部分等于被减数的整数部分
        if(f_b>=f_a){//例如：9.5-9.3
            f_time = f_b - f_a;
            i_time = i_b - i_a;
        }
        else{//例如：9.3-9.5
            f_time = f_b + 0.60 - f_a;
            i_time = i_b + 23 - i_a;
        }
    }
    if(i_b<i_a) {//如果减数的整数部分小于被减数的整数部分
        if (f_b >= f_a) {//例如：7.5-9.2
            f_time = f_b - f_a;
            i_time = i_b + 24 - i_a;
        }
        else {//例如：7.2-9.5
            f_time = f_b + 0.60 - f_a;
            i_time = i_b + 23 - i_a;
        }
    }
    time = i_time + f_time;
    return time;
}

//两个代表时间的小数相加
double add(double a, double b) {
    double result = a + b, f_result;
    int i_result;
    extern_num(result, i_result, f_result);
    if (f_result >= 0.60) {//如果小数部分大于等于0.6 则向整数部分进1，小数部分减0.6
        f_result = f_result - 0.60;
        i_result++;
        result = f_result + i_result;
    }
    return result;
}

//计算已有路径加上第lines行的车次信息后花费的总时间总费用
void read_file(int lines, int num, int flag, int start_num) {
    ifstream fin;
    fin.open("transport_list.csv", ios::in);
    string linestr;
    int i;
    char c[20];
    for (i = 1; i <= lines; i++) {
        getline(fin, linestr);
    }
    for (i = 1; i <= 5; i++) {
        getline(fin, linestr, ',');
    }
    double start_time, end_time, f_start_time, f_end_time, cost_time, interval, f_compare;
    int i_start_time, i_end_time, i_compare, now_fees;

    QString tmp_string;
    QStringList tmp_list;

    getline(fin, linestr, ',');//读取出发时间
    tmp_string=QString::fromStdString(linestr);
    tmp_list=tmp_string.split(":");
    tmp_string=tmp_list[0]+"."+tmp_list[1];//将时间中的“：”改成“.”
    start_time=tmp_string.toDouble();//将string类型转换为double类型

    getline(fin, linestr, ',');//读取到达时间
    tmp_string=QString::fromStdString(linestr);
    tmp_list=tmp_string.split(":");
    tmp_string=tmp_list[0]+"."+tmp_list[1];
    end_time=tmp_string.toDouble();

    getline(fin, linestr);//读取票价
    strcpy_s(c, linestr.c_str());
    now_fees = atoi(c);
    extern_num(start_time, i_start_time, f_start_time);//将出发时间分成一个整数部分一个小数部分
    extern_num(end_time, i_end_time, f_end_time);//将到达时间分成一个整数部分一个小数部分
    cost_time = sub(i_start_time, f_start_time, i_end_time, f_end_time);//两个代表时间的小数相减
    if (flag == 1) {//如果此段路径时整条路径的第一段
        temp_fees[num][start_num] = now_fees;//该段的费用就是目前为止的总费用
        te_arrive[num][start_num] = cost_time;//该段所花费时间就是目前为止的总时间
    }
    else {//如果此段路径不是整条路径的第一段
        extern_num(last_end_time[start_num-1], i_compare, f_compare);
        interval = sub(i_compare, f_compare, i_start_time, f_start_time);
        //计算上一段路径到达时间与此段路径出发时间的间隔，即旅客停留的时间
        te_arrive[num][start_num] = add(te_arrive[num][start_num - 1], cost_time);
        te_arrive[num][start_num] = add(te_arrive[num][start_num], interval);
        //目前为止花费的总时间等于之前已走路径的时间+停留时间+此段路径花费的时间
        temp_fees[num][start_num] = temp_fees[num][start_num - 1] + now_fees;
        //目前为止的总费用等于之前已走路程的总费用+此段路程的费用
    }
    if (start_num != final_result[num][0] - 1){//如果此段路经不是最后一段，则记录此段路径的到达时间，便于下一段路径的计算
        last_end_time[start_num]=end_time;
    }
    fin.close();
}

//计算第num条路径在限定时间内的最少费用
void find_min_fee(int num, int start_num, int flag, double limit_time) {
    int start = final_result[num][start_num];
    int end = final_result[num][start_num + 1];
    int i, j;
    flag = 0;
    for (i = 1; i <= path_num[start][end][0]; i++) {//将一整条路径分成几个小段，根据背包问题的思想选择最优解
        if (start_num == 1) {//如果是整条路径的第一段，flag置1，对数组初始化
            flag = 1;
            te_arrive[num][start_num] = 0;
            temp_fees[num][start_num] = 0;
        }
        temp_trans[num][start_num] = path_num[start][end][i];//逐一试验start到end的所有方案
        read_file(path_num[start][end][i], num, flag, start_num);//计算选择path_num[start][end][i]这个车次时花费的费用与时间
        if (start_num != final_result[num][0] - 1) {//如果没有计算完整条路径，则依次计算接下来的路径
            find_min_fee(num, start_num+1, flag, limit_time);
        }
        if(start_num==final_result[num][0]-1){//如果已经计算完整条路经
            if (te_arrive[num][start_num] <= limit_time && temp_fees[num][start_num] < fees[num]) {
                //如果整条路经花费的时间小于等于限时的时间，并且花费的费用小于同样路径其他策略所花费的费用，则更新每条路径的最优解信息
                fees[num] = temp_fees[num][start_num];
                arrive[num] = te_arrive[num][start_num];
                for (j = 1; j <= trans[num][0]; j++) {
                    trans[num][j] = temp_trans[num][j];
                }
            }
        }
    }
}

//限时最小费用算法
void limited_least_fee(user_infor &user) {
    t_amount=0;
    int start, end, i, j, k, flag1, flag2;
    int num = 0, limit_num;
    double limit_time = user.limited_time;//旅客的限制时间
    start = user.city_from;
    end = user.city_to;
    for (int i = 0; i < 2; i++) {
        if (user.city_pass[i]) {
            num++;
        }
    }//计算途径的城市个数
    limit_num=num+3;
    memset(result, '\0', sizeof(result));
    memset(final_result, '\0', sizeof(final_result));
    memset(temp_fees,'\0',sizeof(temp_fees));
    memset(arrive,'\0',sizeof(arrive));
    memset(te_arrive,'\0',sizeof(te_arrive));
    memset(fees,'\0',sizeof(fees));
    memset(temp_trans,'\0',sizeof(temp_trans));
    memset(trans,'\0',sizeof(trans));
    memset(final_path,'\0',sizeof(final_path));
    memset(final_trans,'\0',sizeof(final_trans));
    memset(path_num,'\0',sizeof(path_num));
    memset(last_end_time,'\0',sizeof(last_end_time));
    srand((unsigned)time(NULL));//设置随机数种子
    for (i = 1; i <= te_; i++) {//随机产生20条路线
        if (num == 0) {//如果没有途径城市
            random_one(i, start, end, limit_num - 1);//随机产生一条从start到end最大长度不超过limit_num-1的路径
        }
        if (num == 1) {//如果途径城市个数是1
            while (1) {
                for (j = 1; j <= 100; j++) {
                    result[i][j] = 0;
                }
                result[i][0] = 0;
                random_one(i, start, end, limit_num);//随机产生一条从start到end最大长度不超过limit_num的路径
                flag1 = 0;
                for (j = 1; j <= result[i][0]; j++) {//如果随机产生的路径中由途径城市1，则随机路径成功，跳出循环
                    if (result[i][j] == user.city_pass[0]) {
                        flag1 = 1;
                        break;
                    }
                }
                if (flag1 == 1) {
                    break;
                }
            }
        }
        if (num == 2) {//如果途径城市个数是2
            while (1) {
                for (j = 1; j <= 100; j++) {
                    result[i][j] = 0;
                }
                result[i][0] = 0;
                random_one(i, start, end, limit_num);//随机产生一条从start到end最大长度不超过limit_num的路径
                flag1 = 0; flag2 = 0;
                for (j = 1; j <= result[i][0]; j++) {
                    //判断随机出的路径中是否出现了途径城市1和途径城市2 如果两个途径城市都出现了 则随机路径成功，跳出循环
                    if (result[i][j] == user.city_pass[0]) {
                        flag1 = 1;
                    }
                    if (result[i][j] == user.city_pass[1]) {
                        flag2 = 1;
                    }
                    if (flag1 == 1 && flag2 == 1) {
                        break;
                    }
                }
                if (flag1 == 1 && flag2 == 1) {
                    break;
                }
            }
        }
    }

    for (i = 1; i <= te_; i++) {//除去20条路径中重复的路径
        for (j = 1; j <= i - 1; j++) {
            int flag = 0;
            if (result[j][0] == result[i][0]) {
                //如果路径i与路径j完全相同，则flag保持为0 如果两条路径不完全相同 则将flag置1
                for (k = 1; k <= result[j][0]; k++) {
                    if (result[j][k] != result[i][k]) {
                        flag = 1;
                        break;
                    }
                }
            }
            else {
                flag = 1;
            }
            if (flag == 0) {//如果有与路径i相同的路径，则去除路径i
                result[i][0] = 0;
                break;
            }
        }
    }
    for (i = 1; i <= te_; i++) {//将20条路径中互不重复的路径写入final_result中
        if (result[i][0] != 0) {
            t_amount++;//记录互不重复的路径条数
            final_result[t_amount][0] = result[i][0];
            for (j = 1; j <= result[i][0]; j++) {
                final_result[t_amount][j] = result[i][j];
            }
        }
    }
    for (i = 1; i <= t_amount; i++) {//将费用时间等初始化
        temp_trans[i][0] = final_result[i][0] - 1;
        trans[i][0] = temp_trans[i][0];
        arrive[i] = maxint;
        fees[i] = maxint;
    }
    find_path_num();//将从一个城市到另一个城市的所有车次在数组中存起来
    for (i = 1; i <= t_amount; i++) {//计算每条路径在限定时间内的最小费用
        find_min_fee(i, 1, 1, limit_time);
    }
    int final_fees = maxint;
    int rem;
    int judge_flag=0;
    for(i=1;i<=t_amount;i++){//判断限制时间内是否有有效路径，如果有，则judge_flag置1,并选出费用最少的一条
            if(fees[i]!=maxint&&arrive[i]>0&&arrive[i]<=limit_time){
                if(fees[i]<final_fees){//不断寻找费用最小的路线
                    rem=i;
                    final_fees=fees[i];
                }
                judge_flag=1;
            }
        }
    if(judge_flag==0){//如果没有有效路径，则弹出提示窗口
        flag_strategy3=1;
    }
    else{//如果有有效路径，则将最优解存入旅客信息中
        flag_strategy3=0;
        for (i = 0; i <= trans[rem][0]; i++) {
            user.path[i] = trans[rem][i];
        }
    }
}
