#ifndef FMGINPUTWIDGET
#define FMGINPUTWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"

class FmgInputWidget : public QWidget{
    Q_OBJECT
public:
    FmgInputWidget(QWidget *parent = 0);
    ~FmgInputWidget();

signals:
    void executeFmg(QString code, QString runway, QString fspeed, QString tspeed, QList<QString> years);
    void executeExport();

private slots:
    void onAirportChanged(const QString &);
    void onRunwayChanged(const QString &);
    void execute();
    void onAirportInfoChanged(QList<Airport>, QHash< QString, QList<QString> >);

private:
    void initData();
    void initUI();
    void initConnect();
    void queryAirport();
    void queryDate();
    void resetDateArea();
    //输入check
    bool validate();
    //重置机场列表
    void resetAirportComboBox(QList<Airport> apList, QHash< QString, QList<QString> > rwHash, bool isSave = false);

private:
    PgDataBase *pgdb;
    QList<Airport> airportList;
    QHash< QString, QList<QString> > runwayHash;
    QList<QString> dateList;

    QComboBox *airportComboBox;
    QComboBox *runwayComboBox;
    QLineEdit *fspeedEdit;
    QLineEdit *tspeedEdit;
    QGridLayout *dateLayout;
    QList<QCheckBox *> dateCheckBoxList;
    QPushButton *executeButton;
    QPushButton *exportButton;
};

#endif // FMGINPUTWIDGET

