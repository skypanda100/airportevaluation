#include "weatherparamsetupwidget.h"
#include "common/sharedmemory.h"

/*** 多要素阀值设置widget ***/
MultiWeatherParamWidget::MultiWeatherParamWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

MultiWeatherParamWidget::~MultiWeatherParamWidget(){
    delete pgDb;
    delete confirmButton;
}

void MultiWeatherParamWidget::onAirportChanged(QString apCode){
    //清空
    QList<int> keyList = editHash.keys();
    for(int key : keyList){
        QList<QLineEdit *> editList = editHash[key];
        for(QLineEdit *edit : editList){
            edit->setText("");
        }
    }

    //赋值
    currentApCode = apCode;
    weatherParamSetupList.clear();
    QString queryStr = QString("select * from weatherparamsetup where code = '%1' order by paramid").arg(currentApCode);
    QSqlQueryModel *plainModel = pgDb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        WeatherParamSetup weatherParamSetup;
        weatherParamSetup.setCode(plainModel->record(i).value(0).toString());
        weatherParamSetup.setParamid(plainModel->record(i).value(1).toInt());
        weatherParamSetup.setLimits(plainModel->record(i).value(2).toString());
        bool isExist = false;
        for(WeatherParam weatherParam : weatherParamList){
            if(weatherParam.id() == weatherParamSetup.paramid()){
                isExist = true;
                break;
            }
        }
        if(isExist){
            weatherParamSetupList.append(weatherParamSetup);
        }
    }
    delete plainModel;
    for(WeatherParamSetup weatherParamSetup : weatherParamSetupList){
        int key = weatherParamSetup.paramid();
        QString limitJson = weatherParamSetup.limits();
        QList<QString> valueList = this->getDataFromJson(limitJson);
        QList<QLineEdit *> editList = editHash[key];
        int valueCount = valueList.count();
        int editCount = editList.count();
        for(int i = 0;i < editCount;i++){
            QLineEdit *edit = editList[i];
            if(valueCount != editCount){
                edit->setText("");
            }else{
                edit->setText(valueList[i]);
            }
        }
    }
}

void MultiWeatherParamWidget::initData(){
    //初始化DB
    pgDb = new PgDataBase;
    //多要素
    QString queryStr = QString("select * from weatherparam where choose_type = 1 and limit_type <> 0 order by id");
    QSqlQueryModel *plainModel = pgDb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        WeatherParam weatherParam;
        weatherParam.setId(plainModel->record(i).value(0).toInt());
        weatherParam.setName(plainModel->record(i).value(1).toString());
        weatherParam.setChoose_type(plainModel->record(i).value(2).toInt());
        weatherParam.setLimit_type(plainModel->record(i).value(3).toInt());
        weatherParamList.append(weatherParam);
    }
    delete plainModel;
}

