#ifndef KFTTJINPUTWIDGET
#define KFTTJINPUTWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
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
    void onQxysChanged(int);
    void onSlfRadioButtonClicked(bool);
    void execute();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    PgDataBase *pgDb;
    QHash< int, QList<WeatherParam> > wpHash;

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

