#ifndef AIRPORTSETUPWIDGET
#define AIRPORTSETUPWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"

/*** 机场添加widget ***/
class AirportAddWidget : public QWidget{
    Q_OBJECT
public:
    AirportAddWidget(QWidget *parent = 0);
    ~AirportAddWidget();

signals:
    void airportChanged();

private:
    void initData();
    void initUI();
    void initConnect();
    bool validate();
    bool createMonthSummary(QString);
    bool createExtremum(QString);
    bool createAutomaticWind(QString);
    bool createAutomaticTemperature(QString);

private slots:
    void onConfirmClicked();

private:
    PgDataBase *pgDb;

    QStringList typeList;

    QLineEdit *codeEdit;
    QLineEdit *nameEdit;
    QLineEdit *lonEdit;
    QLineEdit *latEdit;
    QLineEdit *altEdit;
    QLineEdit *dirEdit;
    QComboBox *typeComboBox;
    QPushButton *confirmButton;
};

/*** 机场修改widget ***/
class AirportModifyWidget : public QWidget{
    Q_OBJECT
public:
    AirportModifyWidget(QWidget *parent = 0);
    ~AirportModifyWidget();

public slots:
    void onAirportChanged();

private:
    void initData();
    void initUI();
    void initConnect();
    bool validate();

private slots:
    void onCodeChanged(int);
    void onConfirmClicked();

private:
    PgDataBase *pgDb;

    QStringList typeList;
    QStringList codeList;
    QList<Airport> airportList;

    QComboBox *codeComboBox;
    QLineEdit *nameEdit;
    QLineEdit *lonEdit;
    QLineEdit *latEdit;
    QLineEdit *altEdit;
    QLineEdit *dirEdit;
    QComboBox *typeComboBox;
    QPushButton *confirmButton;
};

/*** 机场设置widget ***/
class AirportSetupWidget : public QDialog{
    Q_OBJECT
public:
    AirportSetupWidget(QWidget *parent = 0);
    ~AirportSetupWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QTabWidget *tabWidget;
    AirportAddWidget *airportAddWidget;
    AirportModifyWidget *airportModifyWidget;
};

#endif // AIRPORTSETUPWIDGET