void MultiWeatherParamWidget::initUI(){
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout;
    QLabel *infoLabel = new QLabel;
    infoLabel->setText("<font color='#E10000'>大于等于左侧阀值,小于右侧阀值.<br>如果左侧阀值为空,则大于等于无穷小.<br>如果右侧阀值为空,则小于无穷大.</font>");
    scrollLayout->addWidget(infoLabel);

    editHash.clear();
    QRegExp regExp("^-?(\\d+(\\.\\d{1,2})?)$");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);
    for(WeatherParam weatherParam : weatherParamList){
        QList<QLineEdit *> editList;
        int key = weatherParam.id();

        QString title = weatherParam.name();
        int limitType = weatherParam.limit_type();
        QGridLayout *gridLayout = new QGridLayout;
        //不可飞
        QLabel *bkfLabel = new QLabel;
        bkfLabel->setText("不可飞");
        QLineEdit *bkfLeftEdit = new QLineEdit;
        bkfLeftEdit->setValidator(validator);
        editList.append(bkfLeftEdit);
        QLineEdit *bkfRightEdit = new QLineEdit;
        bkfRightEdit->setValidator(validator);
        editList.append(bkfRightEdit);
        gridLayout->addWidget(bkfLabel, 0, 0);
        gridLayout->addWidget(bkfLeftEdit, 0, 1);
        gridLayout->addWidget(bkfRightEdit, 0, 2);
        //限制可飞
        QLabel *xzkfLabel = new QLabel;
        xzkfLabel->setText("限制可飞");
        QLineEdit *xzkfLeftEdit = new QLineEdit;
        xzkfLeftEdit->setValidator(validator);
        editList.append(xzkfLeftEdit);
        QLineEdit *xzkfRightEdit = new QLineEdit;
        xzkfRightEdit->setValidator(validator);
        editList.append(xzkfRightEdit);
        gridLayout->addWidget(xzkfLabel, 1, 0);
        gridLayout->addWidget(xzkfLeftEdit, 1, 1);
        gridLayout->addWidget(xzkfRightEdit, 1, 2);
        //完全可飞
        QLabel *wqkfLabel = new QLabel;
        wqkfLabel->setText("完全可飞");
        QLineEdit *wqkfLeftEdit = new QLineEdit;
        wqkfLeftEdit->setValidator(validator);
        editList.append(wqkfLeftEdit);
        QLineEdit *wqkfRightEdit = new QLineEdit;
        wqkfRightEdit->setValidator(validator);
        editList.append(wqkfRightEdit);
        gridLayout->addWidget(wqkfLabel, 2, 0);
        gridLayout->addWidget(wqkfLeftEdit, 2, 1);
        gridLayout->addWidget(wqkfRightEdit, 2, 2);

        if (limitType == 2){
            //限制可飞
            QLabel *xzkfLabel = new QLabel;
            xzkfLabel->setText("限制可飞");
            QLineEdit *xzkfLeftEdit = new QLineEdit;
            xzkfLeftEdit->setValidator(validator);
            editList.append(xzkfLeftEdit);
            QLineEdit *xzkfRightEdit = new QLineEdit;
            xzkfRightEdit->setValidator(validator);
            editList.append(xzkfRightEdit);
            gridLayout->addWidget(xzkfLabel, 3, 0);
            gridLayout->addWidget(xzkfLeftEdit, 3, 1);
            gridLayout->addWidget(xzkfRightEdit, 3, 2);
            //不可飞
            QLabel *bkfLabel = new QLabel;
            bkfLabel->setText("不可飞");
            QLineEdit *bkfLeftEdit = new QLineEdit;
            bkfLeftEdit->setValidator(validator);
            editList.append(bkfLeftEdit);
            QLineEdit *bkfRightEdit = new QLineEdit;
            bkfRightEdit->setValidator(validator);
            editList.append(bkfRightEdit);
            gridLayout->addWidget(bkfLabel, 4, 0);
            gridLayout->addWidget(bkfLeftEdit, 4, 1);
            gridLayout->addWidget(bkfRightEdit, 4, 2);
        }
        //group
        QGroupBox *groupBox = new QGroupBox;
        groupBox->setTitle(title);
        groupBox->setAlignment(Qt::AlignLeft);
        groupBox->setLayout(gridLayout);
        //添加group
        scrollLayout->addWidget(groupBox);
        //添加hash
        editHash[key] = editList;
    }

    scrollWidget->setLayout(scrollLayout);
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollWidget);

    confirmButton = new QPushButton;
    confirmButton->setText("确定");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(confirmButton);
    this->setLayout(mainLayout);
}

void MultiWeatherParamWidget::initConnect(){
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
}

QList<QString> MultiWeatherParamWidget::getDataFromJson(QString jsonStr){
    QList<QString> valueList;
    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8(), &jsonErr);
    if(jsonErr.error == QJsonParseError::NoError){
        if(!jsonDoc.isEmpty()){
            if(jsonDoc.isObject()){
                QJsonObject jobj = jsonDoc.object();
                QJsonObject::iterator it = jobj.begin();
                while(it != jobj.end()){
                    if(QJsonValue::Array == it.value().type()){
                        QJsonArray array = it.value().toArray();
                        int subArrayCount = array.count();
                        for(int i = 0;i < subArrayCount;i++){
                            QJsonArray subArray = array.at(i).toArray();
                            valueList.append(subArray.at(0).toString());
                            valueList.append(subArray.at(1).toString());
                        }
                    }
                    it++;
                }
            }
        }
    }
    return valueList;
}

