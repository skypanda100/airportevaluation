#include "kfttjresultwidget.h"
#include "exportdialog.h"

KfttjResultWidget::KfttjResultWidget(QWidget *parent)
    :QSplitter(parent)
    ,tableModel(NULL)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

KfttjResultWidget::~KfttjResultWidget(){
    delete tableModel;
    delete tableView;
    if(nkfttjChartView->getChart() != NULL){
        delete nkfttjChartView->getChart();
    }
    delete nkfttjChartView;
    if(kftyfbChartView->getChart() != NULL){
        delete kftyfbChartView->getChart();
    }
    delete kftyfbChartView;
    if(zlwzdChartView->getChart() != NULL){
        delete zlwzdChartView->getChart();
    }
    delete zlwzdChartView;
    if(xzkfnfbChartView->getChart() != NULL){
        delete xzkfnfbChartView->getChart();
    }
    delete xzkfnfbChartView;
    if(bkfnfbChartView->getChart() != NULL){
        delete bkfnfbChartView->getChart();
    }
    delete bkfnfbChartView;
    if(xzkfyfbChartView->getChart() != NULL){
        delete xzkfyfbChartView->getChart();
    }
    delete xzkfyfbChartView;
    if(bkfyfbChartView->getChart() != NULL){
        delete bkfyfbChartView->getChart();
    }
    delete bkfyfbChartView;
    delete imageArea;
    if(kfttjControl != NULL){
        if(kfttjControl->isRunning()){
            kfttjControl->quit();
            kfttjControl->wait();
        }
        delete kfttjControl;
    }
    if(excelControl != NULL){
        if(excelControl->isRunning()){
            excelControl->quit();
            excelControl->wait();
        }
        delete excelControl;
    }
}

void KfttjResultWidget::initData(){
    kfttjControl = new KfttjControl;

    excelControl = new ExcelControl;
}

void KfttjResultWidget::initUI(){
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    tableView = new QTableView;
    createTable(true);

    this->setOrientation(Qt::Vertical);
    this->addWidget(tableView);
    //调整列宽
//    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    imageArea = new QScrollArea;
    QPalette imagePalette;
    imagePalette.setColor(QPalette::Background, QColor(100, 100, 100));
    imageArea->setAutoFillBackground(true);
    imageArea->setPalette(imagePalette);
    imageArea->setContentsMargins(5, 5, 5, 5);
    QWidget *imageWidget = new QWidget;
    QHBoxLayout *imageLayout = new QHBoxLayout;
    imageWidget->setLayout(imageLayout);

    nkfttjChartView = new QChartViewer;
    kftyfbChartView = new QChartViewer;
    zlwzdChartView = new QChartViewer;
    xzkfnfbChartView = new QChartViewer;
    bkfnfbChartView = new QChartViewer;
    xzkfyfbChartView = new QChartViewer;
    bkfyfbChartView = new QChartViewer;

    nkfttjChartView->setFixedSize(300
                                 , 300);
    kftyfbChartView->setFixedSize(600
                                 , 300);
    zlwzdChartView->setFixedSize(300
                                , 300);
    xzkfnfbChartView->setFixedSize(300
                                  , 300);
    bkfnfbChartView->setFixedSize(300
                                 , 300);
    xzkfyfbChartView->setFixedSize(700
                                  , 300);
    bkfyfbChartView->setFixedSize(700
                                 , 300);

    imageLayout->addWidget(nkfttjChartView);
    imageLayout->addWidget(kftyfbChartView);
    imageLayout->addWidget(zlwzdChartView);
    imageLayout->addWidget(xzkfnfbChartView);
    imageLayout->addWidget(bkfnfbChartView);
    imageLayout->addWidget(xzkfyfbChartView);
    imageLayout->addWidget(bkfyfbChartView);

    imageArea->setWidget(imageWidget);
    this->addWidget(imageArea);
    this->setStretchFactor(0, 1);
}

