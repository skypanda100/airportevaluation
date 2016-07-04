#include "exportdialog.h"

ExportDialog::ExportDialog(int chartCount, QWidget *parent)
    :QDialog(parent)
{
    m_chartCount = chartCount <= 0 ? 1 : chartCount;
    this->initUI();
    this->initConnect();
}

ExportDialog::~ExportDialog(){
    delete dataDefineEdit;
    delete dataDefineButton;
    int chartEditCount = chartEditList.size();
    for(int i = chartEditCount - 1;i >= 0;i--){
        delete chartEditList[i];
    }
    int chartButtonCount = chartButtonList.size();
    for(int i = chartButtonCount - 1;i >= 0;i--){
        delete chartButtonList[i];
    }
    delete executeButton;
}

void ExportDialog::initUI(){
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    //标题
    this->setWindowTitle("数据导出");
    //数据定义表格
    QLabel *dataDefineLabel = new QLabel;
    dataDefineLabel->setText("数据定义表格");
    dataDefineEdit = new QLineEdit;
    dataDefineEdit->setMinimumWidth(220);
    dataDefineButton = new QPushButton;
    dataDefineButton->setIcon(QIcon(":/images/search.png"));
    dataDefineButton->setToolTip("搜索");

    //统计图
    QLabel *chartLabel = new QLabel;
    chartLabel->setText("统计图");
    for(int i = 0;i < m_chartCount;i++){
        QLineEdit *chartEdit = new QLineEdit;
        chartEdit->setMinimumWidth(220);
        chartEditList.append(chartEdit);
        QPushButton *chartButton = new QPushButton;
        chartButton->setIcon(QIcon(":/images/search.png"));
        chartButton->setToolTip("搜索");
        chartButtonList.append(chartButton);
    }

    //执行按钮
    executeButton = new QPushButton;
    executeButton->setText("开始");

    //布局
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(dataDefineLabel, 0, 0);
    mainLayout->addWidget(dataDefineEdit, 0, 1);
    mainLayout->addWidget(dataDefineButton, 0, 2);
    mainLayout->addWidget(chartLabel, 1, 0);
    for(int i = 0;i < m_chartCount;i++){
        mainLayout->addWidget(chartEditList[i], 1 + i, 1);
        mainLayout->addWidget(chartButtonList[i], 1 + i, 2);
    }
    mainLayout->addWidget(executeButton, 1 + m_chartCount, 0, 1, 3);
    this->setLayout(mainLayout);
}

void ExportDialog::initConnect(){
    connect(dataDefineButton, SIGNAL(clicked()), this, SLOT(browseClicked()));
    for(int i = 0;i < m_chartCount;i++){
        connect(chartButtonList[i], SIGNAL(clicked()), this, SLOT(browseClicked()));
    }
    connect(executeButton, SIGNAL(clicked()), this, SLOT(executeClicked()));
}

void ExportDialog::browseClicked(){
    QObject *object = this->sender();
    if(object == dataDefineButton){
        QString filePath = QFileDialog::getSaveFileName(this, tr("数据导出"), qApp->applicationDirPath(),
                                                            tr("Excel Files (*.xlsx)"));
        dataDefineEdit->setText(filePath);
    }else{
        QString filePath = QFileDialog::getSaveFileName(this, tr("数据导出"), qApp->applicationDirPath(),
                                                            tr("Images (*.png)"));
        int index = chartButtonList.indexOf((QPushButton *)object);
        chartEditList[index]->setText(filePath);
    }
}

void ExportDialog::executeClicked(){
    if(!validate()){
        return;
    }
    emit exportFiles(fileHash);
}

bool ExportDialog::validate(){
    fileHash.clear();
    QString dataDefinePath = dataDefineEdit->text().trimmed();
    if(dataDefinePath.isEmpty()){
        bool isAllEmpty = true;
        for(int i = 0;i < m_chartCount;i++){
            QLineEdit *chartEdit = chartEditList[i];
            QString chartPath = chartEdit->text().trimmed();
            if(!chartPath.isEmpty()){
                isAllEmpty = false;
                break;
            }
        }
        if(isAllEmpty){
            QMessageBox::critical(this, tr("错误提示"), tr("请输入至少一种产品的路径"));
            return false;
        }
    }
    if(!dataDefinePath.isEmpty()){
        QRegExp regExp(".*(\\.[xX][lL][sS][xX])$");
        int pos = regExp.indexIn(dataDefinePath);
        if(pos < 0){
            QMessageBox::critical(this, tr("错误提示"), tr("请输入正确的文件路径(文件路径 + 文件名)"));
            return false;
        }else{
            fileHash[0].append(dataDefinePath);
        }
    }
    for(int i = 0;i < m_chartCount;i++){
        QLineEdit *chartEdit = chartEditList[i];
        QString chartPath = chartEdit->text().trimmed();
        if(!chartPath.isEmpty()){
            QRegExp regExp(".*(\\.[pP][nN][gG])$");
            int pos = regExp.indexIn(chartPath);
            if(pos < 0){
                QMessageBox::critical(this, tr("错误提示"), tr("请输入正确的文件路径(文件路径 + 文件名)"));
                return false;
            }else{
                fileHash[1].append(chartPath);
            }
        }else{
            fileHash[1].append(chartPath);
        }
    }
    return true;
}
