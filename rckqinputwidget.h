#ifndef RCKQINPUTWIDGET
#define RCKQINPUTWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"

class RckqDateWidget : public QWidget{
    Q_OBJECT
public:
    RckqDateWidget(QWidget *parent = 0);
    ~RckqDateWidget();

    QString text();
    void changeEditType(int editType = 0);

signals:
    void deleteDate();
    void addDate();

private:
    void initUI();
    void initConnect();

private slots:
    void onEditButtonClicked();

private:
    int m_EditType;
    QLineEdit *dateEdit;
    QPushButton *dateEditButton;
};

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
    void onDelDateClicked();
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
    void resetDate();

private:
    PgDataBase *pgdb;
    QList<Airport> airportList;
    QHash< QString, QList<QString> > runwayHash;

    QComboBox *airportComboBox;
    QComboBox *runwayComboBox;
    QComboBox *typeComboBox;
    QComboBox *fhourComboBox;
    QComboBox *thourComboBox;
    QListWidget *dateListWidget;
    QList<RckqDateWidget *> dateEditList;
    QList<QString> weatherList;
    QList<QCheckBox *> weatherCheckBoxList;
    QList<QRadioButton *> weatherRadioButtonList;
    QGridLayout *weatherGLayout;
    QPushButton *executeButton;
    QPushButton *exportButton;

};

#endif // RCKQINPUTWIDGET

