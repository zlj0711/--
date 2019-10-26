#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(500,400);
    this->setStyleSheet("background-color:rgb(255,255,251)");
    setStyleSheet("QPushButton{"
                               "background-color:rgb(189,192,186);"    //背景色（也可以设置图片）
                               "border-style:outset;"                  //边框样式（inset/outset）
                               "border-width:2px;"                     //边框宽度像素
                               "border-radius:8px;"                    //边框圆角半径像素
                               "border-color:rgb(189,192,186);"        //边框颜色
                               "font:bold 20px;"                       //字体，字体大小
                               "color:rgb(255,255,251);"               //字体颜色
                               "padding:6px;"                          //填衬
                               "}"
                               //鼠标按下样式
                               "QPushButton:pressed{"
                               "background-color:rgb(145,152,159);"
                               "border-color:rgb(189,192,186);"
                               "border-style:inset;"
                               "color:rgb(255,255,251);"
                               "}"
                               //鼠标悬停样式
                               "QPushButton:hover{"
                               "background-color:rgb(145,152,159);"
                               "border-color:rgb(145,152,159);"
                               "color:rgb(255,255,251);"
                               "}");

    //返回
    connect(&routequery_widget,&routequery::backtomain,this,&MainWindow::dealrq2main);
    connect(&routechange_widget,&routechange::backtomain,this,&MainWindow::dealrc2main);
    connect(&statequery_widget,&statequery::backtomain,this,&MainWindow::dealsq2main);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//打开路线查询
void MainWindow::on_route_query_clicked()
{
    this->hide();
    routequery_widget.show();
}

//打开路线更改
void MainWindow::on_route_change_clicked()
{
    this->hide();
    routechange_widget.show();
}

//打开状态查询
void MainWindow::on_state_query_clicked()
{
    this->hide();
    statequery_widget.show();
}


//路线查询返回
void MainWindow::dealrq2main(){
    this->show();
    routequery_widget.close();
}

//路线更改返回
void MainWindow::dealrc2main(){
    this->show();
    routechange_widget.close();
}

//状态查询返回
void MainWindow::dealsq2main(){
    this->show();
    statequery_widget.close();
}
