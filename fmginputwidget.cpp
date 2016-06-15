#include "fmginputwidget.h"
#include "common/sharedmemory.h"

FmgInputWidget::FmgInputWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

FmgInputWidget::~FmgInputWidget(){
    delete pgdb;
    delete airportComboBox;
    delete runwayComboBox;
    delete fspeedEdit;
    delete tspeedEdit;
    int dateCheckCount = dateCheckBoxList.size();
    for(int i = 0;i < dateCheckCount;i++){
        QCheckBox *checkBox = dateCheckBoxList[i];
        delete checkBox;
    }
    delete dateLayout;
    delete executeButton;
}

void FmgInputWidget::initData(){
    pgdb = new PgDataBase;
}

void FmgInputWidget::initUI(){
    //机场(机场和跑道)
    queryAirport();
    airportComboBox = new QComboBox;
    runwayComboBox = new QComboBox;

    QHBoxLayout *airportLayout = new QHBoxLayout;
    airportLayout->addWidget(airportComboBox, 1);
    airportLayout->addWidget(runwayComboBox);

    QGroupBox *airportGroup = new QGroupBox;
    airportGroup->setTitle("机场");
    airportGroup->setLayout(airportLayout);

    //风速
    fspeedEdit = new QLineEdit;
    fspeedEdit->setFixedWidth(70);
    fspeedEdit->setValidator(new QIntValidator(0, 1000, this));
    QLabel *speedLabel = new QLabel;
    speedLabel->setText(" ~ ");
    tspeedEdit = new QLineEdit;
    tspeedEdit->setFixedWidth(70);
    tspeedEdit->setValidator(new QIntValidator(0, 1000, this));

    QHBoxLayout *speedLayout = new QHBoxLayout;
    speedLayout->addWidget(fspeedEdit);
    speedLayout->addStretch(1);
    speedLayout->addWidget(speedLabel);
    speedLayout->addStretch(1);
    speedLayout->addWidget(tspeedEdit);

    QGroupBox *speedGroup = new QGroupBox;
    speedGroup->setTitle("风速");
    speedGroup->setLayout(speedLayout);


    dateLayout = new QGridLayout;

    QGroupBox *dateGroup = new QGroupBox;
    dateGroup->setTitle("日期");
    dateGroup->setLayout(dateLayout);

    //执行按钮
    executeButton = new QPushButton;
    executeButton->setText("开始");

    QHBoxLayout *executeLayout = new QHBoxLayout;
    executeLayout->addStretch(1);
    executeLayout->addWidget(executeButton);

    //主界面
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(airportGroup);
    mainLayout->addWidget(speedGroup);
    mainLayout->addWidget(dateGroup);
    mainLayout->addLayout(executeLayout);
    mainLayout->addStretch(1);
    this->setLayout(mainLayout);

    //最后执行
    resetAirportComboBox(this->airportList, this->runwayHash, false);
}

void FmgInputWidget::initConnect(){
   connect(airportComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onAirportChanged(const QString &)));
   connect(runwayComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onRunwayChanged(const QString &)));
   connect(executeButton, SIGNAL(clicked()), this, SLOT(execute()));
   connect(SharedMemory::getInstance()
           , SIGNAL(airportInfoChanged(QList<Airport>,QHash<QString,QList<QString> >))
           , this
           , SLOT(onAirportInfoChanged(QList<Airport>,QHash<QString,QList<QString> >)));
}

/**
 * @brief FmgInputWidget::onAirportChanged
 * 机场变化时对应的跑道也变化对应的日期也变化
 * @param name
 */
void FmgInputWidget::onAirportChanged(const QString &name){
    int lastRunwayCount = runwayComboBox->count();
    for(int i = lastRunwayCount - 1;i >= 0;i--){
        runwayComboBox->removeItem(i);
    }
    QString key = name;
    if(runwayHash.contains(key)){
        QList<QString> runwayList = runwayHash[key];
        int runwayCount = runwayList.size();
        for(int i = 0;i < runwayCount;i++){
            runwayComboBox->insertItem(i, runwayList[i]);
        }
    }
    queryDate();
    resetDateArea();
}

/**
 * @brief FmgInputWidget::onRunwayChanged
 * 跑道变化时对应的日期也变化
 * @param runway
 */
void FmgInputWidget::onRunwayChanged(const QString &runway){
    Q_UNUSED(runway);
    queryDate();
    resetDateArea();
}

/**
 * @brief FmgInputWidget::resetDateArea
 * 充值日期区域
 */
