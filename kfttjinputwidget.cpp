#include "kfttjinputwidget.h"

KfttjInputWidget::KfttjInputWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

KfttjInputWidget::~KfttjInputWidget(){
    delete executeButton;
    delete exportButton;
}

void KfttjInputWidget::initData(){
}

void KfttjInputWidget::initUI(){
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
    mainLayout->addLayout(executeLayout);
    mainLayout->addStretch(1);
    this->setLayout(mainLayout);

}

void KfttjInputWidget::initConnect(){
   connect(executeButton, SIGNAL(clicked()), this, SLOT(execute()));
   connect(exportButton, SIGNAL(clicked()), this, SIGNAL(executeExport()));
}

void KfttjInputWidget::execute(){
    emit executeKfttj();
}


