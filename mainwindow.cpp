#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,isSjdrInit(false)
    ,isKfttjInit(false)
    ,isFmgInit(false)
    ,isRckqInit(false)
    ,welcomeWidget(NULL)
    ,guide_airportSetupWidget(NULL)
    ,guide_weatherParamSetupWidget(NULL)
    ,resultWidget(NULL)
    ,sjdrInputDock(NULL)
    ,sjdrInputWidget(NULL)
    ,sjdrQualityDock(NULL)
    ,sjdrQualityWidget(NULL)
    ,sjdrMainWidget(NULL)
    ,progressBar(NULL)
    ,kfttjInputDock(NULL)
    ,kfttjInputWidget(NULL)
    ,kfttjResultWidget(NULL)
    ,fmgInputDock(NULL)
    ,fmgInputWidget(NULL)
    ,fmgResultWidget(NULL)
    ,rckqInputDock(NULL)
    ,rckqInputWidget(NULL)
    ,rckqResultWidget(NULL)
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
    delete airportAction;
    delete weatherAction;
    delete helpAction;
    delete aboutAction;

    delete viewMenu;

    if(welcomeWidget != NULL){
        delete welcomeWidget;
    }
    if(guide_airportSetupWidget != NULL){
        delete guide_airportSetupWidget;
    }
    if(guide_weatherParamSetupWidget != NULL){
        delete guide_weatherParamSetupWidget;
    }

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

    if(kfttjInputDock != NULL){
        delete kfttjInputDock;
    }

    if(kfttjResultWidget != NULL){
        delete kfttjResultWidget;
    }

    if(fmgInputDock != NULL){
        delete fmgInputDock;
    }

    if(fmgResultWidget != NULL){
        delete fmgResultWidget;
    }

    if(rckqInputDock != NULL){
        delete rckqInputDock;
    }

    if(rckqResultWidget != NULL){
        delete rckqResultWidget;
    }

    //最后析构
    if(resultWidget != NULL){
        delete resultWidget;
    }
}

void MainWindow::showGuide(){
    guide_airportSetupWidget = new AirportSetupWidget;
    guide_airportSetupWidget->setWindowFlags((guide_airportSetupWidget->windowFlags()
                                              & ~Qt::WindowCloseButtonHint)
                                             /*| Qt::WindowStaysOnTopHint*/);

    connect(guide_airportSetupWidget, SIGNAL(nextClicked()), this, SLOT(onNextClicked()));

    guide_airportSetupWidget->show();
}

void MainWindow::initData(){
}

void MainWindow::initUI(){
    //设置各个功能模块
    this->setWindowTitle("民机试飞气象条件统计与评价研究系统");
    this->setupModuleActions();
    this->setupViewActions();
    this->setupSettingActions();
    this->setupHelpActions();
    this->setupCentralWidget();
    this->createStatusBar();
    //设置是否活性
    this->setupActivity();
}

void MainWindow::initConnect(){
    connect(sjdrAction, SIGNAL(triggered()), this, SLOT(onSjdrTriggered()));
    connect(kfttjAction, SIGNAL(triggered()), this, SLOT(onKfttjTriggered()));
    connect(fmgAction, SIGNAL(triggered()), this, SLOT(onFmgTriggered()));
    connect(rckqAction, SIGNAL(triggered()), this, SLOT(onRckqTriggered()));
    connect(airportAction, SIGNAL(triggered()), this, SLOT(onAirportSetupTriggered()));
    connect(weatherAction, SIGNAL(triggered()), this, SLOT(onWeatherSetupTriggered()));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(onHelpTriggered()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(onAboutTriggered()));
}

void MainWindow::setupCentralWidget(){
    welcomeWidget = new WelcomeWidget;

    resultWidget = new QStackedWidget;
    resultWidget->addWidget(welcomeWidget);

    this->setCentralWidget(resultWidget);
}

void MainWindow::setupModuleActions(){
    QMenu *menu = new QMenu("模块", this);
    this->menuBar()->addMenu(menu);
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setWindowTitle("模块");
    toolBar->setIconSize(QSize(30, 30));
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
    toolBar->setIconSize(QSize(30, 30));
    this->addToolBar(toolBar);

    //机场设置
    QIcon airportSetupIcon = QIcon(":/images/airport_setup.png");
    airportAction = new QAction(airportSetupIcon, "机场设置", this);
    menu->addAction(airportAction);
    toolBar->addAction(airportAction);

    //气象要素阀值设置
    QIcon weatherSetupIcon = QIcon(":/images/weather_setup.png");
    weatherAction = new QAction(weatherSetupIcon, "阀值设置", this);
    menu->addAction(weatherAction);
    toolBar->addAction(weatherAction);
}

