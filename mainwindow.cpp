#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,sjdrInputDock(NULL)
    ,sjdrInputWidget(NULL)
    ,sjdrQualityDock(NULL)
    ,sjdrQualityWidget(NULL)
    ,sjdrMainWidget(NULL)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

MainWindow::~MainWindow()
{
    delete sjdrAction;
    delete kfttjAction;
    delete rckqAction;
    delete fmgAction;

    delete viewMenu;

    if(sjdrInputDock != NULL){
        delete sjdrInputDock;
    }
    if(sjdrQualityDock != NULL){
        delete sjdrQualityDock;
    }
    if(sjdrMainWidget != NULL){
        delete sjdrMainWidget;
    }
}

void MainWindow::initData(){
}

void MainWindow::initUI(){
    this->setupModuleActions();
    this->setupViewActions();
    this->setupSettingActions();
    this->setupHelpActions();
    this->createStatusBar();
}

void MainWindow::initConnect(){
    connect(sjdrAction, SIGNAL(triggered()), this, SLOT(onSjdrTriggered()));
}

void MainWindow::setupModuleActions(){
    QMenu *menu = new QMenu("模块", this);
    this->menuBar()->addMenu(menu);
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setWindowTitle("模块");
    this->addToolBar(toolBar);

    //数据导入模块
    QIcon sjdrIcon = QIcon(":/images/sjdr.png");
    sjdrAction = new QAction(sjdrIcon, "数据导入", this);
    menu->addAction(sjdrAction);
    toolBar->addAction(sjdrAction);

    //可飞天统计模块
    QIcon kfttjIcon = QIcon(":/images/kfttj.png");
    kfttjAction = new QAction(kfttjIcon, "可飞天统计", this);
    menu->addAction(kfttjAction);
    toolBar->addAction(kfttjAction);

    //日窗口期模块
    QIcon rckqIcon = QIcon(":/images/rckq.png");
    rckqAction = new QAction(rckqIcon, "日窗口期", this);
    menu->addAction(rckqAction);
    toolBar->addAction(rckqAction);

    //风玫瑰模块
    QIcon fmgIcon = QIcon(":/images/fmg.png");
    fmgAction = new QAction(fmgIcon, "风玫瑰", this);
    menu->addAction(fmgAction);
    toolBar->addAction(fmgAction);
}

void MainWindow::setupViewActions(){
    viewMenu = new QMenu("视图", this);
    this->menuBar()->addMenu(viewMenu);
}

void MainWindow::setupSettingActions(){
    QMenu *menu = new QMenu("设置", this);
    this->menuBar()->addMenu(menu);
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setWindowTitle("设置");
    this->addToolBar(toolBar);
}

void MainWindow::setupHelpActions(){
    QMenu *menu = new QMenu("帮助", this);
    this->menuBar()->addMenu(menu);
}

void MainWindow::createStatusBar(){
    this->statusBar()->showMessage(tr("Ready"));
}


/**
 * @brief MainWindow::onSjdrTriggered
 * 数据导入模块
 */
void MainWindow::onSjdrTriggered(){
    if(sjdrInputDock == NULL){
        sjdrInputDock = new QDockWidget("输入控制", this);
        sjdrInputDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        this->addDockWidget(Qt::LeftDockWidgetArea, sjdrInputDock);
        this->viewMenu->addAction(sjdrInputDock->toggleViewAction());
        this->setupSjdr();
    }else{
        sjdrInputDock->setVisible(true);
    }

    if(sjdrQualityDock == NULL){
        sjdrQualityDock = new QDockWidget("质量控制", this);
        sjdrQualityDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        this->addDockWidget(Qt::LeftDockWidgetArea, sjdrQualityDock);
        this->viewMenu->addAction(sjdrQualityDock->toggleViewAction());
    }else{
        sjdrQualityDock->setVisible(true);
    }
}

void MainWindow::setupSjdr(){
    this->setupSjdrInputWidget();
    this->setupSjdrQualityWidget();
    this->setupSjdrResultWidget();
    connect(sjdrInputWidget, SIGNAL(executeSjdr(Airport,QList<QualityControlSource>,QList<QString>)), sjdrMainWidget, SLOT(executeSjdr(Airport,QList<QualityControlSource>,QList<QString>)));
}

void MainWindow::setupSjdrInputWidget(){
    if(sjdrInputWidget != NULL){
        delete sjdrInputWidget;
    }
    sjdrInputWidget = new SjdrInputWidget;
    sjdrInputDock->setWidget(sjdrInputWidget);
}

void MainWindow::setupSjdrQualityWidget(){

}

void MainWindow::setupSjdrResultWidget(){
    if(sjdrMainWidget != NULL){
        delete sjdrMainWidget;
    }
    sjdrMainWidget = new SjdrMainWidget;
    this->setCentralWidget(sjdrMainWidget);
}
