#include "kfttjresultwidget.h"

KfttjResultWidget::KfttjResultWidget(QWidget *parent)
    :QSplitter(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

KfttjResultWidget::~KfttjResultWidget(){
    delete tableModel;
    delete tableView;
    delete imageWidget;
    delete imageArea;
    if(kfttjControl != NULL){
        if(kfttjControl->isRunning()){
            kfttjControl->quit();
            kfttjControl->wait();
        }
        delete kfttjControl;
    }
}

void KfttjResultWidget::initData(){
    titleList << "日期"
              << "气象要素"
              << "17"
              << "18"
              << "19"
              << "20"
              << "21"
              << "22"
              << "23"
              << "0"
              << "1"
              << "2"
              << "3"
              << "4"
              << "5"
              << "6"
              << "7"
              << "8"
              << "9"
              << "10"
              << "11"
              << "12"
              << "13"
              << "14"
              << "15"
              << "16"
              << "完全可飞"
              << "限制可飞"
              << "不可飞";
    kfttjControl = new KfttjControl;
}

void KfttjResultWidget::initUI(){
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    tableModel = new TableModel(0, titleList.size(), this);
    int titleCount = titleList.size();
    for(int i = 0;i < titleCount;i++){
        tableModel->setHeaderData(i, Qt::Horizontal, titleList[i]);
    }

    tableView = new QTableView;
    tableView->setModel(tableModel);

    this->setOrientation(Qt::Vertical);
    this->addWidget(tableView);
    //调整列宽
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    imageArea = new QScrollArea;
    imageWidget = new QWidget;
    this->addWidget(imageArea);
    this->setStretchFactor(0, 1);
///////////////////////
    //查询月总簿表
    QString summaryStartDatetime = "2012-12-31 17:00:00";
    QString summaryEndDatetime = "2013-12-31 16:00:00";

    QString summarySql = QString("select * from zsdy_monthsummary where datetime >= '%1' and datetime <= '%2' order by datetime")
            .arg(summaryStartDatetime)
            .arg(summaryEndDatetime);

    //查询极值表
    QString extremumStartDatetime = "2013-01-01 00:00:00";
    QString extremumEndDatetime = "2013-12-31 23:59:59";

    QString extremumSql = QString("select * from zsdy_extremum where datetime >= '%1' and datetime <= '%2' order by datetime")
            .arg(extremumStartDatetime)
            .arg(extremumEndDatetime);

    kfttjControl->setSummarySql(summarySql);
    kfttjControl->setExtremumSql(extremumSql);
    kfttjControl->start();
}

void KfttjResultWidget::initConnect(){
    connect(kfttjControl, SIGNAL(sendMessage(int,int)), this, SLOT(receiveMessage(int,int)), Qt::QueuedConnection);
    connect(kfttjControl, SIGNAL(sendMessage(QString,int,int,int,int)), this, SLOT(receiveMessage(QString,int,int,int,int)), Qt::QueuedConnection);
    connect(kfttjControl, SIGNAL(execute(bool)), this, SLOT(execute(bool)), Qt::QueuedConnection);
    connect(kfttjControl, SIGNAL(setProgressValue(int)), this, SIGNAL(setProgressValue(int)), Qt::QueuedConnection);
}

void KfttjResultWidget::receiveMessage(const QString &message, int row, int col, int rows, int cols){
    tableModel->setData(tableModel->index(row, col, QModelIndex()), message);
    if(rows > 1 || cols > 1){
        tableView->setSpan(row, col, rows, cols);
    }
}

void KfttjResultWidget::receiveMessage(int row, int count){
    tableModel->insertRows(row, count, QModelIndex());
}

void KfttjResultWidget::execute(bool isEnd){
    if(isEnd){
        emit setProgressValue(100);
        this->createImages(kfttjControl->getKfttjHash());
    }
}

void KfttjResultWidget::createImages(QHash< QString, QList<float> > kfttjHash){
    //布局
    QHBoxLayout *imageLayout = new QHBoxLayout;
    imageLayout->setSpacing(5);
    //key值
    QList<QString> keyList = kfttjHash.keys();
    int keyCount = keyList.size();
    /***年可飞天统计:采用饼图（要素为完全可飞，限制可飞，不可飞），可以包含几年的数据。***/
    QChartViewer *pieChartView1 = new QChartViewer;
    double datas1[] = {0, 0, 0};
    const char *labels1[] = {"完全可飞", "限制可飞", "不可飞"};
    int colors1[] = {0x329600, 0xffb400, 0xdc3200};
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QList<float> value = kfttjHash[key];
        datas1[0] += value[0];
        datas1[1] += value[1];
        datas1[2] += value[2];
    }
    PieChart *pieChart1 = new PieChart(300, 300, 0xccc1da);
    pieChart1->setDefaultFonts("msyh.ttf");
    pieChart1->addTitle("年可飞天统计");
    pieChart1->setPieSize(150, 150, 130);
    pieChart1->setLabelPos(-40);
    pieChart1->setData(DoubleArray(datas1, (int)(sizeof(datas1) / sizeof(datas1[0]))), StringArray(labels1, (int)(
        sizeof(labels1) / sizeof(labels1[0]))));
    pieChart1->setColors(Chart::DataColor, IntArray(colors1, (int)(sizeof(colors1) / sizeof(colors1[0]))));
    pieChart1->setSectorStyle(Chart::LocalGradientShading, 0xbb000000, 1);
    pieChart1->makeChart();
    pieChartView1->setChart(pieChart1);
    imageLayout->addWidget(pieChartView1);








    //设置滚动区域的widget
    imageWidget->setLayout(imageLayout);
    imageArea->setWidget(imageWidget);
}