void KfttjResultWidget::initConnect(){
    connect(kfttjControl, SIGNAL(sendMessage(int,int)), this, SLOT(receiveMessage(int,int)), Qt::QueuedConnection);
    connect(kfttjControl, SIGNAL(sendMessage(QString,int,int,int,int)), this, SLOT(receiveMessage(QString,int,int,int,int)), Qt::QueuedConnection);
    connect(kfttjControl, SIGNAL(execute(bool)), this, SLOT(execute(bool)), Qt::QueuedConnection);
    connect(kfttjControl, SIGNAL(setProgressValue(int)), this, SIGNAL(setProgressValue(int)), Qt::QueuedConnection);
    connect(excelControl, SIGNAL(execute(bool)), this, SLOT(xlsExecute(bool)));
    connect(excelControl, SIGNAL(setProgressValue(int)), this, SIGNAL(setProgressValue(int)));
}

void KfttjResultWidget::createTable(bool isHourTable){
    //清除table
    if(tableModel != NULL){
        int rowCount = tableModel->rowCount();
        for(int i = rowCount - 1;i >= 0;i--){
            tableModel->removeRow(i);
        }
    }
    //清除title
    titleList.clear();
    if(isHourTable){
        titleList << "日期" << "气象要素"
                  << "17" << "18" << "19" << "20" << "21" << "22"
                  << "23" << "0" << "1" << "2" << "3" << "4"
                  << "5" << "6" << "7" << "8" << "9" << "10"
                  << "11" << "12" << "13" << "14" << "15" << "16"
                  << "完全可飞" << "限制可飞" << "不可飞" << "影响原因";
    }else{
        titleList << "日期" << "气象要素"
                  << "17 ~ 16"
                  << "完全可飞" << "限制可飞" << "不可飞" << "影响原因";
    }
    //设置tablemodel
    if(tableModel != NULL){
        delete tableModel;
    }
    tableModel = new TableModel(0, titleList.size(), this, true);
    int titleCount = titleList.size();
    for(int i = 0;i < titleCount;i++){
        tableModel->setHeaderData(i, Qt::Horizontal, titleList[i]);
    }
    tableView->setModel(tableModel);
}

void KfttjResultWidget::executeKfttj(QString airportCode, QList<QString> dateList, bool isMultiWeather, QList<WeatherParam> wpList){
    if(kfttjControl->isRunning()){
        QMessageBox::information(this, tr("消息提示"), tr("正在处理数据......\n请稍后操作"));
        return;
    }
    //根据多要素单要素创建对应的table
    if(isMultiWeather){
        createTable(true);
    }else{
        WeatherParam weatherParam = wpList[0];
        switch(weatherParam.id()){
        case 21:
        case 22:
        case 23:
        case 24:
        case 27:
            createTable(true);
            break;
        default:
            createTable(false);
        }
    }
    //执行可飞天统计
    int dateCount = dateList.count();
    for(int i = 0;i < dateCount;i++){
        int dateInt = dateList[i].toInt();
        //查询月总簿表
        QString summaryStartDatetime = QString("%1-12-31 17:00:00").arg(dateInt - 1);
        QString summaryEndDatetime = QString("%1-12-31 16:00:00").arg(dateInt);

        QString summarySql = QString("select * from %1_monthsummary where datetime >= '%2' and datetime <= '%3' order by datetime")
                .arg(airportCode)
                .arg(summaryStartDatetime)
                .arg(summaryEndDatetime);

        //查询极值表
        QString extremumStartDatetime = QString("%1-01-01 00:00:00").arg(dateInt);
        QString extremumEndDatetime = QString("%1-12-31 23:59:59").arg(dateInt);

        QString extremumSql = QString("select * from %1_extremum where datetime >= '%2' and datetime <= '%3' order by datetime")
                .arg(airportCode)
                .arg(extremumStartDatetime)
                .arg(extremumEndDatetime);

        kfttjControl->setTitleList(titleList);
        kfttjControl->setSummarySql(summarySql);
        kfttjControl->setExtremumSql(extremumSql);
        kfttjControl->setAirportCode(airportCode);
        kfttjControl->setMultiWeather(isMultiWeather);
        kfttjControl->setWeatherParamList(wpList);
        kfttjControl->start();
    }
}

