#include "welcomewidget.h"

WelcomeWidget::WelcomeWidget(QWidget *parent)
    :QWidget(parent)
    ,welcomePixmap(NULL)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

WelcomeWidget::~WelcomeWidget(){
    if(welcomePixmap != NULL){
        delete welcomePixmap;
    }
}

void WelcomeWidget::initData(){
//    welcomePixmap = new QPixmap(":/images/cloud.jpg");
    welcomePixmap = new QPixmap(":/images/C919.jpg");
}

void WelcomeWidget::initUI(){

}

void WelcomeWidget::initConnect(){

}

void WelcomeWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);

    QRectF target = this->rect();
    QRectF source = welcomePixmap->rect();

    painter.drawPixmap(target, *welcomePixmap, source);
}
