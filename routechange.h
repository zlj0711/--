#pragma once
#include <QWidget>
#include "common.h"
#include "declare.h"
#include "statequery.h"
#include "routequery.h"
#include "update_dairy.h"


extern int flag_strategy3;


namespace Ui {
class routechange;
}

class routechange : public QWidget
{
    Q_OBJECT

public:
    explicit routechange(QWidget *parent = nullptr);
    ~routechange();
    void print_route();

signals:
    void backtomain();

private slots:
    void on_back_clicked();
    void on_id_textChanged(const QString &arg1);
    void on_strategy_currentIndexChanged(const QString &arg1);
    void on_search_clicked();
    void on_go_clicked();

private:
    Ui::routechange *ui;
};
