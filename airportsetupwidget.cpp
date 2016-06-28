#include "airportsetupwidget.h"
#include "common/sharedmemory.h"

/*** 机场添加widget ***/
AirportAddWidget::AirportAddWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

AirportAddWidget::~AirportAddWidget(){
    delete codeEdit;
    delete nameEdit;
    delete lonEdit;
    delete latEdit;
    delete altEdit;
    delete dirEdit;
    delete typeComboBox;
    delete confirmButton;
    delete pgDb;
}

void AirportAddWidget::initData(){
    //初始化DB
    pgDb = new PgDataBase;
    //机场类型
    typeList.append("普通民用");
    typeList.append("普通军用");
    typeList.append("普通合用");
    typeList.append("其他类型");
}

void AirportAddWidget::initUI(){
    //机场code
    QLabel *codeLabel = new QLabel;
    codeLabel->setText("机场code");
    codeEdit = new QLineEdit;
    //机场名称
    QLabel *nameLabel = new QLabel;
    nameLabel->setText("机场名称");
    nameEdit = new QLineEdit;
    //经度
    QLabel *lonLabel = new QLabel;
    lonLabel->setText("经度");
    lonEdit = new QLineEdit;
    QRegExp lonRegExp("(^-?180$)|(^-?1[0-7]\\d(\\.\\d{1,2})?$)|(^-?[1-9]\\d(\\.\\d{1,2})?$)|(^-?[1-9](\\.\\d{1,2})?$)|^0$");
    QRegExpValidator *lonValidator = new QRegExpValidator(lonRegExp, this);
    lonEdit->setValidator(lonValidator);
    //纬度
    QLabel *latLabel = new QLabel;
    latLabel->setText("纬度");
    latEdit = new QLineEdit;
    QRegExp latRegExp("^-?(90|[1-8]?\\d(\\.\\d{1,2})?)$");
    QRegExpValidator *latValidator = new QRegExpValidator(latRegExp, this);
    latEdit->setValidator(latValidator);
    //海拔
    QLabel *altLabel = new QLabel;
    altLabel->setText("海拔");
    altEdit = new QLineEdit;
    altEdit->setValidator(new QDoubleValidator(this));
    //跑道方向
    QLabel *dirLabel = new QLabel;
    dirLabel->setText("跑道方向");
    dirEdit = new QLineEdit;
    QRegExp dirRegExp("(^360$)|(^3[0-5]\\d(\\.\\d{1,2})?$)|(^[1-2]\\d{2}(\\.\\d{1,2})?$)|(^[1-9]\\d(\\.\\d{1,2})?$)|(^[1-9](\\.\\d{1,2})?$)|^0$");
    QRegExpValidator *dirValidator = new QRegExpValidator(dirRegExp, this);
    dirEdit->setValidator(dirValidator);
    //机场类型
    QLabel *typeLabel = new QLabel;
    typeLabel->setText("机场类型");
    typeComboBox = new QComboBox;
    typeComboBox->addItems(typeList);

    //确认按钮
    confirmButton = new QPushButton;
    confirmButton->setText("确定");

    //布局
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(codeLabel, 0, 0);
    mainLayout->addWidget(codeEdit, 0, 1);
    mainLayout->addWidget(nameLabel, 1, 0);
    mainLayout->addWidget(nameEdit, 1, 1);
    mainLayout->addWidget(lonLabel, 2, 0);
    mainLayout->addWidget(lonEdit, 2, 1);
    mainLayout->addWidget(latLabel, 3, 0);
    mainLayout->addWidget(latEdit, 3, 1);
    mainLayout->addWidget(altLabel, 4, 0);
    mainLayout->addWidget(altEdit, 4, 1);
    mainLayout->addWidget(dirLabel, 5, 0);
    mainLayout->addWidget(dirEdit, 5, 1);
    mainLayout->addWidget(typeLabel, 6, 0);
    mainLayout->addWidget(typeComboBox, 6, 1);
    mainLayout->addWidget(confirmButton, 7, 0, 1, 2);
    this->setLayout(mainLayout);
}

void AirportAddWidget::initConnect(){
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
}

