#pragma once
#include <QWidget>
#include "common.h"
#include "declare.h"
#include "conversion.h"
#include "timer.h"
#include "path_design.h"
#include "update_dairy.h"


namespace Ui {
class statequery;
}

class statequery : public QWidget
{
    Q_OBJECT

public:
    explicit statequery(QWidget *parent = nullptr);
    ~statequery();

    QString getroute(int num);

signals:
    void backtomain();

private slots:
    void on_back_clicked();
    void on_search_clicked();
    void on_id_textChanged(const QString &arg1);

private:
    Ui::statequery *ui;
};


class PaintLabel:public QLabel
{
    Q_OBJECT

public:
    explicit PaintLabel(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    ~PaintLabel();

private:
    QTimer * paintmstimer;

};


void state_build(int num);
void state_query(int num);
void print_state(int num);
void state_query4map(int num);
QPointF getbeginPos();
QPointF setpointPos();
QPointF getmoveDistance();
