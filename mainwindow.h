#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "sjdrinputwidget.h"
#include "sjdrmainwidget.h"
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

private slots:
    void onSjdrTriggered();

private:
    QMenu *viewMenu;

    QAction *sjdrAction;
    QAction *kfttjAction;
    QAction *rckqAction;
    QAction *fmgAction;

    QProgressBar *progressBar;

    QDockWidget *sjdrInputDock;
    SjdrInputWidget *sjdrInputWidget;
    QDockWidget *sjdrQualityDock;
    QWidget *sjdrQualityWidget;
    SjdrMainWidget *sjdrMainWidget;
};

#endif // MAINWINDOW_H
