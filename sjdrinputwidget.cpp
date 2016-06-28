#include "sjdrinputwidget.h"
#include "common/sharedmemory.h"

SjdrInputWidget::SjdrInputWidget(QWidget *parent)
    :QWidget(parent)
    ,pgdb(NULL)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

SjdrInputWidget::~SjdrInputWidget(){
    delete pgdb;
    delete airportComboBox;
    int sourceChkCount = sourceChkList.size();
    for(int i = 0;i < sourceChkCount;i++){
        QCheckBox *checkBox = sourceChkList[i];
        delete checkBox;
    }
    delete pathEdit;
    delete browseButton;
    delete executeButton;
    delete stopButton;
}

void SjdrInputWidget::initData(){
    pgdb = new PgDataBase;
}

void SjdrInputWidget::initUI(){
    //机场
    airportComboBox = new QComboBox;
    queryAirport();

    QVBoxLayout *airportLayout = new QVBoxLayout;
    airportLayout->addWidget(airportComboBox);

    QGroupBox *airportGroup = new QGroupBox;
    airportGroup->setTitle("机场");
    airportGroup->setLayout(airportLayout);

    //数据源
    querySource();

    QGridLayout *sourceLayout = new QGridLayout;
    int sourceCount = qualityControlSourceList.size();
    for(int i = 0;i < sourceCount;i++){
        QCheckBox *checkBox = new QCheckBox(qualityControlSourceList[i].name());
        checkBox->setChecked(true);
        sourceLayout->addWidget(checkBox, i / 2 , i % 2);
        sourceChkList.append(checkBox);
    }

    QGroupBox *sourceGroup = new QGroupBox;
    sourceGroup->setTitle("数据源");
    sourceGroup->setLayout(sourceLayout);

    //数据源路径
    pathEdit = new QLineEdit;
    browseButton = new QPushButton;
    browseButton->setIcon(QIcon(":/images/search.png"));
    browseButton->setToolTip("搜索");
    browseButton->setFixedSize(25, 25);

    QHBoxLayout *pathLayout = new QHBoxLayout;
    pathLayout->addWidget(pathEdit, 1);
    pathLayout->addWidget(browseButton);

    QGroupBox *pathGroup = new QGroupBox;
    pathGroup->setTitle("路径");
    pathGroup->setLayout(pathLayout);

    //执行按钮
    executeButton = new QPushButton;
    executeButton->setText("开始");
    stopButton = new QPushButton;
    stopButton->setText("停止");

    QHBoxLayout *executeLayout = new QHBoxLayout;
    executeLayout->addWidget(executeButton);
    executeLayout->addWidget(stopButton);

    //主界面
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(airportGroup);
    mainLayout->addWidget(sourceGroup);
    mainLayout->addWidget(pathGroup);
    mainLayout->addLayout(executeLayout);
    mainLayout->addStretch(1);
    this->setLayout(mainLayout);
}

void SjdrInputWidget::initConnect(){
    connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
    connect(executeButton, SIGNAL(clicked()), this, SLOT(execute()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(execute()));
    connect(SharedMemory::getInstance()
            , SIGNAL(airportInfoChanged(QList<Airport>,QHash<QString,QList<QString> >))
            , this
            , SLOT(onAirportInfoChanged(QList<Airport>,QHash<QString,QList<QString> >)));
}

/**
 * @brief SjdrInputWidget::validate
 * 输入check
 * @return
 */
bool SjdrInputWidget::validate(){
    //机场
    if(airportComboBox->count() == 0){
        QMessageBox::critical(0, QObject::tr("错误提示"), "必须选择一个机场才能进行数据导入!\n请在机场设置里添加一个机场.");
        return false;
    }
    //数据源
    bool isSourceChked = false;
    int sourceChkCount = sourceChkList.size();
    for(int i = 0;i < sourceChkCount;i++){
        QCheckBox *checkBox = sourceChkList[i];
        if(checkBox->isChecked()){
            isSourceChked = true;
            break;
        }
    }
    if(!isSourceChked){
        QMessageBox::critical(0, QObject::tr("错误提示"), "必须选择一个数据源才能进行数据导入!");
        return false;
    }
    //路径
    QString path = pathEdit->text().trimmed();
    if(path.isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "必须输入数据源路径才能进行数据导入!");
        return false;
    }else{
        QFileInfo srcFileInfo = QFileInfo(path);
        if(!srcFileInfo.isDir() && !srcFileInfo.isFile()){
            QMessageBox::critical(0, QObject::tr("错误提示"), "请输入正确的数据源路径!");
            return false;
        }
    }
    return true;
}

