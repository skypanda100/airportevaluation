#ifndef AIRPORTSETUPWIDGET
#define AIRPORTSETUPWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"

/*** 机场添加widget ***/
class AirportAddWidget : public QWidget{
    Q_OBJECT
public:
    AirportAddWidget(QList<Airport> airportList, QWidget *parent = 0);
    ~AirportAddWidget();

private:
    void initData();
    void initUI();
    void initConnect();
    bool validate();

private slots:
    void onConfirmClicked();

private:
    QList<Airport> airportList;
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
    AirportModifyWidget(QList<Airport> airportList, QWidget *parent = 0);
    ~AirportModifyWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private slots:
    void onCodeChanged(int);

private:
    QList<Airport> airportList;
    QStringList typeList;
    QStringList codeList;

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
    QWidget *airportAddWidget;
    QWidget *airportModifyWidget;

    PgDataBase *pgDb;
    QList<Airport> airportList;
};

#endif // AIRPORTSETUPWIDGET

