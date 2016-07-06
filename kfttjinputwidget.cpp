#include "kfttjinputwidget.h"
#include "common/sharedmemory.h"

KfttjInputWidget::KfttjInputWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

KfttjInputWidget::~KfttjInputWidget(){
    delete airportComboBox;
    int dateCheckBoxCount = dateCheckBoxList.size();
    for(int i = 0;i < dateCheckBoxCount;i++){
        QCheckBox *checkBox = dateCheckBoxList[i];
        delete checkBox;
    }
    dateCheckBoxList.clear();
    delete dateLayout;
    delete qxysComboBox;
    delete slfRadioButton;
    delete sfCheckBox;
    delete nfCheckBox;
    delete cfCheckBox;
    delete blfRadioButton;
    delete cgwRadioButton;
    delete cmqyRadioButton;
    delete xzhmqyRadioButton;
    delete cgsRadioButton;
    delete jsRadioButton;
    delete spnjdRadioButton;
    delete ylygRadioButton;
    delete fqjsRadioButton;
    delete njyCheckBox;
    delete jyyCheckBox;
    delete sdCheckBox;
    delete lbCheckBox;
    delete bbCheckBox;
    delete bxCheckBox;
    delete ljCheckBox;
    delete scbCheckBox;
    delete fqbCheckBox;
    delete fqfGroupBox;
    delete wdGroupBox;
    delete qyGroupBox;
    delete sdGroupBox;
    delete njdGroupBox;
    delete yGroupBox;
    delete jsGroupBox;
    delete wxtqGroupBox;
    delete multiScrollArea;
    delete ssfRadioButton;
    delete snfRadioButton;
    delete scfRadioButton;
    delete sblfRadioButton;
    delete sjgwRadioButton;
    delete sjdwRadioButton;
    delete sqjsRadioButton;
    delete singleScrollArea;
    delete executeButton;
    delete exportButton;
    delete pgDb;
}

void KfttjInputWidget::initData(){
    //初始化DB
    pgDb = new PgDataBase;
    //多要素
    wpHash.clear();
    QString queryStr = QString("select * from weatherparam order by choose_type,id");
    QSqlQueryModel *plainModel = pgDb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        WeatherParam weatherParam;
        weatherParam.setId(plainModel->record(i).value(0).toInt());
        weatherParam.setName(plainModel->record(i).value(1).toString());
        weatherParam.setChoose_type(plainModel->record(i).value(2).toInt());
        weatherParam.setLimit_type(plainModel->record(i).value(3).toInt());
        if(weatherParam.choose_type() == 0){
            wpHash[0].append(weatherParam);
        }else{
            wpHash[1].append(weatherParam);
        }

    }
    delete plainModel;
}