void KfttjResultWidget::executeExport(){
    ExportDialog exportDialog(7);
    connect(&exportDialog, SIGNAL(exportFiles(QHash< int, QList<QString> >)), this, SLOT(exportFiles(QHash< int, QList<QString> >)));
    exportDialog.exec();
    disconnect(&exportDialog, SIGNAL(exportFiles(QHash< int, QList<QString> >)), this, SLOT(exportFiles(QHash< int, QList<QString> >)));
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
        this->createCharts();
    }
}

void KfttjResultWidget::xlsExecute(bool isEnd){
    if(isEnd){
        emit setProgressValue(100);
        QMessageBox::information(this, tr("消息提示"), tr("数据导出完成"));
    }
}

void KfttjResultWidget::exportFiles(QHash<int, QList<QString> > fileHash){
    if(excelControl->isRunning()){
        QMessageBox::information(this, tr("消息提示"), tr("正在处理数据......\n请稍后操作"));
        return;
    }
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
                    BaseChart *baseChart = NULL;
                    switch(i){
                    case 0:
                        baseChart = nkfttjChartView->getChart();
                        break;
                    case 1:
                        baseChart = kftyfbChartView->getChart();
                        break;
                    case 2:
                        baseChart = zlwzdChartView->getChart();
                        break;
                    case 3:
                        baseChart = xzkfnfbChartView->getChart();
                        break;
                    case 4:
                        baseChart = bkfnfbChartView->getChart();
                        break;
                    case 5:
                        baseChart = xzkfyfbChartView->getChart();
                        break;
                    case 6:
                        baseChart = bkfyfbChartView->getChart();
                        break;
                    }
                    if(baseChart != NULL){
                        baseChart->makeChart(filePath.toStdString().c_str());
                    }
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

void KfttjResultWidget::createCharts(){
    QHash< QString, QList<float> > kfttjHash = kfttjControl->getKfttjHash();
    QHash< QString, QStringList > xzkfEffectHash = kfttjControl->getXzkfEffectHash();
    QHash< QString, QStringList > bkfEffectHash = kfttjControl->getBkfEffectHash();

    createNkfttjChart(kfttjHash);
    createKftyfbChart(kfttjHash);
    createZlwzdChart(kfttjHash);
    createXzkfNfbChart(xzkfEffectHash);
    createBkfNfbChart(bkfEffectHash);
    createXzkfYfbChart(xzkfEffectHash);
    createBkfYfbChart(bkfEffectHash);
}

/**
 * @brief KfttjResultWidget::createNkfttjChart
 * 年可飞天统计:采用饼图（要素为完全可飞，限制可飞，不可飞），可以包含几年的数据。
 * @param kfttjHash
 */
void KfttjResultWidget::createNkfttjChart(const QHash< QString, QList<float> > &kfttjHash){
    //删除以前的chart
    if(nkfttjChartView->getChart() != NULL){
        delete nkfttjChartView->getChart();
    }
    //key值
    QList<QString> keyList = kfttjHash.keys();
    int keyCount = keyList.size();
    double datas[] = {0, 0, 0};
    const char *labels[] = {"完全可飞", "限制可飞", "不可飞"};
    int colors[] = {0x329600, 0xffb400, 0xdc3200};
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QList<float> value = kfttjHash[key];
        datas[0] += value[0];
        datas[1] += value[1];
        datas[2] += value[2];
    }
    PieChart *pieChart = new PieChart(300, 300, Chart::brushedSilverColor(), Chart::Transparent, 2);
    pieChart->setRoundedFrame(0x646464);
    pieChart->setDefaultFonts("msyh.ttf");
    TextBox *title = pieChart->addTitle("年可飞天统计", "msyh.ttf", 10, 0x000000);
    pieChart->addLine(10, title->getHeight(), pieChart->getWidth() - 11, title->getHeight(), Chart::LineColor);
    pieChart->setPieSize(150, 150, 120);
    pieChart->setLabelPos(-40);
    pieChart->setData(DoubleArray(datas, (int)(sizeof(datas) / sizeof(datas[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));
    pieChart->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));
    pieChart->makeChart();
    nkfttjChartView->setChart(pieChart);
}

/**
 * @brief KfttjResultWidget::createKftyfbChart
 * 采用柱状图（要素为可飞，限制可飞，不可飞，横轴为1到12月，纵轴为绝对数值或者百分比），可以包含几年数据。
 * @param kfttjHash
 */
void KfttjResultWidget::createKftyfbChart(const QHash< QString, QList<float> > &kfttjHash){
    //删除以前的chart
    if(kftyfbChartView->getChart() != NULL){
        delete kftyfbChartView->getChart();
    }
    //key值
    QList<QString> keyList = kfttjHash.keys();
    int keyCount = keyList.size();

    double data0[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double data1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double data2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const char *labels[] = {"1月", "2月", "3月", "4月", "5月", "6月", "7月", "8月", "9月", "10月", "11月", "12月"};
    int colors[] = {0x329600, 0xffb400, 0xdc3200};
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QList<float> value = kfttjHash[key];
        int month = QDateTime::fromString(key, "yyyy-MM-dd").toString("M").toInt();
        data0[month -  1] += value[0];
        data1[month -  1] += value[1];
        data2[month -  1] += value[2];
    }
    XYChart *xyChart = new XYChart(600, 300, Chart::brushedSilverColor(), Chart::Transparent, 2);
    xyChart->setRoundedFrame(0x646464);

    xyChart->setDefaultFonts("msyh.ttf");
    TextBox *title = xyChart->addTitle("可飞天月分布", "msyh.ttf", 10, 0x000000);
    xyChart->addLine(10, title->getHeight(), xyChart->getWidth() - 11, title->getHeight(), Chart::LineColor);
    xyChart->setPlotArea(35, title->getHeight() + 5, xyChart->getWidth() - 35, xyChart->getHeight() -
        title->getHeight() - 35, 0xe8f0f8, -1, Chart::Transparent, 0xaaaaaa);
    xyChart->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));
    BarLayer *layer = xyChart->addBarLayer(Chart::Percentage);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), colors[0],
        "<*block,valign=absmiddle*><*img=service.png*> Service<*/*>");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), colors[1],
        "<*block,valign=absmiddle*><*img=software.png*> Software<*/*>");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), colors[2],
        "<*block,valign=absmiddle*><*img=computer.png*> Hardware<*/*>");
    layer->setBorderColor(Chart::Transparent, Chart::softLighting(Chart::Top));
    layer->setDataLabelStyle()->setAlignment(Chart::Center);
    xyChart->makeChart();
    kftyfbChartView->setChart(xyChart);
}

