#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"
#include "bean/qualitycontrolsource.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initData();
    void initUI();
    void initConnect();
    //菜单栏及工具栏
    void setupModuleActions();
    void setupViewActions();
    void setupSettingActions();
    void setupHelpActions();
    //状态栏
    void createStatusBar();
    //查询机场表
    void queryAirport();
    //查询数据源
    void querySource();
    //设置数据导入模块的输入控制区
    void setupSjdrInputWidget();
    //设置数据导入模块的质量控制区
    void setupSjdrQualityWidget();

private slots:
    void onSjdrTriggered();

private:
    PgDataBase *pgdb;
    QList<Airport> aiportList;
    QList<QualityControlSource> qualityControlSourceList;

    QMenu *viewMenu;

    QAction *sjdrAction;
    QAction *kfttjAction;
    QAction *rckqAction;
    QAction *fmgAction;

    QDockWidget *sjdrInputDock;
    QWidget *sjdrInputWidget;
    QDockWidget *sjdrQualityDock;
    QWidget *sjdrQualityWidget;
};

#endif // MAINWINDOW_H
