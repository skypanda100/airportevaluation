#include "rckqresultwidget.h"

RckqResultWidget::RckqResultWidget(QWidget *parent)
    :QSplitter(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

RckqResultWidget::~RckqResultWidget(){
    delete tableModel;
    delete tableView;
    delete imageWidget;
    delete imageArea;
    if(rckqControl != NULL){
        if(rckqControl->isRunning()){
            rckqControl->quit();
            rckqControl->wait();
        }
        delete rckqControl;
    }
}

void RckqResultWidget::initData(){
    qRegisterMetaType< QList<QString> >("QList<QString>");

    titleList << "气象要素"
              << "日期";
    for(int i = 0;i < 60;i++){
        titleList.append(QString("%1m").arg(i));
    }

    rckqControl = new RckqControl;
}

void RckqResultWidget::initUI(){
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
    //调整列宽(60分)
    for(int i = 0;i < 60;i++){
        tableView->setColumnWidth(i + 2, 35);
    }

    //统计图
    imageArea = new QScrollArea;
    QPalette imagePalette;
    imagePalette.setColor(QPalette::Background, QColor(100, 100, 100));
    imageArea->setAutoFillBackground(true);
    imageArea->setPalette(imagePalette);
    imageArea->setContentsMargins(5, 5, 5, 5);
    imageWidget = new QWidget;


    //纵向布局
    this->setOrientation(Qt::Vertical);
    //添加控件
    this->addWidget(tableView);

}

void RckqResultWidget::initConnect(){
    connect(rckqControl
            , SIGNAL(sendMessage(int,QString,QString,QList<QString>))
            , this
            , SLOT(receiveMessage(int,QString,QString,QList<QString>))
            , Qt::QueuedConnection);
}

void RckqResultWidget::executeRckq(QString code, QString runway, int type, int fhour, int thour, QList<QString> dateList, QList<QString> weatherList){
    int rowCount = tableModel->rowCount();
    for(int i = rowCount - 1;i >= 0;i--){
        tableModel->removeRow(i);
    }
    rckqControl->setData(code, runway, type, fhour, thour, dateList, weatherList);
    rckqControl->start();
}

void RckqResultWidget::receiveMessage(int row, QString weather, QString dateStr, QList<QString> valueList){
    tableModel->insertRows(row, 1, QModelIndex());
    tableModel->setData(tableModel->index(row, 0, QModelIndex()), weather);
    tableModel->setData(tableModel->index(row, 1, QModelIndex()), dateStr);
    int valueCount = valueList.size();
    for(int i = 0;i < valueCount;i++){
        tableModel->setData(tableModel->index(row, 2 + i, QModelIndex()), valueList[i]);
    }
}
