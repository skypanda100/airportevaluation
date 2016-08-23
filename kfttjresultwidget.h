#ifndef KFTTJRESULTWIDGET
#define KFTTJRESULTWIDGET

#include <QtWidgets>
#include "bean/weatherparam.h"
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
    void setProgressValue(int,int);

public slots:
    void executeKfttj(QString airportCode, QString planeName, QList<QString> dateList, bool isMultiWeather, QList<WeatherParam> wpList);
    void executeExport();
    void receiveMessage(const QString &message, int row, int col, int rows = 1, int cols = 1);
    void receiveMessage(int row, int count);
    void execute(bool);
    void xlsExecute(bool);
    void exportFiles(QHash< int, QList<QString> >);

private:
    void initData();
    void initUI();
    void initConnect();
    void createCharts();
    void createTable(bool isHourTable);
    void createNkfttjChart(const QHash< QString, QList<float> >&);
    void createKftyfbChart(const QHash< QString, QList<float> >&);
    void createZlwzdChart(const QHash< QString, QList<float> >&);
    void createXzkfNfbChart(const QHash< QString, QStringList >&);
    void createBkfNfbChart(const QHash< QString, QStringList >&);
    void createXzkfYfbChart(const QHash< QString, QStringList >&);
    void createBkfYfbChart(const QHash< QString, QStringList >&);

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
    ExcelControl *excelControl;
};

#endif // KFTTJRESULTWIDGET

