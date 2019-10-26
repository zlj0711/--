#pragma once
#include <QMainWindow>
#include "common.h"
#include "routequery.h"
#include "routechange.h"
#include "statequery.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void dealrq2main();
    void dealrc2main();
    void dealsq2main();

private slots:
    void on_route_query_clicked();
    void on_route_change_clicked();
    void on_state_query_clicked();

private:
    Ui::MainWindow *ui;
    routequery routequery_widget;
    routechange routechange_widget;
    statequery statequery_widget;
};