bool AirportAddWidget::validate(){
    //check机场code
    QString codeStr = codeEdit->text().trimmed().toUpper();
    QRegExp codeRegExp("^([a-zA-Z]{4})$");
    int codePos = codeRegExp.indexIn(codeStr);
    if(codePos < 0){
        QMessageBox::critical(0, QObject::tr("错误提示"), "机场四字码应该为四个字母!");
        return false;
    }
    QList<Airport> airportList = SharedMemory::getInstance()->getAirportList();
    for(Airport airport : airportList){
        if(airport.code().compare(codeStr) == 0){
            QMessageBox::critical(0, QObject::tr("错误提示"), "已经存在该机场四字码!");
            return false;
        }
    }
    //check机场名称
    QString nameStr = nameEdit->text().trimmed();
    if(nameStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "机场名称不能为空!");
        return false;
    }
    //check经度
    QString longitudeStr = lonEdit->text().trimmed();
    if(longitudeStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "经度不能为空!");
        return false;
    }
    //check纬度
    QString latitudeStr = latEdit->text().trimmed();
    if(latitudeStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "纬度不能为空!");
        return false;
    }
    //check海拔
    QString altitudeStr = altEdit->text().trimmed();
    if(altitudeStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "海拔不能为空!");
        return false;
    }
    //check跑道方向
    QString directionStr = dirEdit->text().trimmed();
    if(directionStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "跑道方向不能为空!");
        return false;
    }

    return true;
}

bool AirportAddWidget::createMonthSummary(QString code){
    QString createSql = QString("CREATE TABLE %1_monthsummary("
                                "datetime timestamp(6) without time zone NOT NULL,"
                                "winddirection character varying(50),"
                                "windspeed character varying(50),"
                                "gustspeed character varying(50),"
                                "leadingvisibility character varying(50),"
                                "runway1no character varying(50),"
                                "runway1vr character varying(50),"
                                "runway2no character varying(50),"
                                "runway2vr character varying(50),"
                                "runway3no character varying(50),"
                                "runway3vr character varying(50),"
                                "runway4no character varying(50),"
                                "runway4vr character varying(50),"
                                "runway5no character varying(50),"
                                "runway5vr character varying(50),"
                                "phenomena character varying(50),"
                                "totalcloudcover character varying(50),"
                                "lowcloudcover character varying(50),"
                                "lowcloudstate1 character varying(50),"
                                "lowcloudstate2 character varying(50),"
                                "lowcloudstate3 character varying(50),"
                                "lowcloudstate4 character varying(50),"
                                "lowcloudstate5 character varying(50),"
                                "middlecloudstate1 character varying(50),"
                                "middlecloudstate2 character varying(50),"
                                "middlecloudstate3 character varying(50),"
                                "highcloudstate1 character varying(50),"
                                "highcloudstate2 character varying(50),"
                                "highcloudstate3 character varying(50),"
                                "temperature character varying(50),"
                                "relativehumidity character varying(50),"
                                "dewpoint character varying(50),"
                                "airdromepressure character varying(50),"
                                "correctedseapressure character varying(50),"
                                "CONSTRAINT %1_monthsummary_pkey PRIMARY KEY (datetime))"
                                ).arg(code.toLower());
    if(pgDb->createTable(createSql)){
        return true;
    }else{
        return false;
    }
}

bool AirportAddWidget::createExtremum(QString code){
    QString createSql = QString("CREATE TABLE %1_extremum("
                                "datetime timestamp(6) without time zone NOT NULL,"
                                "hightemperature numeric(5,1),"
                                "lowtemperature numeric(5,1),"
                                "precipitation numeric(5,1),"
                                "maxsnowdepth numeric(5,1),"
                                "evolution1 character varying(255),"
                                "evolution2 character varying(255),"
                                "evolution3 character varying(255),"
                                "evolution4 character varying(255),"
                                "evolution5 character varying(255),"
                                "evolution6 character varying(255),"
                                "evolution7 character varying(255),"
                                "evolution8 character varying(255),"
                                "evolution9 character varying(255),"
                                "evolution10 character varying(255),"
                                "CONSTRAINT %1_extremum_pkey PRIMARY KEY (datetime))"
                                ).arg(code.toLower());
    if(pgDb->createTable(createSql)){
        return true;
    }else{
        return false;
    }
}

bool AirportAddWidget::createAutomaticWind(QString code){
    QString createSql = QString("CREATE TABLE %1_automaticwind("
                                "datetime timestamp(6) without time zone NOT NULL,"
                                "runwayno character varying(5) NOT NULL,"
                                "windspeed numeric(5,2),"
                                "winddirection integer,"
                                "CONSTRAINT %1_automaticwind_pkey PRIMARY KEY (datetime, runwayno))"
                                ).arg(code.toLower());
    if(pgDb->createTable(createSql)){
        return true;
    }else{
        return false;
    }
}

bool AirportAddWidget::createAutomaticTemperature(QString code){
    QString createSql = QString("CREATE TABLE %1_automatictemperature("
                                "datetime timestamp(6) without time zone NOT NULL,"
                                "runwayno character varying(5) NOT NULL,"
                                "airdromepressure numeric(6,2),"
                                "temperature numeric(5,2),"
                                "relativehumidity numeric(4,2),"
                                "dewpoint numeric(5,2),"
                                "CONSTRAINT %1_automatictemperature_pkey PRIMARY KEY (datetime, runwayno))"
                                ).arg(code.toLower());
    if(pgDb->createTable(createSql)){
        return true;
    }else{
        return false;
    }
}

