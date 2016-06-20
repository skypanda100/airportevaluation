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
    if(excelControl != NULL){
        if(excelControl->isRunning()){
            excelControl->quit();
            excelControl->wait();
        }
        delete excelControl;
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

    excelControl = new ExcelControl;
}

void RckqResultWidget::initUI(){
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
    this->addWidget(imageArea);
}

void RckqResultWidget::initConnect(){
    connect(rckqControl
            , SIGNAL(sendMessage(int,QString,QString,QList<QString>,QString))
            , this
            , SLOT(receiveMessage(int,QString,QString,QList<QString>,QString))
            , Qt::QueuedConnection);
    connect(rckqControl, SIGNAL(execute(bool)), this, SLOT(execute(bool)), Qt::QueuedConnection);
    connect(rckqControl, SIGNAL(setProgressValue(int)), this, SIGNAL(setProgressValue(int)));
    connect(excelControl, SIGNAL(execute(bool)), this, SLOT(xlsExecute(bool)));
    connect(excelControl, SIGNAL(setProgressValue(int)), this, SIGNAL(setProgressValue(int)));
}

void RckqResultWidget::executeRckq(QString code, QString runway, int type, int fhour, int thour, QList<QString> dateList, QList<QString> weatherList){
    dataHash.clear();
    int rowCount = tableModel->rowCount();
    for(int i = rowCount - 1;i >= 0;i--){
        tableModel->removeRow(i);
    }
    rckqControl->setData(code, runway, type, fhour, thour, dateList, weatherList);
    rckqControl->start();

    this->m_type = type;
    this->m_fhour = fhour;
    this->m_thour = thour;
    this->m_dateList = dateList;
    this->m_weatherList = weatherList;
}

void RckqResultWidget::executeExport(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("数据导出"), qApp->applicationDirPath(),
                                                    tr("Excel Files (*.xls *.xlsx)"));
    if(fileName.isEmpty()){
        return;
    }else{
        excelControl->setExportPath(fileName);
        excelControl->setTableParam(tableView, tableModel);
        excelControl->start();
    }
}

void RckqResultWidget::receiveMessage(int row, QString weather, QString dateStr, QList<QString> valueList, QString keyStr){
    tableModel->insertRows(row, 1, QModelIndex());
    tableModel->setData(tableModel->index(row, 0, QModelIndex()), weather);
    tableModel->setData(tableModel->index(row, 1, QModelIndex()), dateStr);
    int valueCount = valueList.size();
    for(int i = 0;i < valueCount;i++){
        tableModel->setData(tableModel->index(row, 2 + i, QModelIndex()), valueList[i]);
    }

    if(m_type == 0){
        if(dataHash.contains(weather)){
            QList<QString> dataList = dataHash[weather];
            dataList.append(valueList);
            dataHash[weather] = dataList;
        }else{
            dataHash[weather] = valueList;
        }
    }else{
        if(dataHash.contains(keyStr)){
            QList<QString> dataList = dataHash[keyStr];
            dataList.append(valueList);
            dataHash[keyStr] = dataList;
        }else{
            dataHash[keyStr] = valueList;
        }
    }
}

void RckqResultWidget::execute(bool isEnd){
    if(isEnd){
        emit setProgressValue(100);
        createCharts();
    }
}

void RckqResultWidget::xlsExecute(bool isEnd){
    if(isEnd){
        emit setProgressValue(100);
        QMessageBox::information(this, tr("消息提示"), tr("数据导出完成"));
    }
}

