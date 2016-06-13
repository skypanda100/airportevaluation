#include "airportsetupwidget.h"

/*** 机场添加widget ***/
AirportAddWidget::AirportAddWidget(QList<Airport> airportList, QWidget *parent)
    :QWidget(parent)
    ,airportList(airportList)
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
}

void AirportAddWidget::initData(){
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

void AirportAddWidget::onConfirmClicked(){
    if(validate()){

    }
}

/*** 机场修改widget ***/
AirportModifyWidget::AirportModifyWidget(QList<Airport> airportList, QWidget *parent)
    :QWidget(parent)
    ,airportList(airportList)
{
    this->initData();
    this->initUI();
    this->initConnect();
    this->onCodeChanged(this->codeComboBox->currentIndex());
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
}

void AirportModifyWidget::initData(){
    //机场类型
    typeList.clear();
    typeList.append("普通民用");
    typeList.append("普通军用");
    typeList.append("普通合用");
    typeList.append("其他类型");
    //机场code
    codeList.clear();
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
}

void AirportModifyWidget::onCodeChanged(int index){
    Airport airport = airportList[index];
    nameEdit->setText(airport.name());
    lonEdit->setText(QString("%1").arg(airport.longitude()));
    latEdit->setText(QString("%1").arg(airport.latitude()));
    altEdit->setText(QString("%1").arg(airport.altitude()));
    dirEdit->setText(QString("%1").arg(airport.direction()));
    typeComboBox->setCurrentText(airport.type());
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
    delete tabWidget;
    delete airportAddWidget;
    delete airportModifyWidget;
    delete pgDb;
}

void AirportSetupWidget::initData(){
    //初始化DB
    pgDb = new PgDataBase;
    //获取所有机场
    QString tempQueryStr("select * from airport");
    QSqlQueryModel *plainModel = pgDb->queryModel(tempQueryStr);
    int rowCount = plainModel->rowCount();
    if(rowCount > 0){
        for(int i = 0;i < rowCount;i++){
            QString code = plainModel->record(i).value(0).toString();
            QString name = plainModel->record(i).value(1).toString();
            float longitude = plainModel->record(i).value(2).toFloat();
            float latitude = plainModel->record(i).value(3).toFloat();
            float altitude = plainModel->record(i).value(4).toFloat();
            float direction = plainModel->record(i).value(5).toFloat();
            QString type = plainModel->record(i).value(6).toString();

            Airport airport;
            airport.setCode(code);
            airport.setName(name);
            airport.setLongitude(longitude);
            airport.setLatitude(latitude);
            airport.setAltitude(altitude);
            airport.setDirection(direction);
            airport.setType(type);

            airportList.append(airport);
        }
    }else{
        airportList.clear();
    }
    delete plainModel;
}

void AirportSetupWidget::initUI(){
    this->setWindowIcon(QIcon(":/images/airport_setup.png"));
    this->setWindowTitle("机场设置");

    //设置标签
    tabWidget = new QTabWidget;
    airportAddWidget = new AirportAddWidget(this->airportList);
    tabWidget->addTab(airportAddWidget, "增加机场");
    airportModifyWidget = new AirportModifyWidget(this->airportList);
    tabWidget->addTab(airportModifyWidget, "修改机场");

    //布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    this->setLayout(mainLayout);
}

void AirportSetupWidget::initConnect(){

}
