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

private slots:
    void onAirportChanged(const QString &);
    void onTypeChanged(int index);
    void onAddDateClicked();
    void execute();

private:
    void initData();
    void initUI();
    void initConnect();
    void queryAirport();
    QList<QString> queryRunway(QString);
    //输入check
    bool validate();

private:
    PgDataBase *pgdb;
    QList<Airport> aiportList;
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

};

#endif // RCKQINPUTWIDGET

