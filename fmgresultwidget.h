#ifndef FMGRESULTWIDGET
#define FMGRESULTWIDGET

#include <QtWidgets>
#include "control/fmgcontrol.h"
#include "control/excelcontrol.h"
#include "qchartviewer.h"
#include "chartdir.h"
#include "model/tablemodel.h"

class FmgResultWidget : public QSplitter{
    Q_OBJECT
public:
    FmgResultWidget(QWidget *parent = 0);
    ~FmgResultWidget();

signals:
    void setProgressValue(int);

public slots:
    void executeFmg(QString code, QString runway, QString fspeed, QString tspeed, QList<QString> years);
    void executeExport();
    void receiveMessage(int row, QString year);
    void receiveMessage(int row, QString year, QList<QString> windCountList);
    void execute(bool);
    void xlsExecute(bool);
    void exportFiles(QHash< int, QList<QString> >);

private:
    void initData();
    void initUI();
    void initConnect();
    void createCharts();

private:
    QHash<QString, int> windCountHash;
    QHash< QString, QList< QList<QString> > > windHash;
    QList<QString> titleList;
    FmgControl *fmgControl;
    ExcelControl *excelControl;

    TableModel *tableModel;
    QTableView *tableView;
    QScrollArea *imageArea;
    QList<QChartViewer *> polarChartViewList;
    QHBoxLayout *imageLayout;
    QWidget *imageWidget;

};

#endif // FMGRESULTWIDGET

