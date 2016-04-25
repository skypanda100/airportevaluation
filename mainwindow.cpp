#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,sjdrInputDock(NULL)
    ,sjdrInputWidget(NULL)
    ,sjdrQualityDock(NULL)
    ,sjdrQualityWidget(NULL)
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

    delete pgdb;
}

void MainWindow::initData(){
    pgdb = new PgDataBase;
}

void MainWindow::initUI(){
    this->setupModuleActions();
    this->setupViewActions();
    this->setupSettingActions();
    this->setupHelpActions();
    this->createStatusBar();

    //测试
    QWidget *mainWidget = new QWidget;
    mainWidget->setAutoFillBackground(true);
    QPalette palette(QColor(80, 80, 80));
    mainWidget->setPalette(palette);
    this->setCentralWidget(mainWidget);
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
        this->setupSjdrInputWidget();
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

void MainWindow::setupSjdrInputWidget(){
    //机场
    queryAirport();
    QComboBox *airportComboBox = new QComboBox;
    int airportCount = aiportList.size();
    for(int i = 0;i < airportCount;i++){
        airportComboBox->insertItem(i, aiportList[i].name());
    }

    QVBoxLayout *airportLayout = new QVBoxLayout;
    airportLayout->addWidget(airportComboBox);

    QGroupBox *airportGroup = new QGroupBox;
    airportGroup->setTitle("机场");
    airportGroup->setLayout(airportLayout);

    //数据源
    querySource();

    QGridLayout *sourceLayout = new QGridLayout;
    int sourceCount = qualityControlSourceList.size();
    for(int i = 0;i < sourceCount;i++){
        QCheckBox *checkBox = new QCheckBox(qualityControlSourceList[i].name());
        sourceLayout->addWidget(checkBox, i / 2 , i % 2);
    }

    QGroupBox *sourceGroup = new QGroupBox;
    sourceGroup->setTitle("数据源");
    sourceGroup->setLayout(sourceLayout);

    //主界面
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(airportGroup);
    mainLayout->addWidget(sourceGroup);
    mainLayout->addStretch(1);
    if(sjdrInputWidget != NULL){
        delete sjdrInputWidget;
    }
    sjdrInputWidget = new QWidget;
    sjdrInputWidget->setLayout(mainLayout);
    sjdrInputDock->setWidget(sjdrInputWidget);
}

void MainWindow::setupSjdrQualityWidget(){

}

void MainWindow::queryAirport(){
    aiportList.clear();
    QString queryStr = QString("select * from airport");
    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        Airport airport;
        airport.setCode(plainModel->record(i).value(0).toString());
        airport.setName(plainModel->record(i).value(1).toString());
        airport.setLongitude(plainModel->record(i).value(2).toFloat());
        airport.setLatitude(plainModel->record(i).value(3).toFloat());
        airport.setAltitude(plainModel->record(i).value(4).toFloat());
        airport.setDirection(plainModel->record(i).value(5).toFloat());
        airport.setType(plainModel->record(i).value(6).toString());
        aiportList.append(airport);
    }
    delete plainModel;
}

void MainWindow::querySource(){
    qualityControlSourceList.clear();
    QString queryStr = QString("select * from qualitycontrolsource");
    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        QualityControlSource qualityControlSource;
        qualityControlSource.setId(plainModel->record(i).value(0).toInt());
        qualityControlSource.setName(plainModel->record(i).value(1).toString());
        qualityControlSource.setFields(plainModel->record(i).value(2).toString());
        qualityControlSourceList.append(qualityControlSource);
    }
    delete plainModel;
}
