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
    void onAirportChanged(QString, QString);

private:
    void initData();
    void initUI();
    void initConnect();
    QList<QString> getDataFromJson(QString);
    QString getTitle(const QString &);

private slots:
    void onConfirmClicked();

private:
    PgDataBase *pgDb;

    QList<WeatherParam> weatherParamList;
    QList<WeatherParamSetup> weatherParamSetupList;

    QHash< int, QList<QLineEdit *> > editHash;

    QPushButton *confirmButton;

    QString currentApCode;
    QString currentPName;
};

/*** 单要素阀值设置widget ***/
class SingleWeatherParamWidget : public QWidget{
    Q_OBJECT
public:
    SingleWeatherParamWidget(QWidget *parent = 0);
    ~SingleWeatherParamWidget();

public slots:
    void onAirportChanged(QString, QString);

private:
    void initData();
    void initUI();
    void initConnect();
    QList<QString> getDataFromJson(QString);
    QString getTitle(const QString &);

private slots:
    void onConfirmClicked();

private:
    PgDataBase *pgDb;

    QList<WeatherParam> weatherParamList;
    QList<WeatherParamSetup> weatherParamSetupList;

    QHash< int, QList<QLineEdit *> > editHash;

    QPushButton *confirmButton;

    QString currentApCode;
    QString currentPName;
};


/*** 气象参数设置widget ***/
class WeatherParamSetupWidget : public QDialog{
    Q_OBJECT
public:
    WeatherParamSetupWidget(QWidget *parent = 0);
    ~WeatherParamSetupWidget();

signals:
    void previousClicked();
    void nextClicked();

private:
    void initData();
    void initUI();
    void initConnect();

private slots:
    void onAirportChanged(int);
    void onPlaneNameChanged(QString);

private:
    PgDataBase *pgDb;

    QComboBox *airportComboBox;
    QComboBox *planeNameComboBox;
    QTabWidget *tabWidget;

    QList<Airport> airportList;
    QStringList apCodeList;
    QStringList apNameList;

    MultiWeatherParamWidget *multiWeatherParamWidget;
    SingleWeatherParamWidget *singleWeatherParamWidget;

    QPushButton *previousButton;
    QPushButton *nextButton;
};

#endif // WEATHERPARAMSETUPWIDGET

