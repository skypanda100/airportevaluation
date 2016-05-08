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
              << "不可飞"
              << "影响原因";
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
    QPalette imagePalette;
    imagePalette.setColor(QPalette::Background, QColor(100, 100, 100));
    imageArea->setAutoFillBackground(true);
    imageArea->setPalette(imagePalette);
    imageArea->setContentsMargins(5, 5, 5, 5);
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
        this->createCharts();
    }
}

void KfttjResultWidget::createCharts(){
    QHash< QString, QList<float> > kfttjHash = kfttjControl->getKfttjHash();
    QHash< QString, QStringList > xzkfEffectHash = kfttjControl->getXzkfEffectHash();
    QHash< QString, QStringList > bkfEffectHash = kfttjControl->getBkfEffectHash();
    //布局
    QHBoxLayout *imageLayout = new QHBoxLayout;
    imageLayout->setSpacing(5);

    createNkfttjChart(kfttjHash, imageLayout);
    createKftyfbChart(kfttjHash, imageLayout);
    createZlwzdChart(kfttjHash, imageLayout);
    createXzkfNfbChart(xzkfEffectHash, imageLayout);
    createBkfNfbChart(bkfEffectHash, imageLayout);
    createXzkfYfbChart(xzkfEffectHash, imageLayout);
    createBkfYfbChart(bkfEffectHash, imageLayout);

    //设置滚动区域的widget
    imageWidget->setLayout(imageLayout);
    imageArea->setWidget(imageWidget);
}

/**
 * @brief KfttjResultWidget::createNkfttjChart
 * 年可飞天统计:采用饼图（要素为完全可飞，限制可飞，不可飞），可以包含几年的数据。
 * @param kfttjHash
 * @param imageLayout
 */
void KfttjResultWidget::createNkfttjChart(QHash< QString, QList<float> > kfttjHash, QLayout *imageLayout){
    //key值
    QList<QString> keyList = kfttjHash.keys();
    int keyCount = keyList.size();
    QChartViewer *pieChartView = new QChartViewer;
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
    pieChartView->setChart(pieChart);
    imageLayout->addWidget(pieChartView);
}

/**
 * @brief KfttjResultWidget::createKftyfbChart
 * 采用柱状图（要素为可飞，限制可飞，不可飞，横轴为1到12月，纵轴为绝对数值或者百分比），可以包含几年数据。
 * @param kfttjHash
 * @param imageLayout
 */
void KfttjResultWidget::createKftyfbChart(QHash< QString, QList<float> > kfttjHash, QLayout *imageLayout){
    //key值
    QList<QString> keyList = kfttjHash.keys();
    int keyCount = keyList.size();
    QChartViewer *xyChartView = new QChartViewer;

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
    xyChartView->setChart(xyChart);
    imageLayout->addWidget(xyChartView);
}

/**
 * @brief KfttjResultWidget::createZlwzdChart
 * 半天统计占全年自然日数的百分比, 采用饼图(要素为半天,整天,缺测)。
 * @param kfttjHash
 * @param imageLayout
 */
void KfttjResultWidget::createZlwzdChart(QHash< QString, QList<float> > kfttjHash, QLayout *imageLayout){
    //key值
    QList<QString> keyList = kfttjHash.keys();
    int keyCount = keyList.size();
    QChartViewer *pieChartView = new QChartViewer;
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
    pieChartView->setChart(pieChart);
    imageLayout->addWidget(pieChartView);
}

/**
 * @brief KfttjResultWidget::createXzkfNfbChart
 * 限制可飞的年分布：采用饼图（要素为影响可飞天的各个要素），可以包含几年数据。
 * @param effectHash
 * @param imageLayout
 */
void KfttjResultWidget::createXzkfNfbChart(QHash<QString, QStringList> effectHash, QLayout *imageLayout){
    //key值
    QList<QString> keyList = effectHash.keys();
    int keyCount = keyList.size();
    QChartViewer *pieChartView = new QChartViewer;
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
    pieChartView->setChart(pieChart);
    imageLayout->addWidget(pieChartView);
}

/**
 * @brief KfttjResultWidget::createBkfNfbChart
 * 不可飞的年分布：采用饼图（要素为影响可飞天的各个要素），可以包含几年数据。
 * @param effectHash
 * @param imageLayout
 */
void KfttjResultWidget::createBkfNfbChart(QHash<QString, QStringList> effectHash, QLayout *imageLayout){
    //key值
    QList<QString> keyList = effectHash.keys();
    int keyCount = keyList.size();
    QChartViewer *pieChartView = new QChartViewer;
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
    pieChartView->setChart(pieChart);
    imageLayout->addWidget(pieChartView);
}

/**
 * @brief KfttjResultWidget::createXzkfYfbChart
 * 限制可飞的月分布：采用柱状图（要素为影响可飞天的各个要素，横轴为1到12月，纵轴为百分比），可以包含几年数据。
 * @param effectHash
 * @param imageLayout
 */
void KfttjResultWidget::createXzkfYfbChart(QHash<QString, QStringList> effectHash, QLayout *imageLayout){
    //key值
    QList<QString> keyList = effectHash.keys();
    int keyCount = keyList.size();
    QChartViewer *xyChartView = new QChartViewer;
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
    xyChartView->setChart(xyChart);
    imageLayout->addWidget(xyChartView);
}

/**
 * @brief KfttjResultWidget::createBkfYfbChart
 * 不可飞的月分布：采用柱状图（要素为影响可飞天的各个要素，横轴为1到12月，纵轴为百分比），可以包含几年数据。
 * @param effectHash
 * @param imageLayout
 */
void KfttjResultWidget::createBkfYfbChart(QHash<QString, QStringList> effectHash, QLayout *imageLayout){
    //key值
    QList<QString> keyList = effectHash.keys();
    int keyCount = keyList.size();
    QChartViewer *xyChartView = new QChartViewer;
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
    xyChartView->setChart(xyChart);
    imageLayout->addWidget(xyChartView);
}
