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

private slots:
    void onAirportChanged(const QString &);
    void onRunwayChanged(const QString &);
    void execute();

private:
    void initData();
    void initUI();
    void initConnect();
    void queryAirport();
    QList<QString> queryRunway(QString);
    void queryDate();
    void resetDateArea();
    //输入check
    bool validate();

private:
    PgDataBase *pgdb;
    QList<Airport> aiportList;
    QHash< QString, QList<QString> > runwayHash;
    QList<QString> dateList;

    QComboBox *airportComboBox;
    QComboBox *runwayComboBox;
    QLineEdit *fspeedEdit;
    QLineEdit *tspeedEdit;
    QGridLayout *dateLayout;
    QList<QCheckBox *> dateCheckBoxList;
    QPushButton *executeButton;
};

#endif // FMGINPUTWIDGET

