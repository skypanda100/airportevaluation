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
    delete imageWidget;
    delete imageArea;
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
    this->addWidget(imageArea);
    this->setStretchFactor(0, 1);
}

void FmgResultWidget::initConnect(){
    connect(fmgControl, SIGNAL(sendMessage(int,QString,QList<QString>)), this, SLOT(receiveMessage(int,QString,QList<QString>)), Qt::QueuedConnection);
    connect(fmgControl, SIGNAL(sendMessage(int,QString)), this, SLOT(receiveMessage(int,QString)), Qt::QueuedConnection);
    connect(fmgControl, SIGNAL(execute(bool)), this, SLOT(execute(bool)), Qt::QueuedConnection);
    connect(fmgControl, SIGNAL(setProgressValue(int)), this, SIGNAL(setProgressValue(int)));
}

void FmgResultWidget::executeFmg(QString code, QString runway, QString fspeed, QString tspeed, QList<QString> years){
    int rowCount = tableModel->rowCount();
    for(int i = rowCount - 1;i >= 0;i--){
        tableModel->removeRow(i);
    }
    windHash.clear();
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

void FmgResultWidget::receiveMessage(int row, QString year, QList<QString> windCountList){
    int windCount = windCountList.size();
    for(int i = 0;i < windCount;i++){
        tableModel->setData(tableModel->index(row, i + 2, QModelIndex()), windCountList[i]);
    }
    if(windHash.contains(year)){
        QList< QList<QString> > windList = windHash[year];
        windList.append(windCountList);
        windHash[year] = windList;
    }else{
        QList< QList<QString> > windList;
        windList.append(windCountList);
        windHash[year] = windList;
    }
}

void FmgResultWidget::execute(bool isEnd){
    if(isEnd){
        emit setProgressValue(100);
        createCharts();
    }
}

void FmgResultWidget::createCharts(){
    //布局
    QHBoxLayout *imageLayout = new QHBoxLayout;
    imageLayout->setSpacing(5);
    //make chart
    QChartViewer *polarChartView = new QChartViewer;
    QList<QString> keyList = windHash.keys();
    int keyCount = keyList.size();
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QList< QList<QString> > windList = windHash[key];
        double *datas_1 = new double[360];
        double *datas_2 = new double[360];
        double *datas_3 = new double[360];
        double *datas_4 = new double[360];
        double *datas_5 = new double[360];
        double *datas_6 = new double[360];
        double *datas_7 = new double[360];
        double *datas_8 = new double[360];
        double *datas_9 = new double[360];
        double *datas_10 = new double[360];
        double *datas_11 = new double[360];
        double *datas_12 = new double[360];
        double *angles = new double[360];
        //角度
        for(int j = 0;j < 360;j++){
            angles[j] = j + 1;
        }
        //1月数据
        QList<QString> wLst_1 = windList[0];
        for(int j = 0;j < 360;j++){
            datas_1[j] = wLst_1[j].toDouble();
        }
        //2月数据
        QList<QString> wLst_2 = windList[1];
        for(int j = 0;j < 360;j++){
            datas_2[j] = wLst_2[j].toDouble();
        }
        //3月数据
        QList<QString> wLst_3 = windList[2];
        for(int j = 0;j < 360;j++){
            datas_3[j] = wLst_3[j].toDouble();
        }
        //4月数据
        QList<QString> wLst_4 = windList[3];
        for(int j = 0;j < 360;j++){
            datas_4[j] = wLst_4[j].toDouble();
        }
        //5月数据
        QList<QString> wLst_5 = windList[4];
        for(int j = 0;j < 360;j++){
            datas_5[j] = wLst_5[j].toDouble();
        }
        //6月数据
        QList<QString> wLst_6 = windList[5];
        for(int j = 0;j < 360;j++){
            datas_6[j] = wLst_6[j].toDouble();
        }
        //7月数据
        QList<QString> wLst_7 = windList[6];
        for(int j = 0;j < 360;j++){
            datas_7[j] = wLst_7[j].toDouble();
        }
        //8月数据
        QList<QString> wLst_8 = windList[7];
        for(int j = 0;j < 360;j++){
            datas_8[j] = wLst_8[j].toDouble();
        }
        //9月数据
        QList<QString> wLst_9 = windList[8];
        for(int j = 0;j < 360;j++){
            datas_9[j] = wLst_9[j].toDouble();
        }
        //10月数据
        QList<QString> wLst_10 = windList[9];
        for(int j = 0;j < 360;j++){
            datas_10[j] = wLst_10[j].toDouble();
        }
        //11月数据
        QList<QString> wLst_11 = windList[10];
        for(int j = 0;j < 360;j++){
            datas_11[j] = wLst_11[j].toDouble();
        }
        //12月数据
        QList<QString> wLst_12 = windList[11];
        for(int j = 0;j < 360;j++){
            datas_12[j] = wLst_12[j].toDouble();
        }

        PolarChart *c = new PolarChart(460, 460);
        c->addTitle(QString("%1年").arg(key).toStdString().c_str(), "msyh.ttf", 10, 0x000000)->setBackground(0xa0c8ff, 0x000000,
                                                                           Chart::glassEffect());
        c->setPlotArea(230, 240, 180);
        c->setGridStyle(false);
        c->addLegend(459, 0, true, "msyh.ttf", 9)->setAlignment(Chart::TopRight);
        c->angularAxis()->setLinearScale(0, 360, 30);

        //1月
        PolarSplineLineLayer *layer1 = c->addSplineLineLayer(DoubleArray(datas_1, 360), 0x0000ff, "1月");
        layer1->setAngles(DoubleArray(angles, 360));
        layer1->setLineWidth(3);
        //2月
        PolarSplineLineLayer *layer2 = c->addSplineLineLayer(DoubleArray(datas_2, 360), 0x0000ff, "2月");
        layer2->setAngles(DoubleArray(angles, 360));
        layer2->setLineWidth(3);
        //3月
        PolarSplineLineLayer *layer3 = c->addSplineLineLayer(DoubleArray(datas_3, 360), 0x0000ff, "3月");
        layer3->setAngles(DoubleArray(angles, 360));
        layer3->setLineWidth(3);
        //4月
        PolarSplineLineLayer *layer4 = c->addSplineLineLayer(DoubleArray(datas_4, 360), 0x0000ff, "4月");
        layer4->setAngles(DoubleArray(angles, 360));
        layer4->setLineWidth(3);
        //5月
        PolarSplineLineLayer *layer5 = c->addSplineLineLayer(DoubleArray(datas_5, 360), 0x0000ff, "5月");
        layer5->setAngles(DoubleArray(angles, 360));
        layer5->setLineWidth(3);
        //6月
        PolarSplineLineLayer *layer6 = c->addSplineLineLayer(DoubleArray(datas_6, 360), 0x0000ff, "6月");
        layer6->setAngles(DoubleArray(angles, 360));
        layer6->setLineWidth(3);
        //7月
        PolarSplineLineLayer *layer7 = c->addSplineLineLayer(DoubleArray(datas_7, 360), 0x0000ff, "7月");
        layer7->setAngles(DoubleArray(angles, 360));
        layer7->setLineWidth(3);
        //8月
        PolarSplineLineLayer *layer8 = c->addSplineLineLayer(DoubleArray(datas_8, 360), 0x0000ff, "8月");
        layer8->setAngles(DoubleArray(angles, 360));
        layer8->setLineWidth(3);
        //9月
        PolarSplineLineLayer *layer9 = c->addSplineLineLayer(DoubleArray(datas_9, 360), 0x0000ff, "9月");
        layer9->setAngles(DoubleArray(angles, 360));
        layer9->setLineWidth(3);
        //10月
        PolarSplineLineLayer *layer10 = c->addSplineLineLayer(DoubleArray(datas_10, 360), 0x0000ff, "10月");
        layer10->setAngles(DoubleArray(angles, 360));
        layer10->setLineWidth(3);
        //11月
        PolarSplineLineLayer *layer11 = c->addSplineLineLayer(DoubleArray(datas_11, 360), 0x0000ff, "11月");
        layer11->setAngles(DoubleArray(angles, 360));
        layer11->setLineWidth(3);
        //12月
        PolarSplineLineLayer *layer12 = c->addSplineLineLayer(DoubleArray(datas_12, 360), 0x0000ff, "12月");
        layer12->setAngles(DoubleArray(angles, 360));
        layer12->setLineWidth(3);

        c->makeChart();
        polarChartView->setChart(c);
        imageLayout->addWidget(polarChartView);
    }

    //设置滚动区域的widget
    imageWidget->setLayout(imageLayout);
    imageArea->setWidget(imageWidget);
}
