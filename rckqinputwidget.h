#ifndef RCKQINPUTWIDGET
#define RCKQINPUTWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"

class RckqInputWidget : public QWidget{
    Q_OBJECT
public:
    RckqInputWidget(QWidget *parent = 0);
    ~RckqInputWidget();

signals:
    void executeRckq(QString code, QString runway, int type, int fhour, int thour, QList<QString> dateList, QList<QString> weatherList);
    void executeExport();

private slots:
    void onAirportChanged(const QString &);
    void onTypeChanged(int index);
    void onAddDateClicked();
    void execute();
    void onAirportInfoChanged(QList<Airport>, QHash< QString, QList<QString> >);

private:
    void initData();
    void initUI();
    void initConnect();
    void queryAirport();
    //输入check
    bool validate();
    //重置机场列表
    void resetAirportComboBox(QList<Airport> apList, QHash< QString, QList<QString> > rwHash, bool isSave = false);

private:
    PgDataBase *pgdb;
    QList<Airport> airportList;
    QHash< QString, QList<QString> > runwayHash;

    QComboBox *airportComboBox;
    QComboBox *runwayComboBox;
    QComboBox *typeComboBox;
    QComboBox *fhourComboBox;
    QComboBox *thourComboBox;
    QList<QLineEdit *> dateEditList;
    QPushButton *addDateButton;
    QGridLayout *dateGLayout;
    QList<QString> weatherList;
    QList<QCheckBox *> weatherCheckBoxList;
    QList<QRadioButton *> weatherRadioButtonList;
    QGridLayout *weatherGLayout;
    QPushButton *executeButton;
    QPushButton *exportButton;

};

#endif // RCKQINPUTWIDGET