void RckqResultWidget::createCharts(){
    //颜色
    int colors[] = {0x4D7FBC, 0xC0504D, 0x9BBB59, 0x7E62A1, 0x4BACC6, 0xF79646, 0x244670, 0x772C2A, 0x5F7530, 0x46345C, 0x1F6477, 0xC77F42};
    //销毁之前的imageWidget
    if(imageWidget != NULL){
        delete imageWidget;
        imageWidget = NULL;
        imageWidget = new QWidget;
    }
    //布局
    QHBoxLayout *imageLayout = new QHBoxLayout;
    imageLayout->setSpacing(5);
    //计算x轴上应有的数据数量
    int xDataCount = 0;
    if(m_fhour > m_thour){
        xDataCount =  (24 - m_fhour + 1 + m_thour) * 60;
    }else{
        xDataCount = (m_thour - m_fhour + 1) * 60;
    }
    double *dataX = new double[xDataCount];
    QDateTime dateTime = QDateTime::fromString("2016-05-08", "yyyy-MM-dd");
    QDateTime tdateTime = QDateTime::fromString(QString("2016-05-08 %1:00:00").arg(m_thour), "yyyy-MM-dd h:mm:ss");
    if(m_fhour > m_thour){
        dateTime = dateTime.addDays(-1);
    }
    QString datetimeStr = QString("%1 %2:00:00")
            .arg(dateTime.toString("yyyy-MM-dd"))
            .arg(m_fhour);
    dateTime = QDateTime::fromString(datetimeStr, "yyyy-MM-dd h:mm:ss");
    int hourIndex = 0;
    do{
        int year = dateTime.toString("yyyy").toInt();
        int month = dateTime.toString("M").toInt();
        int day = dateTime.toString("d").toInt();
        int hour = dateTime.toString("h").toInt();
        for(int i = 0;i < 60;i++){
            dataX[hourIndex * 60 + i] = Chart::chartTime(year, month, day, hour, i, 0);
        }
        dateTime = dateTime.addSecs(3600);
        hourIndex++;
    }while(dateTime.secsTo(tdateTime) >= 0);

    //make chart
    QChartViewer *lineChartView = new QChartViewer;
    XYChart *c = new XYChart(1200, 360, Chart::brushedSilverColor(), Chart::Transparent, 2);
    c->setRoundedFrame(0x646464);
    LegendBox *legendBox;
    if(m_type == 0){
        TextBox *title = c->addTitle(m_dateList[0].toStdString().c_str(), "msyh.ttf", 10, 0x000000);
        title->setMargin(0, 0, 6, 6);
        c->addLine(10, title->getHeight(), c->getWidth() - 11, title->getHeight(), Chart::LineColor);
        legendBox = c->addLegend(c->getWidth() / 2, title->getHeight(), false, "msyh.ttf", 10);
    }else{
        TextBox *title = c->addTitle(m_weatherList[0].toStdString().c_str(), "msyh.ttf", 10, 0x000000);
        title->setMargin(0, 0, 6, 6);
        c->addLine(10, title->getHeight(), c->getWidth() - 11, title->getHeight(), Chart::LineColor);
        legendBox = c->addLegend(c->getWidth() / 2, title->getHeight(), false, "msyh.ttf", 10);
    }
    legendBox->setAlignment(Chart::TopCenter);
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);
    c->setPlotArea(70, 75, 1200, 240, -1, -1, Chart::Transparent, 0x000000, -1);
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);

    c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(), "<*font=msyh.ttf*>{value|hh:nn}",
        Chart::AllPassFilter(), "{value|hh:nn}");

    QList<QString> keyList = dataHash.keys();
    int keyCount = keyList.size();
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QList<QString> valueList = dataHash[key];
        double *datas = new double[xDataCount];
        for(int j = 0;j < xDataCount;j++){
            double value = Chart::NoValue;
            if(valueList[j].compare("") != 0){
                value = valueList[j].toDouble();
            }
            datas[j] = value;
        }
        LineLayer *layer = c->addLineLayer();
        layer->setXData(DoubleArray(dataX, xDataCount));

        layer->addDataSet(DoubleArray(datas, xDataCount), colors[i], key.toStdString().c_str())->setDataSymbol(Chart::NoShape);
        layer->setLineWidth(2);
        layer->setGapColor(c->dashLineColor(colors[i]));
    }
    c->layoutLegend();
    c->packPlotArea(15, legendBox->getTopY() + legendBox->getHeight(), c->getWidth() - 16,
        c->getHeight() - 25);
    c->makeChart();
    lineChartView->setChart(c);
    imageLayout->addWidget(lineChartView, 1, Qt::AlignHCenter);
    //设置滚动区域的widget
    imageWidget->setLayout(imageLayout);
    imageArea->setWidget(imageWidget);
}
