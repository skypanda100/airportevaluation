#include "rckqinputwidget.h"

RckqInputWidget::RckqInputWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

RckqInputWidget::~RckqInputWidget(){
    delete pgdb;
    delete airportComboBox;
    delete runwayComboBox;
    delete typeComboBox;
    delete fhourComboBox;
    delete thourComboBox;
    delete addDateButton;
    int dateEditCount = dateEditList.size();
    for(int i = 0;i < dateEditCount;i++){
        QLineEdit *dateEdit = dateEditList[i];
        delete dateEdit;
    }
    delete dateGLayout;
    int weatherCheckBoxCount = weatherCheckBoxList.size();
    for(int i = 0;i < weatherCheckBoxCount;i++){
        QCheckBox *checkBox = weatherCheckBoxList[i];
        delete checkBox;
    }
    int weatherRadioButtonCount = weatherRadioButtonList.size();
    for(int i = 0;i < weatherRadioButtonCount;i++){
        QRadioButton *radioButton = weatherRadioButtonList[i];
        delete radioButton;
    }
    delete weatherGLayout;
    delete executeButton;
}

void RckqInputWidget::initData(){
    pgdb = new PgDataBase;
    weatherList << "全风速"
                << "侧风速"
                << "顺风速"
                << "逆风速"
                << "气温"
                << "比湿";
}

void RckqInputWidget::initUI(){
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
    //类型（单日两要素，多日单要素）
    typeComboBox = new QComboBox;
    typeComboBox->insertItem(0, "单日两要素");
    typeComboBox->insertItem(1, "多日单要素");

    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(typeComboBox, 1);

    QGroupBox *typeGroup = new QGroupBox;
    typeGroup->setTitle("类型");
    typeGroup->setLayout(typeLayout);
    //日期
    fhourComboBox = new QComboBox;
    for(int i = 0;i < 24;i++){
        fhourComboBox->insertItem(i, QString("%1").arg(i));
    }

    QLabel *hourLabel = new QLabel;
    hourLabel->setText(" ~ ");

    thourComboBox = new QComboBox;
    for(int i = 0;i < 24;i++){
        thourComboBox->insertItem(i, QString("%1").arg(i));
    }

    QHBoxLayout *hourLayout = new QHBoxLayout;
    hourLayout->addWidget(fhourComboBox);
    hourLayout->addStretch(1);
    hourLayout->addWidget(hourLabel);
    hourLayout->addStretch(1);
    hourLayout->addWidget(thourComboBox);

    QLineEdit *dateEdit = new QLineEdit;
    dateEditList.append(dateEdit);
    addDateButton = new QPushButton;
    addDateButton->setVisible(false);
    addDateButton->setToolTip("添加日期");
    addDateButton->setIcon(QIcon(":/images/plus.png"));
    addDateButton->setFixedSize(25, 25);

    dateGLayout = new QGridLayout;
    dateGLayout->addLayout(hourLayout, 0, 0);
    dateGLayout->addWidget(dateEdit, 1, 0);
    dateGLayout->addWidget(addDateButton, 1, 1);

    QGroupBox *dateGroup = new QGroupBox;
    dateGroup->setTitle("日期");
    dateGroup->setLayout(dateGLayout);
    //气象要素
    weatherGLayout = new QGridLayout;
    int weatherCount = weatherList.size();
    int radioRow = (int)(((qreal)weatherCount) / 2.0 + 0.5);
    for(int i = 0;i < weatherCount;i++){
        QCheckBox *checkBox = new QCheckBox;
        checkBox->setText(weatherList[i]);
        weatherCheckBoxList.append(checkBox);
        weatherGLayout->addWidget(checkBox, i / 2, i % 2);

        QRadioButton *radioButton = new QRadioButton;
        radioButton->setText(weatherList[i]);
        radioButton->setVisible(false);
        weatherRadioButtonList.append(radioButton);
        weatherGLayout->addWidget(radioButton, i / 2 + radioRow, i % 2);
    }
    QGroupBox *weatherGroup = new QGroupBox;
    weatherGroup->setTitle("气象要素");
    weatherGroup->setLayout(weatherGLayout);
    //执行按钮
    executeButton = new QPushButton;
    executeButton->setText("开始");

    QHBoxLayout *executeLayout = new QHBoxLayout;
    executeLayout->addStretch(1);
    executeLayout->addWidget(executeButton);

    //主界面
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(airportGroup);
    mainLayout->addWidget(typeGroup);
    mainLayout->addWidget(dateGroup);
    mainLayout->addWidget(weatherGroup);
    mainLayout->addLayout(executeLayout);
    mainLayout->addStretch(1);
    this->setLayout(mainLayout);
}

