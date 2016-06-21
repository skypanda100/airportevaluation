#ifndef KFTTJRESULTWIDGET
#define KFTTJRESULTWIDGET

#include <QtWidgets>
#include "model/tablemodel.h"
#include "control/kfttjcontrol.h"
#include "control/excelcontrol.h"
#include "qchartviewer.h"
#include "chartdir.h"

class KfttjResultWidget : public QSplitter{
    Q_OBJECT
public:
    KfttjResultWidget(QWidget *parent = 0);
    ~KfttjResultWidget();
signals:
    void setProgressValue(int);

public slots:
    void receiveMessage(const QString &message, int row, int col, int rows = 1, int cols = 1);
    void receiveMessage(int row, int count);
    void execute(bool);

private:
    void initData();
    void initUI();
    void initConnect();
    void createCharts();
    void createNkfttjChart(QHash< QString, QList<float> >);
    void createKftyfbChart(QHash< QString, QList<float> >);
    void createZlwzdChart(QHash< QString, QList<float> >);
    void createXzkfNfbChart(QHash< QString, QStringList >);
    void createBkfNfbChart(QHash< QString, QStringList >);
    void createXzkfYfbChart(QHash< QString, QStringList >);
    void createBkfYfbChart(QHash< QString, QStringList >);

private:
    QList<QString> titleList;

    TableModel *tableModel;
    QTableView *tableView;
    QScrollArea *imageArea;
    QChartViewer *nkfttjChartView;
    QChartViewer *kftyfbChartView;
    QChartViewer *zlwzdChartView;
    QChartViewer *xzkfnfbChartView;
    QChartViewer *bkfnfbChartView;
    QChartViewer *xzkfyfbChartView;
    QChartViewer *bkfyfbChartView;

    KfttjControl *kfttjControl;
};

#endif // KFTTJRESULTWIDGET