void MultiWeatherParamWidget::onConfirmClicked(){
    QList<int> keyList = editHash.keys();
    for(int id : keyList){
        QList<QLineEdit *> editList = editHash[id];
        int editCount = editList.count();
        QStringList jsonList;
        for(int i = 0;i < editCount;i += 2){
            QString jsonStr = QString("[\"%1\",\"%2\"]")
                    .arg(editList[i]->text().trimmed())
                    .arg(editList[i + 1]->text().trimmed());
            jsonList.append(jsonStr);
        }
        QString limits = QString("{\"limit\":[%1]}").arg(jsonList.join(","));

        bool isExist = false;
        for(WeatherParamSetup weatherParamSetup : weatherParamSetupList){
            if(id == weatherParamSetup.paramid()){
                isExist = true;
                break;
            }
        }
        if(isExist){
            QString updateSql = QString("update weatherparamsetup set limits = ? where code = ? and paramid = ?");
            QList<QVariant> valList;
            valList.append(limits);
            valList.append(currentApCode);
            valList.append(id);

            pgDb->save(updateSql, valList);
        }else{
            QString insertSql = QString("insert into weatherparamsetup values(?, ?, ?)");
            QList<QVariant> valList;
            valList.append(currentApCode);
            valList.append(id);
            valList.append(limits);

            pgDb->save(insertSql, valList);
        }
    }
    QMessageBox::information(0, QObject::tr("消息提示"), "多要素阀值处理完成");
}

/*** 单要素阀值设置widget ***/
SingleWeatherParamWidget::SingleWeatherParamWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

SingleWeatherParamWidget::~SingleWeatherParamWidget(){
    delete pgDb;
    delete confirmButton;
}

void SingleWeatherParamWidget::onAirportChanged(QString apCode){
    //清空
    QList<int> keyList = editHash.keys();
    for(int key : keyList){
        QList<QLineEdit *> editList = editHash[key];
        for(QLineEdit *edit : editList){
            edit->setText("");
        }
    }

    //赋值
    currentApCode = apCode;
    weatherParamSetupList.clear();
    QString queryStr = QString("select * from weatherparamsetup where code = '%1' order by paramid").arg(currentApCode);
    QSqlQueryModel *plainModel = pgDb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        WeatherParamSetup weatherParamSetup;
        weatherParamSetup.setCode(plainModel->record(i).value(0).toString());
        weatherParamSetup.setParamid(plainModel->record(i).value(1).toInt());
        weatherParamSetup.setLimits(plainModel->record(i).value(2).toString());
        bool isExist = false;
        for(WeatherParam weatherParam : weatherParamList){
            if(weatherParam.id() == weatherParamSetup.paramid()){
                isExist = true;
                break;
            }
        }
        if(isExist){
            weatherParamSetupList.append(weatherParamSetup);
        }
    }
    delete plainModel;
    for(WeatherParamSetup weatherParamSetup : weatherParamSetupList){
        int key = weatherParamSetup.paramid();
        QString limitJson = weatherParamSetup.limits();
        QList<QString> valueList = this->getDataFromJson(limitJson);
        QList<QLineEdit *> editList = editHash[key];
        int valueCount = valueList.count();
        int editCount = editList.count();
        for(int i = 0;i < editCount;i++){
            QLineEdit *edit = editList[i];
            if(valueCount != editCount){
                edit->setText("");
            }else{
                edit->setText(valueList[i]);
            }
        }
    }
}

void SingleWeatherParamWidget::initData(){
    //初始化DB
    pgDb = new PgDataBase;
    //多要素
    QString queryStr = QString("select * from weatherparam where choose_type = 0 and limit_type <> 0 order by id");
    QSqlQueryModel *plainModel = pgDb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        WeatherParam weatherParam;
        weatherParam.setId(plainModel->record(i).value(0).toInt());
        weatherParam.setName(plainModel->record(i).value(1).toString());
        weatherParam.setChoose_type(plainModel->record(i).value(2).toInt());
        weatherParam.setLimit_type(plainModel->record(i).value(3).toInt());
        weatherParamList.append(weatherParam);
    }
    delete plainModel;
}