void KfttjInputWidget::initUI(){
    //设置宽度
    this->setMinimumWidth(290);
    //机场
    airportComboBox = new QComboBox;
    queryAirport();

    QVBoxLayout *airportLayout = new QVBoxLayout;
    airportLayout->addWidget(airportComboBox);

    QGroupBox *airportGroupBox = new QGroupBox;
    airportGroupBox->setTitle("机场");
    airportGroupBox->setLayout(airportLayout);

    //日期
    dateLayout = new QGridLayout;

    QGroupBox *dateGroupBox = new QGroupBox;
    dateGroupBox->setTitle("日期");
    dateGroupBox->setLayout(dateLayout);

    if(airportComboBox->count() > 0){
        onAirportChanged(0);
    }
    //气象要素
    qxysComboBox = new QComboBox;
    qxysComboBox->insertItem(0, "多要素");
    qxysComboBox->insertItem(1, "单要素");
    //气象阀值滚动区域
    multiScrollArea = new QScrollArea;
    QWidget *multiScrollWidget = new QWidget;
    //风
    fqfGroupBox = new QGroupBox;
    fqfGroupBox->setTitle("非强风");
    fqfGroupBox->setCheckable(true);
    fqfGroupBox->setChecked(false);

    slfRadioButton = new QRadioButton;
    slfRadioButton->setText("矢量风");
    slfRadioButton->setChecked(true);

    sfCheckBox = new QCheckBox;
    sfCheckBox->setText("顺风");
    sfCheckBox->setChecked(true);
    nfCheckBox = new QCheckBox;
    nfCheckBox->setText("逆风");
    nfCheckBox->setChecked(true);
    cfCheckBox = new QCheckBox;
    cfCheckBox->setText("侧风");
    cfCheckBox->setChecked(true);

    QGridLayout *fqfGLayout = new QGridLayout;
    fqfGLayout->addWidget(sfCheckBox, 0, 0);
    fqfGLayout->addWidget(nfCheckBox, 0, 1);
    fqfGLayout->addWidget(cfCheckBox, 0, 2);

    blfRadioButton = new QRadioButton;
    blfRadioButton->setText("标量风");

    QVBoxLayout *fqfLayout = new QVBoxLayout;
    fqfLayout->addWidget(slfRadioButton);
    fqfLayout->addLayout(fqfGLayout);
    fqfLayout->addWidget(blfRadioButton);

    fqfGroupBox->setLayout(fqfLayout);
    //温度
    wdGroupBox = new QGroupBox;
    wdGroupBox->setTitle("温度");
    wdGroupBox->setCheckable(true);
    wdGroupBox->setChecked(false);

    cgwRadioButton = new QRadioButton;
    cgwRadioButton->setText("常规温");
    cgwRadioButton->setChecked(true);

    QVBoxLayout *wdLayout = new QVBoxLayout;
    wdLayout->addWidget(cgwRadioButton);

    wdGroupBox->setLayout(wdLayout);
    //气压
    qyGroupBox = new QGroupBox;
    qyGroupBox->setTitle("气压");
    qyGroupBox->setCheckable(true);
    qyGroupBox->setChecked(false);

    cmqyRadioButton = new QRadioButton;
    cmqyRadioButton->setText("场面气压");
    cmqyRadioButton->setChecked(true);

    xzhmqyRadioButton = new QRadioButton;
    xzhmqyRadioButton->setText("修正海面气压");

    QVBoxLayout *qyLayout = new QVBoxLayout;
    qyLayout->addWidget(cmqyRadioButton);
    qyLayout->addWidget(xzhmqyRadioButton);

    qyGroupBox->setLayout(qyLayout);
    //湿度
    sdGroupBox = new QGroupBox;
    sdGroupBox->setTitle("湿度");
    sdGroupBox->setCheckable(true);
    sdGroupBox->setChecked(false);

    cgsRadioButton = new QRadioButton;
    cgsRadioButton->setText("常规湿");
    cgsRadioButton->setChecked(true);
    jsRadioButton = new QRadioButton;
    jsRadioButton->setText("极湿");

    QVBoxLayout *sdLayout = new QVBoxLayout;
    sdLayout->addWidget(cgsRadioButton);
    sdLayout->addWidget(jsRadioButton);

    sdGroupBox->setLayout(sdLayout);
    //能见度
    njdGroupBox = new QGroupBox;
    njdGroupBox->setTitle("能见度");
    njdGroupBox->setCheckable(true);
    njdGroupBox->setChecked(false);

    spnjdRadioButton = new QRadioButton;
    spnjdRadioButton->setText("水平能见度");
    spnjdRadioButton->setChecked(true);

    QVBoxLayout *njdLayout = new QVBoxLayout;
    njdLayout->addWidget(spnjdRadioButton);

    njdGroupBox->setLayout(njdLayout);
    //云
    yGroupBox = new QGroupBox;
    yGroupBox->setTitle("云");
    yGroupBox->setCheckable(true);
    yGroupBox->setChecked(false);

    ylygRadioButton = new QRadioButton;
    ylygRadioButton->setText("云量云高");
    ylygRadioButton->setChecked(true);

    QVBoxLayout *yLayout = new QVBoxLayout;
    yLayout->addWidget(ylygRadioButton);

    yGroupBox->setLayout(yLayout);
    //降水
    jsGroupBox = new QGroupBox;
    jsGroupBox->setTitle("降水");
    jsGroupBox->setCheckable(true);
    jsGroupBox->setChecked(false);

    fqjsRadioButton = new QRadioButton;
    fqjsRadioButton->setText("非强降水");
    fqjsRadioButton->setChecked(true);

    QVBoxLayout *jsLayout = new QVBoxLayout;
    jsLayout->addWidget(fqjsRadioButton);

    jsGroupBox->setLayout(jsLayout);
    //危险天气
    wxtqGroupBox = new QGroupBox;
    wxtqGroupBox->setTitle("危险天气");
    wxtqGroupBox->setCheckable(true);
    wxtqGroupBox->setChecked(false);

    njyCheckBox = new QCheckBox;
    njyCheckBox->setText("浓积云");
    njyCheckBox->setChecked(true);
    jyyCheckBox = new QCheckBox;
    jyyCheckBox->setText("积雨云");
    jyyCheckBox->setChecked(true);
    sdCheckBox = new QCheckBox;
    sdCheckBox->setText("闪电");
    sdCheckBox->setChecked(true);
    lbCheckBox = new QCheckBox;
    lbCheckBox->setText("雷暴");
    lbCheckBox->setChecked(true);
    bbCheckBox = new QCheckBox;
    bbCheckBox->setText("冰雹");
    bbCheckBox->setChecked(true);
    bxCheckBox = new QCheckBox;
    bxCheckBox->setText("飑线");
    bxCheckBox->setChecked(true);
    ljCheckBox = new QCheckBox;
    ljCheckBox->setText("龙卷");
    ljCheckBox->setChecked(true);
    scbCheckBox = new QCheckBox;
    scbCheckBox->setText("沙尘暴");
    scbCheckBox->setChecked(true);
    fqbCheckBox = new QCheckBox;
    fqbCheckBox->setText("风切变");
    fqbCheckBox->setChecked(true);

    QGridLayout *wxtqLayout = new QGridLayout;
    wxtqLayout->addWidget(njyCheckBox, 0, 0);
    wxtqLayout->addWidget(jyyCheckBox, 0, 1);
    wxtqLayout->addWidget(sdCheckBox, 0, 2);
    wxtqLayout->addWidget(lbCheckBox, 1, 0);
    wxtqLayout->addWidget(bbCheckBox, 1, 1);
    wxtqLayout->addWidget(bxCheckBox, 1, 2);
    wxtqLayout->addWidget(ljCheckBox, 2, 0);
    wxtqLayout->addWidget(scbCheckBox, 2, 1);
    wxtqLayout->addWidget(fqbCheckBox, 2, 2);

    wxtqGroupBox->setLayout(wxtqLayout);

    //气象阀值滚动区域布局
    QVBoxLayout *multiScrollLayout = new QVBoxLayout;
    multiScrollLayout->addWidget(fqfGroupBox);
    multiScrollLayout->addWidget(wdGroupBox);
    multiScrollLayout->addWidget(qyGroupBox);
    multiScrollLayout->addWidget(sdGroupBox);
    multiScrollLayout->addWidget(njdGroupBox);
    multiScrollLayout->addWidget(yGroupBox);
    multiScrollLayout->addWidget(jsGroupBox);
    multiScrollLayout->addWidget(wxtqGroupBox);
    multiScrollWidget->setLayout(multiScrollLayout);
    multiScrollArea->setWidget(multiScrollWidget);

    //气象阀值滚动区域
    singleScrollArea = new QScrollArea;
    singleScrollArea->setVisible(false);
    QWidget *singleScrollWidget = new QWidget;
    //强风.矢量风.顺风
    ssfRadioButton = new QRadioButton;
    ssfRadioButton->setText("强风.矢量风.顺风");
    ssfRadioButton->setChecked(true);
    //强风.矢量风.逆风
    snfRadioButton = new QRadioButton;
    snfRadioButton->setText("强风.矢量风.逆风");
    //强风.矢量风.侧风
    scfRadioButton = new QRadioButton;
    scfRadioButton->setText("强风.矢量风.侧风");
    //强风.标量风
    sblfRadioButton = new QRadioButton;
    sblfRadioButton->setText("强风.标量风");
    //温度.极高温
    sjgwRadioButton = new QRadioButton;
    sjgwRadioButton->setText("温度.极高温");
    //温度.极低温
    sjdwRadioButton = new QRadioButton;
    sjdwRadioButton->setText("温度.极低温");
    //强降水
    sqjsRadioButton = new QRadioButton;
    sqjsRadioButton->setText("强降水");

    //气象阀值滚动区域布局
    QVBoxLayout *singleScrollLayout = new QVBoxLayout;
    singleScrollLayout->addWidget(ssfRadioButton);
    singleScrollLayout->addWidget(snfRadioButton);
    singleScrollLayout->addWidget(scfRadioButton);
    singleScrollLayout->addWidget(sblfRadioButton);
    singleScrollLayout->addWidget(sjgwRadioButton);
    singleScrollLayout->addWidget(sjdwRadioButton);
    singleScrollLayout->addWidget(sqjsRadioButton);
    singleScrollWidget->setLayout(singleScrollLayout);
    singleScrollArea->setWidget(singleScrollWidget);

    QVBoxLayout *qxysLayout = new QVBoxLayout;
    qxysLayout->addWidget(qxysComboBox, 1);
    qxysLayout->addWidget(multiScrollArea);
    qxysLayout->addWidget(singleScrollArea);

    QGroupBox *qxysGroupBox = new QGroupBox;
    qxysGroupBox->setTitle("气象要素");
    qxysGroupBox->setLayout(qxysLayout);

    //按钮
    executeButton = new QPushButton;
    executeButton->setText("开始");

    exportButton = new QPushButton;
    exportButton->setText("导出");

    QHBoxLayout *executeLayout = new QHBoxLayout;
    executeLayout->addWidget(executeButton);
    executeLayout->addWidget(exportButton);


    //主界面
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(airportGroupBox);
    mainLayout->addWidget(dateGroupBox);
    mainLayout->addWidget(qxysGroupBox);
    mainLayout->addLayout(executeLayout);
    mainLayout->addStretch(1);
    this->setLayout(mainLayout);
}

