#ifndef SJDRINPUTWIDGET
#define SJDRINPUTWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"
#include "bean/qualitycontrolsource.h"

class SjdrInputWidget : public QWidget{
    Q_OBJECT
public:
    SjdrInputWidget(QWidget *parent = 0);
    ~SjdrInputWidget();

signals:
    void executeSjdr(Airport, QList<QualityControlSource>, QList<QString>);
    void stopExecuteSjdr();

private:
    void initData();
    void initUI();
    void initConnect();
    //输入check
    bool validate();
    //查询机场表
    void queryAirport();
    //查询数据源
    void querySource();
    //递归查找文件
    int findFiles(QString);
    //重置机场列表
    void resetAirportComboBox(QList<Airport> airportList, bool isSave = false);

private slots:
    void browse();
    void execute();
    void onAirportInfoChanged(QList<Airport>, QHash< QString, QList<QString> >);

private:
    PgDataBase *pgdb;
    QList<Airport> airportList;
    QList<QualityControlSource> qualityControlSourceList;
    QList<QString> sourceFileList;

    QComboBox *airportComboBox;
    QList<QCheckBox *> sourceChkList;
    QLineEdit *pathEdit;
    QPushButton *browseButton;
    QPushButton *executeButton;
    QPushButton *stopButton;
};

#endif // SJDRINPUTWIDGET