/**
 * @brief KfttjResultWidget::createZlwzdChart
 * 半天统计占全年自然日数的百分比, 采用饼图(要素为半天,整天,缺测)。
 * @param kfttjHash
 */
void KfttjResultWidget::createZlwzdChart(const QHash< QString, QList<float> > &kfttjHash){
    //删除以前的chart
    if(zlwzdChartView->getChart() != NULL){
        delete zlwzdChartView->getChart();
    }
    //key值
    QList<QString> keyList = kfttjHash.keys();
    int keyCount = keyList.size();
    double datas[] = {0, 0, 0};
    const char *labels[] = {"整天", "半天", "缺测"};
    int colors[] = {0x329600, 0xffb400, 0xdc3200};
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QList<float> value = kfttjHash[key];
        float sum = value[0] + value[1] + value[2];
        if(sum == 0){
            datas[2] += 1;
        }else if(sum == 0.5){
            datas[1] += 1;
        }else{
            datas[0] += 1;
        }
    }
    PieChart *pieChart = new PieChart(300, 300, Chart::brushedSilverColor(), Chart::Transparent, 2);
    pieChart->setRoundedFrame(0x646464);
    pieChart->setDefaultFonts("msyh.ttf");
    TextBox *title = pieChart->addTitle("资料完整度", "msyh.ttf", 10, 0x000000);
    pieChart->setPieSize(150, 150, 120);
    pieChart->addLine(10, title->getHeight(), pieChart->getWidth() - 11, title->getHeight(), Chart::LineColor);
    pieChart->setLabelPos(-40);
    pieChart->setData(DoubleArray(datas, (int)(sizeof(datas) / sizeof(datas[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));
    pieChart->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));
    pieChart->makeChart();
    zlwzdChartView->setChart(pieChart);
}

