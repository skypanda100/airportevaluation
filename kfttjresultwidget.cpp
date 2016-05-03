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
//    for (int column = 2; column < tableModel->columnCount() - 3; ++column){
//        tableView->resizeColumnToContents(column);
////        tableView->setColumnWidth(column, 25);
//    }
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
    }
}
