#include "fmgresultwidget.h"
FmgResultWidget::FmgResultWidget(QWidget *parent)
    :QSplitter(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

FmgResultWidget::~FmgResultWidget(){

}

void FmgResultWidget::initData(){

}

void FmgResultWidget::initUI(){
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    this->setOrientation(Qt::Vertical);
}

void FmgResultWidget::initConnect(){

}
