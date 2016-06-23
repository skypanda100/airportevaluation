#include "kfttjinputwidget.h"

KfttjInputWidget::KfttjInputWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

KfttjInputWidget::~KfttjInputWidget(){
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

    slfRadioButton = new QRadioButton;
    slfRadioButton->setText("矢量风");
    slfRadioButton->setChecked(true);

    sfCheckBox = new QCheckBox;
    sfCheckBox->setText("顺风");
    nfCheckBox = new QCheckBox;
    nfCheckBox->setText("逆风");
    cfCheckBox = new QCheckBox;
    cfCheckBox->setText("侧风");

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

    njyCheckBox = new QCheckBox;
    njyCheckBox->setText("浓积云");
    jyyCheckBox = new QCheckBox;
    jyyCheckBox->setText("积雨云");
    sdCheckBox = new QCheckBox;
    sdCheckBox->setText("闪电");
    lbCheckBox = new QCheckBox;
    lbCheckBox->setText("雷暴");
    bbCheckBox = new QCheckBox;
    bbCheckBox->setText("冰雹");
    bxCheckBox = new QCheckBox;
    bxCheckBox->setText("飑线");
    ljCheckBox = new QCheckBox;
    ljCheckBox->setText("龙卷");
    scbCheckBox = new QCheckBox;
    scbCheckBox->setText("沙尘暴");
    fqbCheckBox = new QCheckBox;
    fqbCheckBox->setText("风切变");

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

void KfttjInputWidget::execute(){
    emit executeKfttj();
}