void KfttjInputWidget::initConnect(){
    connect(qxysComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onQxysChanged(int)));
    connect(slfRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSlfRadioButtonClicked(bool)));
    connect(executeButton, SIGNAL(clicked()), this, SLOT(execute()));
    connect(exportButton, SIGNAL(clicked()), this, SIGNAL(executeExport()));
    connect(SharedMemory::getInstance()
            , SIGNAL(airportInfoChanged(QList<Airport>,QHash<QString,QList<QString> >))
            , this
            , SLOT(onAirportInfoChanged(QList<Airport>,QHash<QString,QList<QString> >)));
}

/**
 * @brief KfttjInputWidget::queryAirport
 * 查找机场
 */
void KfttjInputWidget::queryAirport(){
    resetAirportComboBox(SharedMemory::getInstance()->getAirportList(), false);
}

/**
 * @brief KfttjInputWidget::onAirportInfoChanged
 * @param airportList
 */
void KfttjInputWidget::onAirportInfoChanged(QList<Airport> airportList, QHash< QString, QList<QString> >){
    resetAirportComboBox(airportList, true);
}

void KfttjInputWidget::resetAirportComboBox(QList<Airport> apList, bool isSave){
    QString currentAirportCode;
    if(airportList.count() > 0){
        QString currentAirportText = airportComboBox->currentText();
        for(Airport airport : airportList){
            if(currentAirportText.compare(airport.name()) == 0){
                currentAirportCode = airport.code();
                break;
            }
        }
    }
    disconnect(airportComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onAirportChanged(int)));
    for(int i = airportComboBox->count() - 1;i >= 0;i--){
        airportComboBox->removeItem(i);
    }

    this->airportList = apList;
    int airportCount = airportList.size();
    for(int i = 0;i < airportCount;i++){
        airportComboBox->insertItem(i, airportList[i].name());
    }
    if(isSave){
        if(!currentAirportCode.isEmpty()){
            for(Airport airport : airportList){
                if(currentAirportCode.compare(airport.code()) == 0){
                    airportComboBox->setCurrentText(airport.name());
                    break;
                }
            }
        }
        if(airportComboBox->currentIndex() > -1){
            onAirportChanged(airportComboBox->currentIndex());
        }
    }
    connect(airportComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onAirportChanged(int)));
}