void SingleWeatherParamWidget::initUI(){
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout;
    QLabel *infoLabel = new QLabel;
    infoLabel->setText("<font color='#E10000'>大于等于左侧阀值,小于右侧阀值.<br>如果左侧阀值为空,则大于等于无穷小.<br>如果右侧阀值为空,则小于无穷大.</font>");
    scrollLayout->addWidget(infoLabel);

    editHash.clear();
    QRegExp regExp("^-?(\\d+(\\.\\d{1,2})?)$");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);
    for(WeatherParam weatherParam : weatherParamList){
        QList<QLineEdit *> editList;
        int key = weatherParam.id();

        QString title = weatherParam.name();
        int limitType = weatherParam.limit_type();
        QGridLayout *gridLayout = new QGridLayout;
        //不可飞
        QLabel *bkfLabel = new QLabel;
        bkfLabel->setText("不可飞");
        QLineEdit *bkfLeftEdit = new QLineEdit;
        bkfLeftEdit->setValidator(validator);
        editList.append(bkfLeftEdit);
        QLineEdit *bkfRightEdit = new QLineEdit;
        bkfRightEdit->setValidator(validator);
        editList.append(bkfRightEdit);
        gridLayout->addWidget(bkfLabel, 0, 0);
        gridLayout->addWidget(bkfLeftEdit, 0, 1);
        gridLayout->addWidget(bkfRightEdit, 0, 2);
        //限制可飞
        QLabel *xzkfLabel = new QLabel;
        xzkfLabel->setText("限制可飞");
        QLineEdit *xzkfLeftEdit = new QLineEdit;
        xzkfLeftEdit->setValidator(validator);
        editList.append(xzkfLeftEdit);
        QLineEdit *xzkfRightEdit = new QLineEdit;
        xzkfRightEdit->setValidator(validator);
        editList.append(xzkfRightEdit);
        gridLayout->addWidget(xzkfLabel, 1, 0);
        gridLayout->addWidget(xzkfLeftEdit, 1, 1);
        gridLayout->addWidget(xzkfRightEdit, 1, 2);
        //完全可飞
        QLabel *wqkfLabel = new QLabel;
        wqkfLabel->setText("完全可飞");
        QLineEdit *wqkfLeftEdit = new QLineEdit;
        wqkfLeftEdit->setValidator(validator);
        editList.append(wqkfLeftEdit);
        QLineEdit *wqkfRightEdit = new QLineEdit;
        wqkfRightEdit->setValidator(validator);
        editList.append(wqkfRightEdit);
        gridLayout->addWidget(wqkfLabel, 2, 0);
        gridLayout->addWidget(wqkfLeftEdit, 2, 1);
        gridLayout->addWidget(wqkfRightEdit, 2, 2);

        if (limitType == 2){
            //限制可飞
            QLabel *xzkfLabel = new QLabel;
            xzkfLabel->setText("限制可飞");
            QLineEdit *xzkfLeftEdit = new QLineEdit;
            xzkfLeftEdit->setValidator(validator);
            editList.append(xzkfLeftEdit);
            QLineEdit *xzkfRightEdit = new QLineEdit;
            xzkfRightEdit->setValidator(validator);
            editList.append(xzkfRightEdit);
            gridLayout->addWidget(xzkfLabel, 3, 0);
            gridLayout->addWidget(xzkfLeftEdit, 3, 1);
            gridLayout->addWidget(xzkfRightEdit, 3, 2);
            //不可飞
            QLabel *bkfLabel = new QLabel;
            bkfLabel->setText("不可飞");
            QLineEdit *bkfLeftEdit = new QLineEdit;
            bkfLeftEdit->setValidator(validator);
            editList.append(bkfLeftEdit);
            QLineEdit *bkfRightEdit = new QLineEdit;
            bkfRightEdit->setValidator(validator);
            editList.append(bkfRightEdit);
            gridLayout->addWidget(bkfLabel, 4, 0);
            gridLayout->addWidget(bkfLeftEdit, 4, 1);
            gridLayout->addWidget(bkfRightEdit, 4, 2);
        }
        //group
        QGroupBox *groupBox = new QGroupBox;
        groupBox->setTitle(title);
        groupBox->setAlignment(Qt::AlignLeft);
        groupBox->setLayout(gridLayout);
        //添加group
        scrollLayout->addWidget(groupBox);
        //添加hash
        editHash[key] = editList;
    }

    scrollWidget->setLayout(scrollLayout);
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollWidget);

    confirmButton = new QPushButton;
    confirmButton->setText("确定");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(confirmButton);
    this->setLayout(mainLayout);
}

void SingleWeatherParamWidget::initConnect(){
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
}

QList<QString> SingleWeatherParamWidget::getDataFromJson(QString jsonStr){
    QList<QString> valueList;
    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8(), &jsonErr);
    if(jsonErr.error == QJsonParseError::NoError){
        if(!jsonDoc.isEmpty()){
            if(jsonDoc.isObject()){
                QJsonObject jobj = jsonDoc.object();
                QJsonObject::iterator it = jobj.begin();
                while(it != jobj.end()){
                    if(QJsonValue::Array == it.value().type()){
                        QJsonArray array = it.value().toArray();
                        int subArrayCount = array.count();
                        for(int i = 0;i < subArrayCount;i++){
                            QJsonArray subArray = array.at(i).toArray();
                            valueList.append(subArray.at(0).toString());
                            valueList.append(subArray.at(1).toString());
                        }
                    }
                    it++;
                }
            }
        }
    }
    return valueList;
}


