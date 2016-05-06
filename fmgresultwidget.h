#ifndef FMGRESULTWIDGET
#define FMGRESULTWIDGET

#include <QtWidgets>
#include "control/fmgcontrol.h"
#include "qchartviewer.h"
#include "chartdir.h"

class FmgResultWidget : public QSplitter{
    Q_OBJECT
public:
    FmgResultWidget(QWidget *parent = 0);
    ~FmgResultWidget();

signals:
    void setProgressValue(int);

public slots:
    void executeFmg(QString code, QString runway, QString fspeed, QString tspeed, QList<QString> years);
    void receiveMessage(int row, QString year);
    void receiveMessage(int row, QString year, QList<QString> windCountList);
    void execute(bool);

private:
    void initData();
    void initUI();
    void initConnect();
    void createCharts();

private:
    QHash< QString, QList< QList<QString> > > windHash;
    QList<QString> titleList;
    FmgControl *fmgControl;

    QStandardItemModel *tableModel;
    QTableView *tableView;
    QScrollArea *imageArea;
    QWidget *imageWidget;

};

#endif // FMGRESULTWIDGET

