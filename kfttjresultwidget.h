#ifndef KFTTJRESULTWIDGET
#define KFTTJRESULTWIDGET

#include <QtWidgets>
#include "model/tablemodel.h"
#include "control/kfttjcontrol.h"
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
    void createCharts(QHash< QString, QList<float> >);
    void createNkfttjChart(QHash< QString, QList<float> >, QLayout *);
    void createKftyfbChart(QHash< QString, QList<float> >, QLayout *);
    void createZlwzdChart(QHash< QString, QList<float> >, QLayout *);

private:
    QList<QString> titleList;

    TableModel *tableModel;
    QTableView *tableView;
    QScrollArea *imageArea;
    QWidget *imageWidget;

    KfttjControl *kfttjControl;
};

#endif // KFTTJRESULTWIDGET