/**
 * @brief KfttjResultWidget::createXzkfNfbChart
 * 限制可飞的年分布：采用饼图（要素为影响可飞天的各个要素），可以包含几年数据。
 * @param effectHash
 */
void KfttjResultWidget::createXzkfNfbChart(const QHash<QString, QStringList> &effectHash){
    //删除以前的chart
    if(xzkfnfbChartView->getChart() != NULL){
        delete xzkfnfbChartView->getChart();
    }
    //key值
    QList<QString> keyList = effectHash.keys();
    int keyCount = keyList.size();
    QList<QString> effectElementList;
    QList<double> effectCountList;
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QStringList effectList = effectHash[key];
        int effectCount = effectList.size();
        for(int j = 0;j < effectCount;j++){
            QString effectElement = effectList[j];
            int index = effectElementList.indexOf(effectElement);
            if(index >= 0){
                effectCountList[index] += 1;
            }else{
                effectElementList.append(effectElement);
                effectCountList.append(1);
            }
        }
    }

    double *datas = new double[effectCountList.size()];
    int dataCount = effectCountList.size();
    for(int i = 0;i < dataCount;i++){
        datas[i] = effectCountList[i];
    }
    const char **labels = new const char*[effectElementList.size()];
    int *colors = new int[effectElementList.size()];
    int labelCount = effectElementList.size();
    for(int i = 0;i < labelCount;i++){
        labels[i] = strdup(effectElementList[i].toStdString().c_str());
        colors[i] = COLORS[i];
    }

    PieChart *pieChart = new PieChart(300, 300, Chart::brushedSilverColor(), Chart::Transparent, 2);
    pieChart->setRoundedFrame(0x646464);
    pieChart->setDefaultFonts("msyh.ttf");
    TextBox *title = pieChart->addTitle("限制可飞要素的年分布", "msyh.ttf", 10, 0x000000);
    pieChart->addLine(10, title->getHeight(), pieChart->getWidth() - 11, title->getHeight(), Chart::LineColor);
    pieChart->setPieSize(150, 150, 120);
    pieChart->setLabelPos(-40);
    pieChart->setData(DoubleArray(datas, dataCount), StringArray(labels, labelCount));
    pieChart->setColors(Chart::DataColor, IntArray(colors, labelCount));
    pieChart->makeChart();
    xzkfnfbChartView->setChart(pieChart);
}

/**
 * @brief KfttjResultWidget::createBkfNfbChart
 * 不可飞的年分布：采用饼图（要素为影响可飞天的各个要素），可以包含几年数据。
 * @param effectHash
 */
