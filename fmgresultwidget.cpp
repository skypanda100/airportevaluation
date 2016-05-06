#include "fmgresultwidget.h"
FmgResultWidget::FmgResultWidget(QWidget *parent)
    :QSplitter(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

FmgResultWidget::~FmgResultWidget(){
    delete tableModel;
    delete tableView;
    if(fmgControl != NULL){
        if(fmgControl->isRunning()){
            fmgControl->quit();
            fmgControl->wait();
        }
        delete fmgControl;
    }
}

void FmgResultWidget::initData(){
    qRegisterMetaType< QList<QString> >("QList<QString>");

    titleList << "年"
              << "月";
    for(int i = 1;i <= 360;i++){
        titleList.append(QString("%1°").arg(i));
    }
    fmgControl = new FmgControl;
}

void FmgResultWidget::initUI(){
    //设置背景色
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    //表格
    tableModel = new QStandardItemModel(0, titleList.size(), this);
    int titleCount = titleList.size();
    for(int i = 0;i < titleCount;i++){
        tableModel->setHeaderData(i, Qt::Horizontal, titleList[i]);
    }
    tableView = new QTableView;
    tableView->setModel(tableModel);
    //调整列宽(360度 + 月)
    for(int i = 0;i < 360 + 1;i++){
        tableView->setColumnWidth(i + 1, 35);
    }
    //纵向布局
    this->setOrientation(Qt::Vertical);
    //添加控件
    this->addWidget(tableView);
}

void FmgResultWidget::initConnect(){
    connect(fmgControl, SIGNAL(sendMessage(int,QList<QString>)), this, SLOT(receiveMessage(int,QList<QString>)), Qt::QueuedConnection);
    connect(fmgControl, SIGNAL(sendMessage(int,QString)), this, SLOT(receiveMessage(int,QString)), Qt::QueuedConnection);
}

void FmgResultWidget::executeFmg(QString code, QString runway, QString fspeed, QString tspeed, QList<QString> years){
    int rowCount = tableModel->rowCount();
    for(int i = rowCount - 1;i >= 0;i--){
        tableModel->removeRow(i);
    }
    fmgControl->setData(code, runway, fspeed, tspeed, years);
    fmgControl->start();
}

void FmgResultWidget::receiveMessage(int row, QString year){
    tableModel->insertRows(row, 12, QModelIndex());
    tableModel->setData(tableModel->index(row, 0, QModelIndex()), QString("%1年").arg(year));
    for(int i = 0;i < 12;i++){
        tableModel->setData(tableModel->index(row + i, 1, QModelIndex()), QString("%1月").arg(i + 1));
    }
    tableView->setSpan(row, 0, 12, 1);
}

void FmgResultWidget::receiveMessage(int row, QList<QString> windCountList){
    int windCount = windCountList.size();
    for(int i = 0;i < windCount;i++){
        tableModel->setData(tableModel->index(row, i + 2, QModelIndex()), windCountList[i]);
    }
}
