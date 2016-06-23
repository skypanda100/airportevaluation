#ifndef KFTTJINPUTWIDGET
#define KFTTJINPUTWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/airport.h"
#include "bean/weatherparam.h"

class KfttjInputWidget : public QWidget{
    Q_OBJECT
public:
    KfttjInputWidget(QWidget *parent = 0);
    ~KfttjInputWidget();

signals:
    void executeKfttj();
    void executeExport();

private slots:
    void onAirportChanged(int);
    void onQxysChanged(int);
    void onSlfRadioButtonClicked(bool);
    void execute();
    void onAirportInfoChanged(QList<Airport>, QHash< QString, QList<QString> >);

private:
    void initData();
    void initUI();
    void initConnect();
    //查询机场表
    void queryAirport();
    //重置机场列表
    void resetAirportComboBox(QList<Airport> airportList, bool isSave = false);
    //重置日期
    void resetDateArea();

private:
    QList<Airport> airportList;
    QList<QString> dateList;

    PgDataBase *pgDb;
    QHash< int, QList<WeatherParam> > wpHash;

    QComboBox *airportComboBox;
    QList<QCheckBox *> dateCheckBoxList;
    QGridLayout *dateLayout;
    QComboBox *qxysComboBox;
    QGroupBox *fqfGroupBox;
    QRadioButton *slfRadioButton;
    QCheckBox *sfCheckBox;
    QCheckBox *nfCheckBox;
    QCheckBox *cfCheckBox;
    QRadioButton *blfRadioButton;
    QGroupBox *wdGroupBox;
    QRadioButton *cgwRadioButton;
    QGroupBox *qyGroupBox;
    QRadioButton *cmqyRadioButton;
    QRadioButton *xzhmqyRadioButton;
    QGroupBox *sdGroupBox;
    QRadioButton *cgsRadioButton;
    QRadioButton *jsRadioButton;
    QGroupBox *njdGroupBox;
    QRadioButton *spnjdRadioButton;
    QGroupBox *yGroupBox;
    QRadioButton *ylygRadioButton;
    QGroupBox *jsGroupBox;
    QRadioButton *fqjsRadioButton;
    QGroupBox *wxtqGroupBox;
    QCheckBox *njyCheckBox;
    QCheckBox *jyyCheckBox;
    QCheckBox *sdCheckBox;
    QCheckBox *lbCheckBox;
    QCheckBox *bbCheckBox;
    QCheckBox *bxCheckBox;
    QCheckBox *ljCheckBox;
    QCheckBox *scbCheckBox;
    QCheckBox *fqbCheckBox;

    QScrollArea *multiScrollArea;

    QRadioButton *ssfRadioButton;
    QRadioButton *snfRadioButton;
    QRadioButton *scfRadioButton;
    QRadioButton *sblfRadioButton;
    QRadioButton *sjgwRadioButton;
    QRadioButton *sjdwRadioButton;
    QRadioButton *sqjsRadioButton;

    QScrollArea *singleScrollArea;

    QPushButton *executeButton;
    QPushButton *exportButton;
};
#endif // KFTTJINPUTWIDGET