void MainWindow::setupHelpActions(){
    QMenu *menu = new QMenu("帮助", this);
    this->menuBar()->addMenu(menu);

    //帮助内容
    helpAction = new QAction("帮助内容", this);
    menu->addAction(helpAction);

    //关于
    aboutAction = new QAction("关于", this);
//    menu->addAction(aboutAction);
}

void MainWindow::createStatusBar(){
    this->statusBar()->showMessage(tr("Ready"));
    progressBar = new QProgressBar;
    progressBar->setTextVisible(false);
    progressBar->setFixedHeight(18);
    progressBar->setRange(0, 100);
    progressBar->setVisible(false);
    this->statusBar()->addPermanentWidget(progressBar);
}

void MainWindow::setProgressValue(int value, int maxValue){
    if(progressBar != NULL){
        if(progressBar->maximum() != maxValue){
            progressBar->setRange(0, maxValue);
        }
        progressBar->setValue(value);
        if(value < progressBar->maximum()){
            this->statusBar()->showMessage(tr("正在处理......"));
        }else{
            this->statusBar()->showMessage(tr("处理完成"));
        }
        if(value <= 0){
            progressBar->setVisible(false);
        }else if(value > 0 && value < progressBar->maximum()){
            progressBar->setVisible(true);
        }else{
            progressBar->setVisible(false);
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
    if(isKfttjInit){
        this->viewMenu->removeAction(kfttjInputDock->toggleViewAction());
        kfttjInputDock->setVisible(false);
    }
    if(isFmgInit){
        this->viewMenu->removeAction(fmgInputDock->toggleViewAction());
        fmgInputDock->setVisible(false);
    }
    if(isRckqInit){
       this->viewMenu->removeAction(rckqInputDock->toggleViewAction());
        rckqInputDock->setVisible(false);
    }
}

/**
 * @brief MainWindow::onKfttjTriggered
 * 可飞天统计模块
 */
void MainWindow::onKfttjTriggered(){
    if(!isKfttjInit){
        isKfttjInit = true;
        kfttjInputDock = new QDockWidget("可飞天统计输入控制", this);
        kfttjInputDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        this->addDockWidget(Qt::LeftDockWidgetArea, kfttjInputDock);
        this->viewMenu->addAction(kfttjInputDock->toggleViewAction());

        this->setupKfttj();
    }else{
        kfttjInputDock->setVisible(true);
        this->viewMenu->addAction(kfttjInputDock->toggleViewAction());
        resultWidget->setCurrentWidget(kfttjResultWidget);
    }
    //去除其他模块
    if(isSjdrInit){
        this->viewMenu->removeAction(sjdrInputDock->toggleViewAction());
        this->viewMenu->removeAction(sjdrQualityDock->toggleViewAction());
        sjdrInputDock->setVisible(false);
        sjdrQualityDock->setVisible(false);
    }
    if(isRckqInit){
        this->viewMenu->removeAction(rckqInputDock->toggleViewAction());
        rckqInputDock->setVisible(false);
    }
    if(isFmgInit){
        this->viewMenu->removeAction(fmgInputDock->toggleViewAction());
        fmgInputDock->setVisible(false);
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
        resultWidget->setCurrentWidget(fmgResultWidget);
    }
    //去除其他模块
    if(isSjdrInit){
        this->viewMenu->removeAction(sjdrInputDock->toggleViewAction());
        this->viewMenu->removeAction(sjdrQualityDock->toggleViewAction());
        sjdrInputDock->setVisible(false);
        sjdrQualityDock->setVisible(false);
    }
    if(isKfttjInit){
        this->viewMenu->removeAction(kfttjInputDock->toggleViewAction());
        kfttjInputDock->setVisible(false);
    }
    if(isRckqInit){
        this->viewMenu->removeAction(rckqInputDock->toggleViewAction());
        rckqInputDock->setVisible(false);
    }
}

/**
 * @brief MainWindow::onRckqTriggered
 * 日窗口期模块
 */
void MainWindow::onRckqTriggered(){
    if(!isRckqInit){
        isRckqInit = true;
        rckqInputDock = new QDockWidget("日窗口期输入控制", this);
        rckqInputDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        this->addDockWidget(Qt::LeftDockWidgetArea, rckqInputDock);
        this->viewMenu->addAction(rckqInputDock->toggleViewAction());

        this->setupRckq();
    }else{
        rckqInputDock->setVisible(true);
        this->viewMenu->addAction(rckqInputDock->toggleViewAction());
        resultWidget->setCurrentWidget(rckqResultWidget);
    }
    //去除其他模块
    if(isSjdrInit){
        this->viewMenu->removeAction(sjdrInputDock->toggleViewAction());
        this->viewMenu->removeAction(sjdrQualityDock->toggleViewAction());
        sjdrInputDock->setVisible(false);
        sjdrQualityDock->setVisible(false);
    }
    if(isKfttjInit){
        this->viewMenu->removeAction(kfttjInputDock->toggleViewAction());
        kfttjInputDock->setVisible(false);
    }
    if(isFmgInit){
        this->viewMenu->removeAction(fmgInputDock->toggleViewAction());
        fmgInputDock->setVisible(false);
    }
}

/**
 * @brief MainWindow::onAirportSetupTriggered
 * 机场设置模块
 */
void MainWindow::onAirportSetupTriggered(){
    AirportSetupWidget *airportSetupWidget = new AirportSetupWidget;
    airportSetupWidget->exec();
    delete airportSetupWidget;
}

/**
 * @brief MainWindow::onWeatherSetupTriggered
 * 气象阀值设置模块
 */
void MainWindow::onWeatherSetupTriggered(){
    WeatherParamSetupWidget *weatherParamSetupWidget = new WeatherParamSetupWidget;
    weatherParamSetupWidget->exec();
    delete weatherParamSetupWidget;
}

/**
 * @brief MainWindow::onHelpTriggered
 * 帮助内容模块
 */
void MainWindow::onHelpTriggered(){
    QProcess *helpProcess = new QProcess(this);
    QStringList argument(QApplication::applicationDirPath() + "/help/help.chm");
    helpProcess->start("hh.exe", argument);
}

/**
 * @brief MainWindow::onAboutTriggered
 * 关于模块
 */
void MainWindow::onAboutTriggered(){

}

/**
 * @brief MainWindow::onNextClicked
 * 响应下一步
 */
void MainWindow::onNextClicked(){
    if(this->sender() == guide_airportSetupWidget){
        guide_airportSetupWidget->close();

        guide_weatherParamSetupWidget = new WeatherParamSetupWidget;
        guide_weatherParamSetupWidget->setWindowFlags((guide_weatherParamSetupWidget->windowFlags()
                                                       & ~Qt::WindowCloseButtonHint)
                                                      /*| Qt::WindowStaysOnTopHint*/);
        connect(guide_weatherParamSetupWidget, SIGNAL(nextClicked()), this, SLOT(onNextClicked()));
        connect(guide_weatherParamSetupWidget, SIGNAL(previousClicked()), this, SLOT(onPreviousClicked()));

        guide_weatherParamSetupWidget->show();
    }else{
        delete guide_airportSetupWidget;
        guide_airportSetupWidget = NULL;
        delete guide_weatherParamSetupWidget;
        guide_weatherParamSetupWidget = NULL;
        SharedMemory::isWelcome = false;
        this->setupActivity();
        this->onSjdrTriggered();
        this->show();
    }
}

/**
 * @brief MainWindow::onPreviousClicked
 * 响应上一步
 */
void MainWindow::onPreviousClicked(){
    if(this->sender() == guide_weatherParamSetupWidget){
        guide_weatherParamSetupWidget->close();
        delete guide_weatherParamSetupWidget;
        guide_weatherParamSetupWidget = NULL;

        guide_airportSetupWidget->show();
    }
}

void MainWindow::setupSjdr(){
    this->setupSjdrInputWidget();
    this->setupSjdrQualityWidget();
    this->setupSjdrResultWidget();
    connect(sjdrInputWidget, SIGNAL(executeSjdr(Airport,QList<QualityControlSource>,QList<QString>)), sjdrMainWidget, SLOT(executeSjdr(Airport,QList<QualityControlSource>,QList<QString>)));
    connect(sjdrInputWidget, SIGNAL(stopExecuteSjdr()), sjdrMainWidget, SLOT(stopExecuteSjdr()));
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
    connect(sjdrMainWidget, SIGNAL(setProgressValue(int,int)), this, SLOT(setProgressValue(int,int)));
    resultWidget->addWidget(sjdrMainWidget);
    resultWidget->setCurrentWidget(sjdrMainWidget);
}

void MainWindow::setupKfttj(){
    this->setupKfttjInputWidget();
    this->setupKfttjResultWidget();
    connect(kfttjInputWidget, SIGNAL(executeKfttj(QString,QList<QString>,bool,QList<WeatherParam>)), kfttjResultWidget, SLOT(executeKfttj(QString,QList<QString>,bool,QList<WeatherParam>)));
    connect(kfttjInputWidget
            , SIGNAL(executeExport())
            , kfttjResultWidget
            , SLOT(executeExport()));
}

void MainWindow::setupKfttjInputWidget(){
    if(kfttjInputWidget != NULL){
        delete kfttjInputWidget;
    }
    kfttjInputWidget = new KfttjInputWidget;
    kfttjInputDock->setWidget(kfttjInputWidget);
}

void MainWindow::setupKfttjResultWidget(){
    if(kfttjResultWidget != NULL){
        delete kfttjResultWidget;
    }
    kfttjResultWidget = new KfttjResultWidget;
    connect(kfttjResultWidget, SIGNAL(setProgressValue(int,int)), this, SLOT(setProgressValue(int,int)));
    resultWidget->addWidget(kfttjResultWidget);
    resultWidget->setCurrentWidget(kfttjResultWidget);
}

void MainWindow::setupFmg(){
    this->setupFmgInputWidget();
    this->setupFmgResultWidget();
    connect(fmgInputWidget, SIGNAL(executeFmg(QString,QString,QString,QString,QList<QString>)), fmgResultWidget, SLOT(executeFmg(QString,QString,QString,QString,QList<QString>)));
    connect(fmgInputWidget
            , SIGNAL(executeExport())
            , fmgResultWidget
            , SLOT(executeExport()));
}

void MainWindow::setupFmgInputWidget(){
    if(fmgInputWidget != NULL){
        delete fmgInputWidget;
    }
    fmgInputWidget = new FmgInputWidget;
    fmgInputDock->setWidget(fmgInputWidget);
}

void MainWindow::setupFmgResultWidget(){
    if(fmgResultWidget != NULL){
        delete fmgResultWidget;
    }
    fmgResultWidget = new FmgResultWidget;
    connect(fmgResultWidget, SIGNAL(setProgressValue(int,int)), this, SLOT(setProgressValue(int,int)));
    resultWidget->addWidget(fmgResultWidget);
    resultWidget->setCurrentWidget(fmgResultWidget);
}

void MainWindow::setupRckq(){
    this->setupRckqInputWidget();
    this->setupRckqResultWidget();
    connect(rckqInputWidget
            , SIGNAL(executeRckq(QString,QString,int,int,int,QList<QString>,QList<QString>))
            , rckqResultWidget
            , SLOT(executeRckq(QString,QString,int,int,int,QList<QString>,QList<QString>)));
    connect(rckqInputWidget
            , SIGNAL(executeExport())
            , rckqResultWidget
            , SLOT(executeExport()));
}

void MainWindow::setupRckqInputWidget(){
    if(rckqInputWidget != NULL){
        delete rckqInputWidget;
    }
    rckqInputWidget = new RckqInputWidget;
    rckqInputDock->setWidget(rckqInputWidget);
}

void MainWindow::setupRckqResultWidget(){
    if(rckqResultWidget != NULL){
        delete rckqResultWidget;
    }
    rckqResultWidget = new RckqResultWidget;
    connect(rckqResultWidget, SIGNAL(setProgressValue(int,int)), this, SLOT(setProgressValue(int,int)));
    resultWidget->addWidget(rckqResultWidget);
    resultWidget->setCurrentWidget(rckqResultWidget);

}

void MainWindow::setupActivity(){
    bool isDisabled = false;
    if(SharedMemory::isWelcome){
        isDisabled = true;
    }
    sjdrAction->setDisabled(isDisabled);
    kfttjAction->setDisabled(isDisabled);
    rckqAction->setDisabled(isDisabled);
    fmgAction->setDisabled(isDisabled);
    airportAction->setDisabled(isDisabled);
    weatherAction->setDisabled(isDisabled);
}
