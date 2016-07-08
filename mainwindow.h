#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "sjdrinputwidget.h"
#include "sjdrqualitywidget.h"
#include "sjdrmainwidget.h"
#include "kfttjinputwidget.h"
#include "kfttjresultwidget.h"
#include "fmginputwidget.h"
#include "fmgresultwidget.h"
#include "rckqinputwidget.h"
#include "rckqresultwidget.h"
#include "airportsetupwidget.h"
#include "weatherparamsetupwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setProgressValue(int value, int maxValue = 100);

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
    //设置可飞天统计模块的输入控制区
    void setupKfttjInputWidget();
    //设置可飞天统计模块的结果区
    void setupKfttjResultWidget();
    //设置风玫瑰模块区
    void setupFmg();
    //设置风玫瑰模块的输入控制区
    void setupFmgInputWidget();
    //设置风玫瑰模块的结果区
    void setupFmgResultWidget();
    //设置日窗口期模块区
    void setupRckq();
    //设置日窗口期模块的输入控制区
    void setupRckqInputWidget();
    //设置日窗口期模块的结果区
    void setupRckqResultWidget();

private slots:
    void onSjdrTriggered();
    void onKfttjTriggered();
    void onFmgTriggered();
    void onRckqTriggered();
    void onAirportSetupTriggered();
    void onWeatherSetupTriggered();

private:
    bool isSjdrInit;
    bool isKfttjInit;
    bool isFmgInit;
    bool isRckqInit;

    QStackedWidget *resultWidget;

    QMenu *viewMenu;

    QAction *sjdrAction;
    QAction *kfttjAction;
    QAction *rckqAction;
    QAction *fmgAction;
    QAction *airportAction;
    QAction *weatherAction;

    QProgressBar *progressBar;

    QDockWidget *sjdrInputDock;
    SjdrInputWidget *sjdrInputWidget;
    QDockWidget *sjdrQualityDock;
    SjdrQualityWidget *sjdrQualityWidget;
    SjdrMainWidget *sjdrMainWidget;

    QDockWidget *kfttjInputDock;
    KfttjInputWidget *kfttjInputWidget;
    KfttjResultWidget *kfttjResultWidget;

    QDockWidget *fmgInputDock;
    FmgInputWidget *fmgInputWidget;
    FmgResultWidget *fmgResultWidget;

    QDockWidget *rckqInputDock;
    RckqInputWidget *rckqInputWidget;
    RckqResultWidget *rckqResultWidget;

};

#endif // MAINWINDOW_H