void KfttjResultWidget::createBkfNfbChart(const QHash<QString, QStringList> &effectHash){
    //删除以前的chart
    if(bkfnfbChartView->getChart() != NULL){
        delete bkfnfbChartView->getChart();
    }
    //key值
    QList<QString> keyList = effectHash.keys();
    int keyCount = keyList.size();
    QList<QString> effectElementList;
    QList<double> effectCountList;
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QStringList effectList = effectHash[key];
        int effectCount = effectList.size();
        for(int j = 0;j < effectCount;j++){
            QString effectElement = effectList[j];
            int index = effectElementList.indexOf(effectElement);
            if(index >= 0){
                effectCountList[index] += 1;
            }else{
                effectElementList.append(effectElement);
                effectCountList.append(1);
            }
        }
    }

    double *datas = new double[effectCountList.size()];
    int dataCount = effectCountList.size();
    for(int i = 0;i < dataCount;i++){
        datas[i] = effectCountList[i];
    }
    const char **labels = new const char*[effectElementList.size()];
    int *colors = new int[effectElementList.size()];
    int labelCount = effectElementList.size();
    for(int i = 0;i < labelCount;i++){
        labels[i] = strdup(effectElementList[i].toStdString().c_str());
        colors[i] = COLORS[i];
    }

    PieChart *pieChart = new PieChart(300, 300, Chart::brushedSilverColor(), Chart::Transparent, 2);
    pieChart->setRoundedFrame(0x646464);
    pieChart->setDefaultFonts("msyh.ttf");
    TextBox *title = pieChart->addTitle("不可飞要素的年分布", "msyh.ttf", 10, 0x000000);
    pieChart->addLine(10, title->getHeight(), pieChart->getWidth() - 11, title->getHeight(), Chart::LineColor);
    pieChart->setPieSize(150, 150, 120);
    pieChart->setLabelPos(-40);
    pieChart->setData(DoubleArray(datas, dataCount), StringArray(labels, labelCount));
    pieChart->setColors(Chart::DataColor, IntArray(colors, labelCount));
    pieChart->makeChart();
    bkfnfbChartView->setChart(pieChart);
}

/**
 * @brief KfttjResultWidget::createXzkfYfbChart
 * 限制可飞的月分布：采用柱状图（要素为影响可飞天的各个要素，横轴为1到12月，纵轴为百分比），可以包含几年数据。
 * @param effectHash
 */
void KfttjResultWidget::createXzkfYfbChart(const QHash<QString, QStringList> &effectHash){
    //删除以前的chart
    if(xzkfyfbChartView->getChart() != NULL){
        delete xzkfyfbChartView->getChart();
    }
    //key值
    QList<QString> keyList = effectHash.keys();
    int keyCount = keyList.size();
    QList<QString> effectElementList;
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QStringList effectList = effectHash[key];
        int effectCount = effectList.size();
        for(int j = 0;j < effectCount;j++){
            QString effectElement = effectList[j];
            int index = effectElementList.indexOf(effectElement);
            if(index < 0){
                effectElementList.append(effectElement);
            }
        }
    }

    int effectElementCount = effectElementList.size();
    double **datas = new double*[effectElementCount];
    for(int i = 0;i < effectElementCount;i++){
        datas[i] = new double[12];
        for(int j = 0;j < 12;j++){
            datas[i][j] = 0;
        }
        for(int j = 0;j < keyCount;j++){
            QString key = keyList[j];
            QStringList effectList = effectHash[key];
            if(effectList.contains(effectElementList[i])){
                int month = QDateTime::fromString(key, "yyyy-MM-dd").toString("M").toInt();
                datas[i][month - 1] += 1;
            }
        }
    }
    const char *labels[] = {"1月", "2月", "3月", "4月", "5月", "6月", "7月", "8月", "9月", "10月", "11月", "12月"};

    int *colors = new int[effectElementCount];
    for(int i = 0;i < effectElementCount;i++){
        colors[i] = COLORS[i];
    }

    XYChart *xyChart = new XYChart(700, 300, Chart::brushedSilverColor(), Chart::Transparent, 2);
    xyChart->setRoundedFrame(0x646464);
    xyChart->setDefaultFonts("msyh.ttf");
    TextBox *title = xyChart->addTitle("限制可飞要素的月分布", "msyh.ttf", 10, 0x000000);
    xyChart->addLine(10, title->getHeight(), xyChart->getWidth() - 11, title->getHeight(), Chart::LineColor);

    xyChart->setPlotArea(35, title->getHeight() + 5, xyChart->getWidth() - 135, xyChart->getHeight() -
        title->getHeight() - 35, 0xe8f0f8, -1, Chart::Transparent, 0xaaaaaa);
    xyChart->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    LegendBox *legendBox = xyChart->addLegend(xyChart->getWidth() - 10, title->getHeight(), true, "msyh.ttf", 8);
    legendBox->setAlignment(Chart::TopRight);
    legendBox->setBackground(0xeeeeee, 0x000000);

    BarLayer *layer = xyChart->addBarLayer(Chart::Percentage);
    for(int i = 0;i < effectElementCount;i++){
        layer->addDataSet(DoubleArray(datas[i], 12), colors[i],
                          QString("<*block,valign=absmiddle*>%1<*/*>").arg(effectElementList[i]).toStdString().c_str());
    }
    layer->setBorderColor(Chart::Transparent, Chart::softLighting(Chart::Top));
    layer->setDataLabelStyle()->setAlignment(Chart::Center);
    xyChart->makeChart();
    xzkfyfbChartView->setChart(xyChart);
}