void KfttjInputWidget::onAirportChanged(int index){
    dateList.clear();
    if(index < airportList.count()){
        Airport airport = airportList[index];
        QString apCode = airport.code();
        QString dateSql = QString("select distinct to_char(datetime, 'yyyy') from %1_monthsummary "
                                     "union "
                                     "select distinct to_char(datetime, 'yyyy') from %1_extremum").arg(apCode);
        QList<QVariant> resList = pgDb->queryVariant(dateSql);
        int resCount = resList.size();
        for(int i = 0;i < resCount;i++){
            dateList.append(resList[i].toString());
        }
    }
    qSort(dateList.begin(), dateList.end());
    resetDateArea();
}

/**
 * @brief KfttjInputWidget::resetDateArea
 * 充值日期区域
 */
void KfttjInputWidget::resetDateArea(){
    int dateCheckBoxCount = dateCheckBoxList.size();
    for(int i = 0;i < dateCheckBoxCount;i++){
        QCheckBox *checkBox = dateCheckBoxList[i];
        delete checkBox;
    }
    dateCheckBoxList.clear();

    int dateCount = dateList.size();
    for(int i = 0;i < dateCount;i++){
        QCheckBox *checkBox = new QCheckBox(dateList[i]);
        dateLayout->addWidget(checkBox, i / 3 , i % 3);
        dateCheckBoxList.append(checkBox);
    }
}