void FmgInputWidget::resetDateArea(){
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

/**
 * @brief FmgInputWidget::queryAirport
 * 查找机场
 */
void FmgInputWidget::queryAirport(){
    airportList = SharedMemory::getInstance()->getAirportList();
    runwayHash = SharedMemory::getInstance()->getRunwayHash();
}

/**
 * @brief FmgInputWidget::queryDate
 * 查找对应机场对应跑道的时间
 */
void FmgInputWidget::queryDate(){
    dateList.clear();
    int airportIndex = airportComboBox->currentIndex();
    QString codeStr = airportList[airportIndex].code();
    QString runwayStr = runwayComboBox->currentText();
    if(!codeStr.isEmpty() && !runwayStr.isEmpty()){
        QString queryStr = QString("select distinct to_char(datetime, 'yyyy') as year from %1_automaticwind where runwayno = '%2' order by year desc").arg(codeStr).arg(runwayStr);
        QList<QVariant> resList = pgdb->queryVariant(queryStr);
        int resCount = resList.size();
        for(int i = 0;i < resCount;i++){
            dateList.append(resList[i].toString());
        }
    }
}

/**
 * @brief FmgInputWidget::validate
 * 输入check
 * @return
 */
bool FmgInputWidget::validate(){
    return true;
}

/**
 * @brief FmgInputWidget::resetAirportComboBox
 * @param airportList
 * @param isSave
 */
void FmgInputWidget::resetAirportComboBox(QList<Airport> apList, QHash< QString, QList<QString> > rwHash, bool isSave){
    if(isSave){
        disconnect(airportComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onAirportChanged(const QString &)));
        disconnect(runwayComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onRunwayChanged(const QString &)));

        QString currentCode("");
        QString currentRunway("");
        if(airportList.count() > 0){
            currentCode = airportList[airportComboBox->currentIndex()].code();
            currentRunway = runwayComboBox->currentText();
        }

        airportComboBox->clear();
        runwayComboBox->clear();

        this->airportList = apList;
        this->runwayHash = rwHash;

        int airportCount = airportList.size();
        int currentIndex = 0;
        for(int i = 0;i < airportCount;i++){
            airportComboBox->insertItem(i, airportList[i].name());
            if(airportList[i].code().compare(currentCode) == 0){
               currentIndex = i;
            }
        }
        airportComboBox->setCurrentIndex(currentIndex);

        if(airportCount > 0){
            QString key = airportList[currentIndex].name();
            if(runwayHash.contains(key)){
                QList<QString> runwayList = runwayHash[key];
                int runwayCount = runwayList.size();
                for(int i = 0;i < runwayCount;i++){
                    runwayComboBox->insertItem(i, runwayList[i]);
                }
                runwayComboBox->setCurrentText(currentRunway);
            }
        }

        connect(airportComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onAirportChanged(const QString &)));
        connect(runwayComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onRunwayChanged(const QString &)));
    }else{
        this->airportList = apList;
        this->runwayHash = rwHash;

        int airportCount = airportList.size();
        for(int i = 0;i < airportCount;i++){
            airportComboBox->insertItem(i, airportList[i].name());
        }

        if(airportCount > 0){
            QString key = airportList[0].name();
            if(runwayHash.contains(key)){
                QList<QString> runwayList = runwayHash[key];
                int runwayCount = runwayList.size();
                for(int i = 0;i < runwayCount;i++){
                    runwayComboBox->insertItem(i, runwayList[i]);
                }
            }
        }
    }
    this->onRunwayChanged("");
}


/**
 * @brief FmgInputWidget::execute
 * 开始风玫瑰模块的计算
 */
void FmgInputWidget::execute(){
    if(!validate()){
        return;
    }
    QString code = airportList[airportComboBox->currentIndex()].code();
    QString runway = runwayComboBox->currentText();
    QString fspeed = fspeedEdit->text();
    QString tspeed = tspeedEdit->text();
    QList<QString> years;
    int dateCheckBoxCount = dateCheckBoxList.size();
    for(int i = 0;i < dateCheckBoxCount;i++){
        QCheckBox *checkBox = dateCheckBoxList[i];
        if(checkBox->isChecked()){
            years.append(checkBox->text());
        }
    }
    emit executeFmg(code, runway, fspeed, tspeed, years);
}

void FmgInputWidget::onAirportInfoChanged(QList<Airport> airportList, QHash< QString, QList<QString> > runwayHash){
    resetAirportComboBox(airportList, runwayHash, true);
}