/**
 * @brief KfttjResultWidget::createBkfYfbChart
 * 不可飞的月分布：采用柱状图（要素为影响可飞天的各个要素，横轴为1到12月，纵轴为百分比），可以包含几年数据。
 * @param effectHash
 */
void KfttjResultWidget::createBkfYfbChart(const QHash<QString, QStringList> &effectHash){
    //删除以前的chart
    if(bkfyfbChartView->getChart() != NULL){
        delete bkfyfbChartView->getChart();
    }
    //key值
    QList<QString> keyList = effectHash.keys();
    int keyCount = keyList.size();
    QList<QString> effectElementList;
    for(int i = 0;i < keyCount;i++){
        QString key = keyList[i];
        QStringList effectList = effectHash[key];
        int effectCount = effectList.size();
        for(int j = 0;j < effectCount;j++){
            QString effectElement = effectList[j];
            int index = effectElementList.indexOf(effectElement);
            if(index < 0){
                effectElementList.append(effectElement);
            }
        }
    }

    int effectElementCount = effectElementList.size();
    double **datas = new double*[effectElementCount];
    for(int i = 0;i < effectElementCount;i++){
        datas[i] = new double[12];
        for(int j = 0;j < 12;j++){
            datas[i][j] = 0;
        }
        for(int j = 0;j < keyCount;j++){
            QString key = keyList[j];
            QStringList effectList = effectHash[key];
            if(effectList.contains(effectElementList[i])){
                int month = QDateTime::fromString(key, "yyyy-MM-dd").toString("M").toInt();
                datas[i][month - 1] += 1;
            }
        }
    }
    const char *labels[] = {"1月", "2月", "3月", "4月", "5月", "6月", "7月", "8月", "9月", "10月", "11月", "12月"};

    int *colors = new int[effectElementCount];
    for(int i = 0;i < effectElementCount;i++){
        colors[i] = COLORS[i];
    }

    XYChart *xyChart = new XYChart(700, 300, Chart::brushedSilverColor(), Chart::Transparent, 2);
    xyChart->setRoundedFrame(0x646464);
    xyChart->setDefaultFonts("msyh.ttf");
    TextBox *title = xyChart->addTitle("不可飞要素的月分布", "msyh.ttf", 10, 0x000000);
    xyChart->addLine(10, title->getHeight(), xyChart->getWidth() - 11, title->getHeight(), Chart::LineColor);

    xyChart->setPlotArea(35, title->getHeight() + 5, xyChart->getWidth() - 135, xyChart->getHeight() -
        title->getHeight() - 35, 0xe8f0f8, -1, Chart::Transparent, 0xaaaaaa);
    xyChart->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    LegendBox *legendBox = xyChart->addLegend(xyChart->getWidth() - 10, title->getHeight(), true, "msyh.ttf", 8);
    legendBox->setAlignment(Chart::TopRight);
    legendBox->setBackground(0xeeeeee, 0x000000);

    BarLayer *layer = xyChart->addBarLayer(Chart::Percentage);
    for(int i = 0;i < effectElementCount;i++){
        layer->addDataSet(DoubleArray(datas[i], 12), colors[i],
                          QString("<*block,valign=absmiddle*>%1<*/*>").arg(effectElementList[i]).toStdString().c_str());
    }
    layer->setBorderColor(Chart::Transparent, Chart::softLighting(Chart::Top));
    layer->setDataLabelStyle()->setAlignment(Chart::Center);
    xyChart->makeChart();
    bkfyfbChartView->setChart(xyChart);
}
