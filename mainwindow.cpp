#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,isSjdrInit(false)
    ,isKfttjInit(false)
    ,isFmgInit(false)
    ,resultWidget(NULL)
    ,sjdrInputDock(NULL)
    ,sjdrInputWidget(NULL)
    ,sjdrQualityDock(NULL)
    ,sjdrQualityWidget(NULL)
    ,sjdrMainWidget(NULL)
    ,progressBar(NULL)
    ,kfttjResultWidget(NULL)
    ,fmgInputDock(NULL)
    ,fmgInputWidget(NULL)
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

    if(progressBar != NULL){
        delete progressBar;
    }

    if(kfttjResultWidget != NULL){
        delete kfttjResultWidget;
    }

    if(resultWidget != NULL){
        delete resultWidget;
    }

    if(fmgInputDock != NULL){
        delete fmgInputDock;
    }
}

void MainWindow::initData(){
}

void MainWindow::initUI(){
    this->setupModuleActions();
    this->setupViewActions();
    this->setupSettingActions();
    this->setupHelpActions();
    this->setupCentralWidget();
    this->createStatusBar();
}

void MainWindow::initConnect(){
    connect(sjdrAction, SIGNAL(triggered()), this, SLOT(onSjdrTriggered()));
    connect(kfttjAction, SIGNAL(triggered()), this, SLOT(onKfttjTriggered()));
    connect(fmgAction, SIGNAL(triggered()), this, SLOT(onFmgTriggered()));
}

void MainWindow::setupCentralWidget(){
    resultWidget = new QStackedWidget;
    this->setCentralWidget(resultWidget);
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
    progressBar = new QProgressBar;
    progressBar->setTextVisible(false);
    progressBar->setFixedHeight(18);
    progressBar->setRange(0, 100);
    this->statusBar()->addPermanentWidget(progressBar);
}

void MainWindow::setProgressValue(int value){
    if(progressBar != NULL){
        progressBar->setValue(value);
        if(value < 100){
            this->statusBar()->showMessage(tr("正在处理......"));
        }else{
            this->statusBar()->showMessage(tr("处理完成"));
        }
    }
}

/**
 * @brief MainWindow::onSjdrTriggered
 * 数据导入模块
 */
void MainWindow::onSjdrTriggered(){
    if(!isSjdrInit){
        isSjdrInit = true;
        sjdrInputDock = new QDockWidget("数据导入输入控制", this);
        sjdrInputDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        this->addDockWidget(Qt::LeftDockWidgetArea, sjdrInputDock);
        this->viewMenu->addAction(sjdrInputDock->toggleViewAction());

        sjdrQualityDock = new QDockWidget("数据导入质量控制", this);
        sjdrQualityDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        this->addDockWidget(Qt::LeftDockWidgetArea, sjdrQualityDock);
        this->viewMenu->addAction(sjdrQualityDock->toggleViewAction());

        this->setupSjdr();
    }else{
        sjdrInputDock->setVisible(true);
        this->viewMenu->addAction(sjdrInputDock->toggleViewAction());
        sjdrQualityDock->setVisible(true);
        this->viewMenu->addAction(sjdrQualityDock->toggleViewAction());
        resultWidget->setCurrentWidget(sjdrMainWidget);
    }
    //去除其他模块
    if(isFmgInit){
        this->viewMenu->removeAction(fmgInputDock->toggleViewAction());
        fmgInputDock->setVisible(false);
    }
}

/**
 * @brief MainWindow::onKfttjTriggered
 * 可飞天统计模块
 */
void MainWindow::onKfttjTriggered(){
    if(!isKfttjInit){
        isKfttjInit = true;
        this->setupKfttj();
    }else{
        resultWidget->setCurrentWidget(kfttjResultWidget);
    }
}

/**
 * @brief MainWindow::onFmgTriggered
 * 风玫瑰模块
 */
void MainWindow::onFmgTriggered(){
    if(!isFmgInit){
        isFmgInit = true;
        fmgInputDock = new QDockWidget("风玫瑰输入控制", this);
        fmgInputDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        this->addDockWidget(Qt::LeftDockWidgetArea, fmgInputDock);
        this->viewMenu->addAction(fmgInputDock->toggleViewAction());

        this->setupFmg();
    }else{
        fmgInputDock->setVisible(true);
        this->viewMenu->addAction(fmgInputDock->toggleViewAction());
    }
    //去除其他模块
    if(isSjdrInit){
        this->viewMenu->removeAction(sjdrInputDock->toggleViewAction());
        this->viewMenu->removeAction(sjdrQualityDock->toggleViewAction());
        sjdrInputDock->setVisible(false);
        sjdrQualityDock->setVisible(false);
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
    if(sjdrQualityWidget != NULL){
        delete sjdrQualityWidget;
    }
    sjdrQualityWidget = new SjdrQualityWidget;
    sjdrQualityDock->setWidget(sjdrQualityWidget);
}

void MainWindow::setupSjdrResultWidget(){
    if(sjdrMainWidget != NULL){
        delete sjdrMainWidget;
    }
    sjdrMainWidget = new SjdrMainWidget;
    connect(sjdrMainWidget, SIGNAL(setProgressValue(int)), this, SLOT(setProgressValue(int)));
    resultWidget->addWidget(sjdrMainWidget);
    resultWidget->setCurrentWidget(sjdrMainWidget);
}

void MainWindow::setupKfttj(){
    this->setupKfttjResultWidget();
}

void MainWindow::setupKfttjResultWidget(){
    if(kfttjResultWidget != NULL){
        delete kfttjResultWidget;
    }
    kfttjResultWidget = new KfttjResultWidget;
    connect(kfttjResultWidget, SIGNAL(setProgressValue(int)), this, SLOT(setProgressValue(int)));
    resultWidget->addWidget(kfttjResultWidget);
    resultWidget->setCurrentWidget(kfttjResultWidget);
}

void MainWindow::setupFmg(){
    this->setupFmgInputWidget();
}

void MainWindow::setupFmgInputWidget(){
    if(fmgInputWidget != NULL){
        delete fmgInputWidget;
    }
    fmgInputWidget = new FmgInputWidget;
    fmgInputDock->setWidget(fmgInputWidget);
}
