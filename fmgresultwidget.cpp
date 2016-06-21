#include "fmgresultwidget.h"
#include "exportdialog.h"

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
    if(excelControl != NULL){
        if(excelControl->isRunning()){
            excelControl->quit();
            excelControl->wait();
        }
        delete excelControl;
    }
    int polarChartViewCount = polarChartViewList.count();
    for(int i = polarChartViewCount - 1;i >= 0;i--){
        QChartViewer *polarChartView = polarChartViewList[i];
        if(polarChartView->getChart() != NULL){
            delete polarChartView->getChart();
        }
        delete polarChartView;
    }
    delete imageLayout;
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

    excelControl = new ExcelControl;
}

void FmgResultWidget::initUI(){
    //设置背景色
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    //表格
    tableModel = new TableModel(0, titleList.size(), this, false);
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
    imageLayout = new QHBoxLayout;
    imageWidget->setLayout(imageLayout);
    imageArea->setWidget(imageWidget);
    //纵向布局
    this->setOrientation(Qt::Vertical);
    //添加控件
    this->addWidget(tableView);
    this->addWidget(imageArea);
//    this->setStretchFactor(0, 1);
}

void FmgResultWidget::initConnect(){
    connect(fmgControl, SIGNAL(sendMessage(int,QString,QList<QString>)), this, SLOT(receiveMessage(int,QString,QList<QString>)), Qt::QueuedConnection);
    connect(fmgControl, SIGNAL(sendMessage(int,QString)), this, SLOT(receiveMessage(int,QString)), Qt::QueuedConnection);
    connect(fmgControl, SIGNAL(execute(bool)), this, SLOT(execute(bool)), Qt::QueuedConnection);
    connect(fmgControl, SIGNAL(setProgressValue(int)), this, SIGNAL(setProgressValue(int)));
    connect(excelControl, SIGNAL(execute(bool)), this, SLOT(xlsExecute(bool)));
    connect(excelControl, SIGNAL(setProgressValue(int)), this, SIGNAL(setProgressValue(int)));
}

void FmgResultWidget::executeFmg(QString code, QString runway, QString fspeed, QString tspeed, QList<QString> years){
    int rowCount = tableModel->rowCount();
    for(int i = rowCount - 1;i >= 0;i--){
        tableModel->removeRow(i);
    }
    windHash.clear();
    windCountHash.clear();
    fmgControl->setData(code, runway, fspeed, tspeed, years);
    fmgControl->start();
}

