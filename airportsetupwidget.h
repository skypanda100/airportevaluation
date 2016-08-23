#ifndef AIRPORTSETUPWIDGET
#define AIRPORTSETUPWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"

/*** 机型ComboBox的Item ***/
class PlaneItemWidget : public QWidget{
    Q_OBJECT
public:
    PlaneItemWidget(QWidget *parent = 0);
    ~PlaneItemWidget();
    void setText(QString);
    QString text();

signals:
    void deleteItem();
    void showText(QString);

protected:
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();

private:
    QLineEdit *valueEdit;
    QPushButton *deleteButton;
    bool mousePress;
};


/*** 机场添加widget ***/
class AirportAddWidget : public QWidget{
    Q_OBJECT
public:
    AirportAddWidget(QWidget *parent = 0);
    ~AirportAddWidget();

signals:
    void airportChanged();

protected:
    bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;

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
    void onItemDelete();
    void onItemShow(QString);

private:
    PgDataBase *pgDb;

    QStringList typeList;

    QLineEdit *codeEdit;
    QLineEdit *nameEdit;
    QComboBox *planeNameComboBox;
    QListWidget *planeNameListWidget;
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

protected:
    bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();
    bool validate();

private slots:
    void onCodeChanged(int);
    void onConfirmClicked();
    void onItemDelete();
    void onItemShow(QString);

private:
    PgDataBase *pgDb;

    QStringList typeList;
    QStringList codeList;
    QList<Airport> airportList;

    QComboBox *codeComboBox;
    QLineEdit *nameEdit;
    QComboBox *planeNameComboBox;
    QListWidget *planeNameListWidget;
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

signals:
    void previousClicked();
    void nextClicked();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QTabWidget *tabWidget;
    AirportAddWidget *airportAddWidget;
    AirportModifyWidget *airportModifyWidget;
    QPushButton *previousButton;
    QPushButton *nextButton;
};

#endif // AIRPORTSETUPWIDGET

