#ifndef SJDRMAINWIDGET
#define SJDRMAINWIDGET

#include <QtWidgets>
#include "bean/airport.h"
#include "bean/qualitycontrolsource.h"
#include "control/sjdrcontrol.h"

class SjdrMainWidget : public QTreeView{
    Q_OBJECT

public:
    SjdrMainWidget(QWidget *parent = 0);
    ~SjdrMainWidget();

public slots:
    void executeSjdr(Airport, QList<QualityControlSource>, QList<QString>);
    void receiveMessage(QStringList);

private:
    void initData();
    void initUI();
    void initConnect();
    //判断数据源类型
    void assortSource(QList<QualityControlSource>, QList<QString>);
    //插入数据
    void insertRow(const QString &name, const QString &info, const QString &type, const QString &path);
    //删除数据
    void removeAllRows();
    //刷新
    void updateActions();
    //数据控制
    void dataControl(const QFileInfo &fileInfo, SourceType sourceType = UNKNOWN, QString type = "未知");

private:
    Airport currentAirport;
    QHash< int, QList<QString> > srcHash;
    QStringList headers;
    SjdrControl *sjdrControl;
};

#endif // SJDRMAINWIDGET