void RckqInputWidget::initConnect(){
    connect(airportComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onAirportChanged(const QString &)));
    connect(typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged(int)));
    connect(addDateButton, SIGNAL(clicked()), this, SLOT(onAddDateClicked()));
    connect(executeButton, SIGNAL(clicked()), this, SLOT(execute()));
}

/**
 * @brief FmgInputWidget::onAirportChanged
 * 机场变化时对应的跑道也变化对应的日期也变化
 * @param name
 */
void RckqInputWidget::onAirportChanged(const QString &name){
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
}

/**
 * @brief RckqInputWidget::onTypeChanged
 * 类型变化时对应的日期和气象要素也变化
 * @param index
 */
void RckqInputWidget::onTypeChanged(int index){
    if(index == 0){
        addDateButton->setVisible(false);
        int dateEditCount = dateEditList.size();
        for(int i = 1;i < dateEditCount;i++){
            QLineEdit *dateEdit = dateEditList[i];
            dateEdit->setVisible(false);
        }

        int weatherCount = weatherList.size();
        for(int i = 0;i < weatherCount;i++){
            weatherRadioButtonList[i]->setVisible(false);
        }
        for(int i = 0;i < weatherCount;i++){
            weatherCheckBoxList[i]->setVisible(true);
        }
    }else{
        addDateButton->setVisible(true);
        int dateEditCount = dateEditList.size();
        for(int i = 1;i < dateEditCount;i++){
            QLineEdit *dateEdit = dateEditList[i];
            dateEdit->setVisible(true);
        }

        int weatherCount = weatherList.size();
        for(int i = 0;i < weatherCount;i++){
            weatherCheckBoxList[i]->setVisible(false);
        }
        for(int i = 0;i < weatherCount;i++){
            weatherRadioButtonList[i]->setVisible(true);
        }
    }
}

/**
 * @brief RckqInputWidget::onAddDateClicked
 * 添加日期edit并且加号移动位置
 */
void RckqInputWidget::onAddDateClicked(){
    QLineEdit *dateEdit = new QLineEdit;
    dateEditList.append(dateEdit);
    dateGLayout->removeWidget(addDateButton);
    dateGLayout->addWidget(dateEdit, dateEditList.size(), 0);
    dateGLayout->addWidget(addDateButton, dateEditList.size(), 1);
}

/**
 * @brief RckqInputWidget::queryAirport
 * 查找机场
 */
void RckqInputWidget::queryAirport(){
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
 * @brief RckqInputWidget::queryRunway
 * 查找机场跑道
 * @param codeStr
 * @return
 */
QList<QString> RckqInputWidget::queryRunway(QString codeStr){
    QList<QString> runwayList;
    //查找自动站风表
    QString queryStr = QString("select distinct runwayno from %1_automaticwind").arg(codeStr);
    QList<QVariant> resList = pgdb->queryVariant(queryStr);
    int resCount = resList.size();
    for(int i = 0;i < resCount;i++){
        runwayList.append(resList[i].toString());
    }
    //查找温度气压表
    queryStr = QString("select distinct runwayno from %1_automatictemperature").arg(codeStr);
    resList = pgdb->queryVariant(queryStr);
    resCount = resList.size();
    for(int i = 0;i < resCount;i++){
        if(!runwayList.contains(resList[i].toString())){
            runwayList.append(resList[i].toString());
        }
    }
    return runwayList;
}

/**
 * @brief RckqInputWidget::validate
 * 输入check
 * @return
 */
bool RckqInputWidget::validate(){
    return true;
}

void RckqInputWidget::execute(){

}