void SingleWeatherParamWidget::onConfirmClicked(){
    QList<int> keyList = editHash.keys();
    for(int id : keyList){
        QList<QLineEdit *> editList = editHash[id];
        int editCount = editList.count();
        QStringList jsonList;
        for(int i = 0;i < editCount;i += 2){
            QString jsonStr = QString("[\"%1\",\"%2\"]")
                    .arg(editList[i]->text().trimmed())
                    .arg(editList[i + 1]->text().trimmed());
            jsonList.append(jsonStr);
        }
        QString limits = QString("{\"limit\":[%1]}").arg(jsonList.join(","));

        bool isExist = false;
        for(WeatherParamSetup weatherParamSetup : weatherParamSetupList){
            if(id == weatherParamSetup.paramid()){
                isExist = true;
                break;
            }
        }
        if(isExist){
            QString updateSql = QString("update weatherparamsetup set limits = ? where code = ? and paramid = ?");
            QList<QVariant> valList;
            valList.append(limits);
            valList.append(currentApCode);
            valList.append(id);

            pgDb->save(updateSql, valList);
        }else{
            QString insertSql = QString("insert into weatherparamsetup values(?, ?, ?)");
            QList<QVariant> valList;
            valList.append(currentApCode);
            valList.append(id);
            valList.append(limits);

            pgDb->save(insertSql, valList);
        }
    }
    QMessageBox::information(0, QObject::tr("消息提示"), "单要素阀值处理完成");
}

/*** 气象参数设置widget ***/
WeatherParamSetupWidget::WeatherParamSetupWidget(QWidget *parent)
    :QDialog(parent)
    ,multiWeatherParamWidget(NULL)
    ,singleWeatherParamWidget(NULL)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

WeatherParamSetupWidget::~WeatherParamSetupWidget(){
    delete airportComboBox;
    if(multiWeatherParamWidget != NULL){
        delete multiWeatherParamWidget;
    }
    if(singleWeatherParamWidget != NULL){
        delete singleWeatherParamWidget;
    }
    delete tabWidget;
    delete pgDb;
}

void WeatherParamSetupWidget::initData(){
    //初始化DB
    pgDb = new PgDataBase;
    //机场
    apCodeList.clear();
    apNameList.clear();
    airportList = SharedMemory::getInstance()->getAirportList();
    int airportCount = airportList.size();
    for(int i = 0;i < airportCount;i++){
        Airport airport = airportList[i];
        apCodeList.append(airport.code());
        apNameList.append(airport.name());
    }
}

void WeatherParamSetupWidget::initUI(){
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedWidth(440);
    this->setFixedHeight(500);

    this->setWindowIcon(QIcon(":/images/weather_setup.png"));
    this->setWindowTitle("阀值设置");

    //设置机场
    airportComboBox = new QComboBox;
    airportComboBox->addItems(apNameList);
    //设置标签
    tabWidget = new QTabWidget;
    tabWidget->setContentsMargins(5, 5, 5, 5);
    if(apNameList.size() > 0){
        multiWeatherParamWidget = new MultiWeatherParamWidget;
        multiWeatherParamWidget->onAirportChanged(apCodeList[airportComboBox->currentIndex()]);
        tabWidget->addTab(multiWeatherParamWidget, "多要素");
        singleWeatherParamWidget = new SingleWeatherParamWidget;
        singleWeatherParamWidget->onAirportChanged(apCodeList[airportComboBox->currentIndex()]);
        tabWidget->addTab(singleWeatherParamWidget, "单要素");
    }
    //布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(airportComboBox);
    mainLayout->addWidget(tabWidget);
    this->setLayout(mainLayout);
}

void WeatherParamSetupWidget::initConnect(){
    connect(airportComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onAirportChanged(int)));
}

void WeatherParamSetupWidget::onAirportChanged(int index){
    multiWeatherParamWidget->onAirportChanged(apCodeList[index]);
    singleWeatherParamWidget->onAirportChanged(apCodeList[index]);
}