/**
 * @brief SjdrInputWidget::queryAirport
 * 查找机场
 */
void SjdrInputWidget::queryAirport(){
    resetAirportComboBox(SharedMemory::getInstance()->getAirportList(), false);
}

/**
 * @brief SjdrInputWidget::querySource
 * 查找数据源类型
 */
void SjdrInputWidget::querySource(){
    qualityControlSourceList.clear();
    QString queryStr = QString("select * from qualitycontrolsource");
    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        QualityControlSource qualityControlSource;
        qualityControlSource.setId(plainModel->record(i).value(0).toInt());
        qualityControlSource.setName(plainModel->record(i).value(1).toString());
        qualityControlSource.setFields(plainModel->record(i).value(2).toString());
        qualityControlSourceList.append(qualityControlSource);
    }
    delete plainModel;
}

/**
 * @brief SjdrInputWidget::findFiles
 * 递归查找文件
 * @return
 */
int SjdrInputWidget::findFiles(QString path){
    //判断指定路径是否存在
    QDir dir(path);
    if (!dir.exists()) {
        return -1;
    }

    //取到所有的文件和文件名，但是去掉.和..的文件夹（这是QT默认有的）
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    //文件夹优先
    dir.setSorting(QDir::DirsFirst);

    //转化成一个list
    QFileInfoList list = dir.entryInfoList();
    if(list.size() < 1 ) {
        return -1;
    }
    int i = 0;
    //递归
    do{
        QFileInfo fileInfo = list.at(i);

        //如果是文件夹，递归
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            findFiles(fileInfo.filePath());
        }else{
            bool isSourceFile = fileInfo.fileName().endsWith(".mdb", Qt::CaseInsensitive) | fileInfo.fileName().endsWith(".his", Qt::CaseInsensitive);
            if (isSourceFile == true) {
                sourceFileList.append(fileInfo.filePath());
            }
        }
        i++;
    } while(i < list.size());

    return i;
}

/**
 * @brief SjdrInputWidget::browse
 * 搜索文件
 */
void SjdrInputWidget::browse(){
    QString currentDir = pathEdit->text().isEmpty() ? QDir::currentPath() : pathEdit->text();
    QString directory = QFileDialog::getExistingDirectory(this,
                               tr("搜索文件"), currentDir);

    if (!directory.isEmpty()) {
           pathEdit->setText(directory);
    }
}

/**
 * @brief SjdrInputWidget::execute
 * 执行数据导入
 */
void SjdrInputWidget::execute(){
    bool isStart = true;
    if(this->sender() == stopButton){
        isStart = false;
    }
    if(isStart){
        if(!validate()){
           return;
        }
        //清除
        sourceFileList.clear();
        int ret = findFiles(pathEdit->text());
        if(!(ret > 0)){
            QMessageBox::critical(0, QObject::tr("错误提示"), "指定目录下没有符合要求的文件");
            return;
        }
        int sourceChkCount = sourceChkList.size();
        QList<QualityControlSource> qualityControlSourceChkedList;
        for(int i = 0;i < sourceChkCount;i++){
            QCheckBox *checkBox = sourceChkList[i];
            if(checkBox->isChecked()){
                qualityControlSourceChkedList.append(qualityControlSourceList[i]);
            }
        }
        emit executeSjdr(airportList[airportComboBox->currentIndex()], qualityControlSourceChkedList, sourceFileList);
    }else{
        emit stopExecuteSjdr();
    }
}

/**
 * @brief SjdrInputWidget::onAirportInfoChanged
 * @param airportList
 */
void SjdrInputWidget::onAirportInfoChanged(QList<Airport> airportList, QHash< QString, QList<QString> >){
    resetAirportComboBox(airportList, true);
}

void SjdrInputWidget::resetAirportComboBox(QList<Airport> apList, bool isSave){
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
    }
}