void FmgResultWidget::executeExport(){
    ExportDialog exportDialog(polarChartViewList.size());
    connect(&exportDialog, SIGNAL(exportFiles(QHash< int, QList<QString> >)), this, SLOT(exportFiles(QHash< int, QList<QString> >)));
    exportDialog.exec();
    disconnect(&exportDialog, SIGNAL(exportFiles(QHash< int, QList<QString> >)), this, SLOT(exportFiles(QHash< int, QList<QString> >)));
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
    int sumWind = 0;
    for(int i = 0;i < windCount;i++){
        tableModel->setData(tableModel->index(row, i + 2, QModelIndex()), windCountList[i]);
        sumWind += windCountList[i].toInt();
    }
    if(windCountHash.contains(year)){
        windCountHash[year] += sumWind;
    }else{
        windCountHash[year] = sumWind;
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

void FmgResultWidget::xlsExecute(bool isEnd){
    if(isEnd){
        emit setProgressValue(100);
        QMessageBox::information(this, tr("消息提示"), tr("数据导出完成"));
    }
}

void FmgResultWidget::exportFiles(QHash< int, QList<QString> > fileHash){
    QString excelPath("");
    QList<int> typeList = fileHash.keys();
    for(int type : typeList){
        if(type == 0){
            for(QString filePath : fileHash[type]){
                excelPath = filePath;
            }
        }else{
            int fileCount = fileHash[type].size();
            for(int i = 0;i < fileCount;i++){
                QString filePath = fileHash[type][i].trimmed();
                if(!filePath.isEmpty()){
                    QChartViewer *chartViewer = polarChartViewList[i];
                    BaseChart *chart = chartViewer->getChart();
                    chart->makeChart(filePath.toStdString().c_str());
                }
            }
        }
    }
    if(excelPath.isEmpty()){
        QMessageBox::information(this, tr("消息提示"), tr("数据导出完成"));
    }else{
        excelControl->setExportPath(excelPath);
        excelControl->setTableParam(tableView, tableModel);
        excelControl->start();
    }
}

void FmgResultWidget::createCharts(){
    //销毁之前的chartView
    int polarChartViewCount = polarChartViewList.count();
    for(int i = polarChartViewCount - 1;i >= 0;i--){
        QChartViewer *polarChartView = polarChartViewList[i];
        if(polarChartView->getChart() != NULL){
            delete polarChartView->getChart();
        }
        delete polarChartView;
    }
    polarChartViewList.clear();
    //销毁之前的layout
    if(imageLayout != NULL){
        delete imageLayout;
    }
    //销毁之前的widget
    if(imageWidget != NULL){
        delete imageWidget;
    }
    //布局
    imageLayout = new QHBoxLayout;
    //widget
    imageWidget = new QWidget;
    //make chart
    QList<QString> keyList = windHash.keys();
    int keyCount = keyList.size();
    for(int i = 0;i < keyCount;i++){
        QChartViewer *polarChartView = new QChartViewer;
        QString key = keyList[i];
        int sumWind = windCountHash[key];
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
            datas_1[j] = wLst_1[j].toDouble() / sumWind;
        }
        //2月数据
        QList<QString> wLst_2 = windList[1];
        for(int j = 0;j < 360;j++){
            datas_2[j] = wLst_2[j].toDouble() / sumWind;
        }
        //3月数据
        QList<QString> wLst_3 = windList[2];
        for(int j = 0;j < 360;j++){
            datas_3[j] = wLst_3[j].toDouble() / sumWind;
        }
        //4月数据
        QList<QString> wLst_4 = windList[3];
        for(int j = 0;j < 360;j++){
            datas_4[j] = wLst_4[j].toDouble() / sumWind;
        }
        //5月数据
        QList<QString> wLst_5 = windList[4];
        for(int j = 0;j < 360;j++){
            datas_5[j] = wLst_5[j].toDouble() / sumWind;
        }
        //6月数据
        QList<QString> wLst_6 = windList[5];
        for(int j = 0;j < 360;j++){
            datas_6[j] = wLst_6[j].toDouble() / sumWind;
        }
        //7月数据
        QList<QString> wLst_7 = windList[6];
        for(int j = 0;j < 360;j++){
            datas_7[j] = wLst_7[j].toDouble() / sumWind;
        }
        //8月数据
        QList<QString> wLst_8 = windList[7];
        for(int j = 0;j < 360;j++){
            datas_8[j] = wLst_8[j].toDouble() / sumWind;
        }
        //9月数据
        QList<QString> wLst_9 = windList[8];
        for(int j = 0;j < 360;j++){
            datas_9[j] = wLst_9[j].toDouble() / sumWind;
        }
        //10月数据
        QList<QString> wLst_10 = windList[9];
        for(int j = 0;j < 360;j++){
            datas_10[j] = wLst_10[j].toDouble() / sumWind;
        }
        //11月数据
        QList<QString> wLst_11 = windList[10];
        for(int j = 0;j < 360;j++){
            datas_11[j] = wLst_11[j].toDouble() / sumWind;
        }
        //12月数据
        QList<QString> wLst_12 = windList[11];
        for(int j = 0;j < 360;j++){
            datas_12[j] = wLst_12[j].toDouble() / sumWind;
        }

        PolarChart *c = new PolarChart(600, 500, Chart::brushedSilverColor(), Chart::Transparent, 2);
        c->setRoundedFrame(0x646464);
        TextBox *title = c->addTitle(QString("%1年").arg(key).toStdString().c_str(), "msyh.ttf", 10, 0x000000);
        c->addLine(10, title->getHeight(), c->getWidth() - 11, title->getHeight(), Chart::LineColor);
        c->setPlotArea(300, 250, 200);
        c->setGridStyle(false);
        c->addLegend(590, title->getHeight() + 5, true, "msyh.ttf", 9)->setAlignment(Chart::TopRight);
        c->angularAxis()->setLinearScale(0, 360, 10);

        //1月
        PolarSplineLineLayer *layer1 = c->addSplineLineLayer(DoubleArray(datas_1, 360), 0x4D7FBC, "1月");
        layer1->setAngles(DoubleArray(angles, 360));
        layer1->setLineWidth(2);
        //2月
        PolarSplineLineLayer *layer2 = c->addSplineLineLayer(DoubleArray(datas_2, 360), 0xC0504D, "2月");
        layer2->setAngles(DoubleArray(angles, 360));
        layer2->setLineWidth(2);
        //3月
        PolarSplineLineLayer *layer3 = c->addSplineLineLayer(DoubleArray(datas_3, 360), 0x9BBB59, "3月");
        layer3->setAngles(DoubleArray(angles, 360));
        layer3->setLineWidth(2);
        //4月
        PolarSplineLineLayer *layer4 = c->addSplineLineLayer(DoubleArray(datas_4, 360), 0x7E62A1, "4月");
        layer4->setAngles(DoubleArray(angles, 360));
        layer4->setLineWidth(2);
        //5月
        PolarSplineLineLayer *layer5 = c->addSplineLineLayer(DoubleArray(datas_5, 360), 0x4BACC6, "5月");
        layer5->setAngles(DoubleArray(angles, 360));
        layer5->setLineWidth(2);
        //6月
        PolarSplineLineLayer *layer6 = c->addSplineLineLayer(DoubleArray(datas_6, 360), 0xF79646, "6月");
        layer6->setAngles(DoubleArray(angles, 360));
        layer6->setLineWidth(2);
        //7月
        PolarSplineLineLayer *layer7 = c->addSplineLineLayer(DoubleArray(datas_7, 360), 0x244670, "7月");
        layer7->setAngles(DoubleArray(angles, 360));
        layer7->setLineWidth(2);
        //8月
        PolarSplineLineLayer *layer8 = c->addSplineLineLayer(DoubleArray(datas_8, 360), 0x772C2A, "8月");
        layer8->setAngles(DoubleArray(angles, 360));
        layer8->setLineWidth(2);
        //9月
        PolarSplineLineLayer *layer9 = c->addSplineLineLayer(DoubleArray(datas_9, 360), 0x5F7530, "9月");
        layer9->setAngles(DoubleArray(angles, 360));
        layer9->setLineWidth(2);
        //10月
        PolarSplineLineLayer *layer10 = c->addSplineLineLayer(DoubleArray(datas_10, 360), 0x46345C, "10月");
        layer10->setAngles(DoubleArray(angles, 360));
        layer10->setLineWidth(2);
        //11月
        PolarSplineLineLayer *layer11 = c->addSplineLineLayer(DoubleArray(datas_11, 360), 0x1F6477, "11月");
        layer11->setAngles(DoubleArray(angles, 360));
        layer11->setLineWidth(2);
        //12月
        PolarSplineLineLayer *layer12 = c->addSplineLineLayer(DoubleArray(datas_12, 360), 0xC77F42, "12月");
        layer12->setAngles(DoubleArray(angles, 360));
        layer12->setLineWidth(2);

        c->makeChart();
        polarChartView->setChart(c);
        imageLayout->addWidget(polarChartView);
        polarChartViewList.append(polarChartView);
    }
    imageWidget->setLayout(imageLayout);
    imageArea->setWidget(imageWidget);
}
