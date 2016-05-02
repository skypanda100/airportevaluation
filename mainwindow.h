#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "sjdrinputwidget.h"
#include "sjdrqualitywidget.h"
#include "sjdrmainwidget.h"
#include "kfttjresultwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setProgressValue(int);

private:
    void initData();
    void initUI();
    void initConnect();
    //菜单栏及工具栏
    void setupModuleActions();
    void setupViewActions();
    void setupSettingActions();
    void setupHelpActions();
    //中央区域
    void setupCentralWidget();
    //状态栏
    void createStatusBar();
    //设置数据导入模块区
    void setupSjdr();
    //设置数据导入模块的输入控制区
    void setupSjdrInputWidget();
    //设置数据导入模块的质量控制区
    void setupSjdrQualityWidget();
    //设置数据导入模块的结果区
    void setupSjdrResultWidget();
    //设置可飞天统计模块区
    void setupKfttj();
    //设置可飞天统计模块的结果区
    void setupKfttjResultWidget();

private slots:
    void onSjdrTriggered();
    void onKfttjTriggered();

private:
    bool isSjdrInit;
    bool isKfttjInit;

    QStackedWidget *resultWidget;

    QMenu *viewMenu;

    QAction *sjdrAction;
    QAction *kfttjAction;
    QAction *rckqAction;
    QAction *fmgAction;

    QProgressBar *progressBar;

    QDockWidget *sjdrInputDock;
    SjdrInputWidget *sjdrInputWidget;
    QDockWidget *sjdrQualityDock;
    SjdrQualityWidget *sjdrQualityWidget;
    SjdrMainWidget *sjdrMainWidget;

    KfttjResultWidget *kfttjResultWidget;
};

#endif // MAINWINDOW_H