void AirportAddWidget::onConfirmClicked(){
    if(validate()){
        //机场code
        QString code = codeEdit->text().trimmed().toUpper();
        //机场名称
        QString name = nameEdit->text().trimmed();
        //经度
        QString longitude = lonEdit->text();
        //纬度
        QString latitude = latEdit->text();
        //海拔
        QString altitude = altEdit->text();
        //跑道方向
        QString direction = dirEdit->text();
        //机场类型
        QString type = typeComboBox->currentText();
        //构造插入语句
        QString saveAirportSql("insert into airport values(?,?,?,?,?,?,?)");
        QList<QVariant> values;
        values.append(code);
        values.append(name);
        values.append(longitude);
        values.append(latitude);
        values.append(altitude);
        values.append(direction);
        values.append(type);

        bool ret = pgDb->save(saveAirportSql, values);
        if(ret){
            if(createMonthSummary(code)
                    && createExtremum(code)
                    && createAutomaticWind(code)
                    && createAutomaticTemperature(code)){
                QMessageBox::information(0, QObject::tr("消息提示"), "机场保存成功!");
                SharedMemory::getInstance()->queryAirportInfomation();
                emit airportChanged();
            }else{
                QString deleteSql = QString("delete from airport where code = '%1'").arg(code.toUpper());
                pgDb->deleteData(deleteSql);
                QMessageBox::critical(0, QObject::tr("错误提示"), "机场保存失败!");
            }
        }else{
            QMessageBox::critical(0, QObject::tr("错误提示"), "机场保存失败!");
        }
    }
}

/*** 机场修改widget ***/
AirportModifyWidget::AirportModifyWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
    if(this->codeComboBox->count() > 0){
        this->onCodeChanged(this->codeComboBox->currentIndex());
    }
}

AirportModifyWidget::~AirportModifyWidget(){
    delete codeComboBox;
    delete nameEdit;
    delete lonEdit;
    delete latEdit;
    delete altEdit;
    delete dirEdit;
    delete typeComboBox;
    delete confirmButton;
    delete pgDb;
}

void AirportModifyWidget::initData(){
    //初始化DB
    pgDb = new PgDataBase;
    //机场类型
    typeList.clear();
    typeList.append("普通民用");
    typeList.append("普通军用");
    typeList.append("普通合用");
    typeList.append("其他类型");
    //机场code
    codeList.clear();
    airportList = SharedMemory::getInstance()->getAirportList();
    int airportCount = airportList.size();
    for(int i = 0;i < airportCount;i++){
        Airport airport = airportList[i];
        codeList.append(airport.code());
    }
}

void AirportModifyWidget::initUI(){
    //机场code
    QLabel *codeLabel = new QLabel;
    codeLabel->setText("机场code");
    codeComboBox = new QComboBox;
    codeComboBox->addItems(codeList);
    //机场名称
    QLabel *nameLabel = new QLabel;
    nameLabel->setText("机场名称");
    nameEdit = new QLineEdit;
    //经度
    QLabel *lonLabel = new QLabel;
    lonLabel->setText("经度");
    lonEdit = new QLineEdit;
    //纬度
    QLabel *latLabel = new QLabel;
    latLabel->setText("纬度");
    latEdit = new QLineEdit;
    //海拔
    QLabel *altLabel = new QLabel;
    altLabel->setText("海拔");
    altEdit = new QLineEdit;
    //跑道方向
    QLabel *dirLabel = new QLabel;
    dirLabel->setText("跑道方向");
    dirEdit = new QLineEdit;
    //机场类型
    QLabel *typeLabel = new QLabel;
    typeLabel->setText("机场类型");
    typeComboBox = new QComboBox;
    typeComboBox->addItems(typeList);

    //确认按钮
    confirmButton = new QPushButton;
    confirmButton->setText("确定");

    //布局
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(codeLabel, 0, 0);
    mainLayout->addWidget(codeComboBox, 0, 1);
    mainLayout->addWidget(nameLabel, 1, 0);
    mainLayout->addWidget(nameEdit, 1, 1);
    mainLayout->addWidget(lonLabel, 2, 0);
    mainLayout->addWidget(lonEdit, 2, 1);
    mainLayout->addWidget(latLabel, 3, 0);
    mainLayout->addWidget(latEdit, 3, 1);
    mainLayout->addWidget(altLabel, 4, 0);
    mainLayout->addWidget(altEdit, 4, 1);
    mainLayout->addWidget(dirLabel, 5, 0);
    mainLayout->addWidget(dirEdit, 5, 1);
    mainLayout->addWidget(typeLabel, 6, 0);
    mainLayout->addWidget(typeComboBox, 6, 1);
    mainLayout->addWidget(confirmButton, 7, 0, 1, 2);
    this->setLayout(mainLayout);
}

