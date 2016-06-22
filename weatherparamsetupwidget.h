#ifndef WEATHERPARAMSETUPWIDGET
#define WEATHERPARAMSETUPWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"
#include "bean/weatherparam.h"
#include "bean/weatherparamsetup.h"

/*** 多要素阀值设置widget ***/
class MultiWeatherParamWidget : public QWidget{
    Q_OBJECT
public:
    MultiWeatherParamWidget(QWidget *parent = 0);
    ~MultiWeatherParamWidget();

public slots:
    void onAirportChanged(QString);

private:
    void initData();
    void initUI();
    void initConnect();
    QList<QString> getDataFromJson(QString);

private slots:
    void onConfirmClicked();

private:
    PgDataBase *pgDb;

    QList<WeatherParam> weatherParamList;
    QList<WeatherParamSetup> weatherParamSetupList;

    QHash< int, QList<QLineEdit *> > editHash;

    QPushButton *confirmButton;

    QString currentApCode;
};

/*** 单要素阀值设置widget ***/
class SingleWeatherParamWidget : public QWidget{
    Q_OBJECT
public:
    SingleWeatherParamWidget(QWidget *parent = 0);
    ~SingleWeatherParamWidget();

public slots:
    void onAirportChanged(QString);

private:
    void initData();
    void initUI();
    void initConnect();
    QList<QString> getDataFromJson(QString);

private slots:
    void onConfirmClicked();

private:
    PgDataBase *pgDb;

    QList<WeatherParam> weatherParamList;
    QList<WeatherParamSetup> weatherParamSetupList;

    QHash< int, QList<QLineEdit *> > editHash;

    QPushButton *confirmButton;

    QString currentApCode;
};


/*** 气象参数设置widget ***/
class WeatherParamSetupWidget : public QDialog{
    Q_OBJECT
public:
    WeatherParamSetupWidget(QWidget *parent = 0);
    ~WeatherParamSetupWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private slots:
    void onAirportChanged(int);

private:
    PgDataBase *pgDb;

    QComboBox *airportComboBox;
    QTabWidget *tabWidget;

    QList<Airport> airportList;
    QStringList apCodeList;
    QStringList apNameList;

    MultiWeatherParamWidget *multiWeatherParamWidget;
    SingleWeatherParamWidget *singleWeatherParamWidget;
};

#endif // WEATHERPARAMSETUPWIDGET