void KfttjInputWidget::onQxysChanged(int index){
    if(index == 0){
        this->multiScrollArea->setVisible(true);
        this->singleScrollArea->setVisible(false);
    }else{
        this->multiScrollArea->setVisible(false);
        this->singleScrollArea->setVisible(true);
    }
}

void KfttjInputWidget::onSlfRadioButtonClicked(bool isChecked){
    if(isChecked){
        sfCheckBox->setDisabled(false);
        nfCheckBox->setDisabled(false);
        cfCheckBox->setDisabled(false);
    }else{
        sfCheckBox->setDisabled(true);
        nfCheckBox->setDisabled(true);
        cfCheckBox->setDisabled(true);
    }
}

bool KfttjInputWidget::validate(){
    //检查机场
    if(airportComboBox->count() == 0){
        QMessageBox::critical(0, QObject::tr("错误提示"), "必须选择一个机场才能进行可飞天统计!\n请在机场设置里添加一个机场.");
        return false;
    }
    //检查日期
    if(dateCheckBoxList.count() == 0){
        QMessageBox::critical(0, QObject::tr("错误提示"), "必须选择一个日期才能进行可飞天统计!\n请在数据导入里导入月总簿.");
        return false;
    }
    bool isDateChecked = false;
    for(QCheckBox *checkBox : dateCheckBoxList){
        if(checkBox->isChecked()){
            isDateChecked = true;
            break;
        }
    }
    if(!isDateChecked){
        QMessageBox::critical(0, QObject::tr("错误提示"), "必须选择一个日期才能进行可飞天统计!");
        return false;
    }
    //检查气象要素
    if(qxysComboBox->currentIndex() == 0){
        bool isQxysChecked = false;
        //非强风
        if(fqfGroupBox->isChecked()){
            if(slfRadioButton->isChecked()){
                if(sfCheckBox->isChecked() || nfCheckBox->isChecked() || cfCheckBox->isChecked()){
                    isQxysChecked = true;
                }
            }else{
                isQxysChecked = true;
            }
        }
        if(isQxysChecked){
            return true;
        }
        //温度
        if(wdGroupBox->isChecked()){
            if(cgwRadioButton->isChecked()){
                isQxysChecked = true;
            }
        }
        if(isQxysChecked){
            return true;
        }
        //气压
        if(qyGroupBox->isChecked()){
            isQxysChecked = true;
        }
        if(isQxysChecked){
            return true;
        }
        //湿度
        if(sdGroupBox->isChecked()){
            isQxysChecked = true;
        }
        if(isQxysChecked){
            return true;
        }
        //能见度
        if(njdGroupBox->isChecked()){
            if(spnjdRadioButton->isChecked()){
                isQxysChecked = true;
            }
        }
        if(isQxysChecked){
            return true;
        }
        //云
        if(yGroupBox->isChecked()){
            if(ylygRadioButton->isChecked()){
                isQxysChecked = true;
            }
        }
        if(isQxysChecked){
            return true;
        }
        //降水
        if(jsGroupBox->isChecked()){
            if(fqjsRadioButton->isChecked()){
                isQxysChecked = true;
            }
        }
        if(isQxysChecked){
            return true;
        }
        //危险天气
        if(wxtqGroupBox->isChecked()){
            if(njyCheckBox->isChecked() || jyyCheckBox->isChecked()
                    || sdCheckBox->isChecked() || lbCheckBox->isChecked()
                    || bbCheckBox->isChecked() || bxCheckBox->isChecked()
                    || ljCheckBox->isChecked() || scbCheckBox->isChecked()
                    || fqbCheckBox->isChecked()){
                isQxysChecked = true;
            }
        }
        if(isQxysChecked){
            return true;
        }
        QMessageBox::critical(0, QObject::tr("错误提示"), "必须选择一个气象要素才能进行可飞天统计!");
        return false;
    }

    return true;
}