void AirportModifyWidget::initConnect(){
    connect(codeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCodeChanged(int)));
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
}

bool AirportModifyWidget::validate(){
    //check机场名称
    QString nameStr = nameEdit->text().trimmed();
    if(nameStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "机场名称不能为空!");
        return false;
    }
    //check经度
    QString longitudeStr = lonEdit->text().trimmed();
    if(longitudeStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "经度不能为空!");
        return false;
    }
    //check纬度
    QString latitudeStr = latEdit->text().trimmed();
    if(latitudeStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "纬度不能为空!");
        return false;
    }
    //check海拔
    QString altitudeStr = altEdit->text().trimmed();
    if(altitudeStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "海拔不能为空!");
        return false;
    }
    //check跑道方向
    QString directionStr = dirEdit->text().trimmed();
    if(directionStr.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "跑道方向不能为空!");
        return false;
    }

    return true;
}

void AirportModifyWidget::onCodeChanged(int index){
    if(index < 0){
        return;
    }
    Airport airport = airportList[index];
    nameEdit->setText(airport.name());
    lonEdit->setText(QString("%1").arg(airport.longitude()));
    latEdit->setText(QString("%1").arg(airport.latitude()));
    altEdit->setText(QString("%1").arg(airport.altitude()));
    dirEdit->setText(QString("%1").arg(airport.direction()));
    typeComboBox->setCurrentText(airport.type());
}

void AirportModifyWidget::onConfirmClicked(){
    if(validate()){
        //机场code
        QString code = codeComboBox->currentText();
        //机场名称
        QString name = nameEdit->text().trimmed();
        //经度
        QString longitude = lonEdit->text();
        //纬度
        QString latitude = latEdit->text();
        //海拔
        QString altitude = altEdit->text();
        //跑道方向
        QString direction = dirEdit->text();
        //机场类型
        QString type = typeComboBox->currentText();
        //构造插入语句
        QString saveAirportSql("update airport set name=?,longitude=?,latitude=?,altitude=?,direction=?,type=? where code=?");
        QList<QVariant> values;
        values.append(name);
        values.append(longitude);
        values.append(latitude);
        values.append(altitude);
        values.append(direction);
        values.append(type);
        values.append(code);

        bool ret = pgDb->save(saveAirportSql, values);
        if(ret){
            SharedMemory::getInstance()->queryAirportInfomation();
            QMessageBox::information(0, QObject::tr("消息提示"), "机场保存成功!");
        }else{
            QMessageBox::critical(0, QObject::tr("错误提示"), "机场保存失败!");
        }
    }
}

void AirportModifyWidget::onAirportChanged(){
    //删除codeComboBox(解决comboBox在clear时的bug,先断开信号槽,再连接信号槽)
    disconnect(codeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCodeChanged(int)));
    codeComboBox->clear();
    connect(codeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCodeChanged(int)));
    //机场code
    codeList.clear();
    airportList = SharedMemory::getInstance()->getAirportList();
    int airportCount = airportList.size();
    for(int i = 0;i < airportCount;i++){
        Airport airport = airportList[i];
        codeList.append(airport.code());
    }
    //重置codeComboBox
    codeComboBox->addItems(codeList);
    this->onCodeChanged(this->codeComboBox->currentIndex());
}

/*** 机场设置widget ***/
AirportSetupWidget::AirportSetupWidget(QWidget *parent)
    :QDialog(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

AirportSetupWidget::~AirportSetupWidget(){
    delete airportAddWidget;
    delete airportModifyWidget;
    delete tabWidget;
}

void AirportSetupWidget::initData(){
}

void AirportSetupWidget::initUI(){
    this->setWindowIcon(QIcon(":/images/airport_setup.png"));
    this->setWindowTitle("机场设置");

    //设置标签
    tabWidget = new QTabWidget;
    airportAddWidget = new AirportAddWidget;
    tabWidget->addTab(airportAddWidget, "增加机场");
    airportModifyWidget = new AirportModifyWidget;
    tabWidget->addTab(airportModifyWidget, "修改机场");

    //布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    this->setLayout(mainLayout);
}

void AirportSetupWidget::initConnect(){
    connect(airportAddWidget, SIGNAL(airportChanged()), airportModifyWidget, SLOT(onAirportChanged()));
}
