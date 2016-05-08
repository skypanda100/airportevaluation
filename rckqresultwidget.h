#ifndef RCKQRESULTWIDGET
#define RCKQRESULTWIDGET

#include <QtWidgets>
#include "control/rckqcontrol.h"
#include "qchartviewer.h"
#include "chartdir.h"

class RckqResultWidget : public QSplitter{
    Q_OBJECT
public:
    RckqResultWidget(QWidget *parent = 0);
    ~RckqResultWidget();

signals:
    void setProgressValue(int);

public slots:
    void executeRckq(QString code, QString runway, int type, int fhour, int thour, QList<QString> dateList, QList<QString> weatherList);
    void receiveMessage(int row, QString weather, QString dateStr, QList<QString> valueList, QString keyStr);
    void execute(bool);

private:
    void initData();
    void initUI();
    void initConnect();
    void createCharts();

private:
    QStandardItemModel *tableModel;
    QTableView *tableView;
    QScrollArea *imageArea;
    QWidget *imageWidget;

    QList<QString> titleList;
    RckqControl *rckqControl;

    QHash< QString, QList<QString> > dataHash;
    int m_type;
    int m_fhour;
    int m_thour;
    QList<QString> m_dateList;
    QList<QString> m_weatherList;
};

#endif // RCKQRESULTWIDGET