void KfttjInputWidget::execute(){
    if(validate()){
        //机场
        QList<Airport> airportList = SharedMemory::getInstance()->getAirportList();
        QString airportCode = airportList[airportComboBox->currentIndex()].code();
        //日期
        QList<QString> dateList;
        for(QCheckBox *checkBox : dateCheckBoxList){
            if(checkBox->isChecked()){
                dateList.append(checkBox->text());
            }
        }
        //气象要素
        bool isMultiWeather = true;
        QList<WeatherParam> wpList;
        if(qxysComboBox->currentIndex() == 0){
            isMultiWeather = true;
            QList<WeatherParam> weatherParamList = wpHash[1];
            //非强风
            if(fqfGroupBox->isChecked()){
                if(slfRadioButton->isChecked()){
                    if(sfCheckBox->isChecked()){
                        wpList.append(weatherParamList[0]);
                    }
                    if(nfCheckBox->isChecked()){
                        wpList.append(weatherParamList[1]);
                    }
                    if(cfCheckBox->isChecked()){
                        wpList.append(weatherParamList[2]);
                    }
                }else{
                    wpList.append(weatherParamList[3]);
                }
            }
            //温度
            if(wdGroupBox->isChecked()){
                if(cgwRadioButton->isChecked()){
                    wpList.append(weatherParamList[4]);
                }
            }
            //气压
            if(qyGroupBox->isChecked()){
                if(cmqyRadioButton->isChecked()){
                    wpList.append(weatherParamList[5]);
                }else{
                    wpList.append(weatherParamList[6]);
                }
            }
            //湿度
            if(sdGroupBox->isChecked()){
                if(cgsRadioButton->isChecked()){
                    wpList.append(weatherParamList[7]);
                }else{
                    wpList.append(weatherParamList[8]);
                }
            }
            //能见度
            if(njdGroupBox->isChecked()){
                if(spnjdRadioButton->isChecked()){
                    wpList.append(weatherParamList[9]);
                }
            }
            //云
            if(yGroupBox->isChecked()){
                if(ylygRadioButton->isChecked()){
                    wpList.append(weatherParamList[10]);
                }
            }
            //降水
            if(jsGroupBox->isChecked()){
                if(fqjsRadioButton->isChecked()){
                    wpList.append(weatherParamList[11]);
                }
            }
            //危险天气
            if(wxtqGroupBox->isChecked()){
                if(njyCheckBox->isChecked()){
                    wpList.append(weatherParamList[12]);
                }
                if(jyyCheckBox->isChecked()){
                    wpList.append(weatherParamList[13]);
                }
                if(sdCheckBox->isChecked()){
                    wpList.append(weatherParamList[14]);
                }
                if(lbCheckBox->isChecked()){
                    wpList.append(weatherParamList[15]);
                }
                if(bbCheckBox->isChecked()){
                    wpList.append(weatherParamList[16]);
                }
                if(bxCheckBox->isChecked()){
                    wpList.append(weatherParamList[17]);
                }
                if(ljCheckBox->isChecked()){
                    wpList.append(weatherParamList[18]);
                }
                if(scbCheckBox->isChecked()){
                    wpList.append(weatherParamList[19]);
                }
                if(fqbCheckBox->isChecked()){
                    wpList.append(weatherParamList[20]);
                }
            }
        }else{
            isMultiWeather = false;
            QList<WeatherParam> weatherParamList = wpHash[0];
            if(ssfRadioButton->isChecked()){
                wpList.append(weatherParamList[0]);
            }
            if(snfRadioButton->isChecked()){
                wpList.append(weatherParamList[1]);
            }
            if(scfRadioButton->isChecked()){
                wpList.append(weatherParamList[2]);
            }
            if(sblfRadioButton->isChecked()){
                wpList.append(weatherParamList[3]);
            }
            if(sjgwRadioButton->isChecked()){
                wpList.append(weatherParamList[4]);
            }
            if(sjdwRadioButton->isChecked()){
                wpList.append(weatherParamList[5]);
            }
            if(sqjsRadioButton->isChecked()){
                wpList.append(weatherParamList[6]);
            }
        }
        emit executeKfttj(airportCode, dateList, isMultiWeather, wpList);
    }
}


