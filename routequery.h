#pragma once
#include <QWidget>
#include "common.h"
#include "declare.h"
#include "conversion.h"
#include "statequery.h"
#include "path_design.h"
#include "timer.h"
#include "update_dairy.h"


extern int counter;
extern int flag_strategy3;


namespace Ui {
class routequery;
}

class routequery : public QWidget
{
    Q_OBJECT

public:
    explicit routequery(QWidget *parent = nullptr);
    ~routequery();

    void print_route();

signals:
    void backtomain();

private slots:
    void on_back_clicked();
    void on_search_clicked();
    void on_pass1_currentTextChanged(const QString &arg1);
    void on_strategy_currentTextChanged(const QString &arg1);
    void on_go_clicked();

private:
    Ui::routequery *ui;
};

int judge_input(user_infor user);
