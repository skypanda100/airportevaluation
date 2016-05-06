#include "fmginputwidget.h"

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
    int airportCount = aiportList.size();
    for(int i = 0;i < airportCount;i++){
        airportComboBox->insertItem(i, aiportList[i].name());
    }

    runwayComboBox = new QComboBox;
    if(airportCount > 0){
        QString key = aiportList[0].name();
        if(runwayHash.contains(key)){
            QList<QString> runwayList = runwayHash[key];
            int runwayCount = runwayList.size();
            for(int i = 0;i < runwayCount;i++){
                runwayComboBox->insertItem(i, runwayList[i]);
            }
        }
    }

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

    //日期
    queryDate();

    dateLayout = new QGridLayout;
    resetDateArea();

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
}

void FmgInputWidget::initConnect(){
   connect(airportComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onAirportChanged(const QString &)));
   connect(runwayComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onRunwayChanged(const QString &)));
   connect(executeButton, SIGNAL(clicked()), this, SLOT(execute()));
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
    aiportList.clear();
    runwayHash.clear();
    QString queryStr = QString("select * from airport");
    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        Airport airport;
        airport.setCode(plainModel->record(i).value(0).toString());
        airport.setName(plainModel->record(i).value(1).toString());
        airport.setLongitude(plainModel->record(i).value(2).toFloat());
        airport.setLatitude(plainModel->record(i).value(3).toFloat());
        airport.setAltitude(plainModel->record(i).value(4).toFloat());
        airport.setDirection(plainModel->record(i).value(5).toFloat());
        airport.setType(plainModel->record(i).value(6).toString());
        aiportList.append(airport);

        //查找跑道
        QList<QString> runwayList = queryRunway(airport.code().toLower());
        runwayHash[airport.name()] = runwayList;
    }
    delete plainModel;
}

/**
 * @brief FmgInputWidget::queryRunway
 * 查找机场跑道
 * @param codeStr
 * @return
 */
QList<QString> FmgInputWidget::queryRunway(QString codeStr){
    QList<QString> runwayList;
    QString queryStr = QString("select distinct runwayno from %1_automaticwind").arg(codeStr);
    QList<QVariant> resList = pgdb->queryVariant(queryStr);
    int resCount = resList.size();
    for(int i = 0;i < resCount;i++){
        runwayList.append(resList[i].toString());
    }
    return runwayList;
}

/**
 * @brief FmgInputWidget::queryDate
 * 查找对应机场对应跑道的时间
 */
void FmgInputWidget::queryDate(){
    dateList.clear();
    int airportIndex = airportComboBox->currentIndex();
    QString codeStr = aiportList[airportIndex].code();
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
 * @brief FmgInputWidget::execute
 * 开始风玫瑰模块的计算
 */
void FmgInputWidget::execute(){
    if(!validate()){
        return;
    }
    QString code = aiportList[airportComboBox->currentIndex()].code();
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
