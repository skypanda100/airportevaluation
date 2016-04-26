#ifndef SJDRMAINWIDGET
#define SJDRMAINWIDGET

#include <QtWidgets>
#include "bean/airport.h"
#include "bean/qualitycontrolsource.h"

/*** 左边 ***/
class SjdrMainLeftWidget : public QTreeWidget{
    Q_OBJECT

public:
    SjdrMainLeftWidget(QWidget *parent = 0);
    ~SjdrMainLeftWidget();

    void addItem(const QString &, const QString &);
    void deleteItems();

private:
    void initUI();
    void initConnect();
};

/*** 右边 ***/
class SjdrMainRightWidget : public QWidget{
    Q_OBJECT

public:
    SjdrMainRightWidget(QWidget *parent = 0);
    ~SjdrMainRightWidget();

private:
    void initUI();
    void initConnect();
};

/*** 主 ***/
class SjdrMainWidget : public QSplitter{
    Q_OBJECT

public:
    SjdrMainWidget(QWidget *parent = 0);
    ~SjdrMainWidget();

public slots:
    void executeSjdr(Airport, QList<QualityControlSource>, QList<QString>);

private:
    void initUI();
    void initConnect();
    //判断数据源类型
    void assortSource(QList<QualityControlSource>, QList<QString>);

private:
    Airport currentAirport;
    QHash< int, QList<QString> > srcHash;
    SjdrMainLeftWidget *sjdrMainLeftWidget;
    SjdrMainRightWidget *sjdrMainRightWidget;

};

#endif // SJDRMAINWIDGET

