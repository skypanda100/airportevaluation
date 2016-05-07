#ifndef RCKQRESULTWIDGET
#define RCKQRESULTWIDGET

#include <QtWidgets>
#include "control/rckqcontrol.h"

class RckqResultWidget : public QSplitter{
    Q_OBJECT
public:
    RckqResultWidget(QWidget *parent = 0);
    ~RckqResultWidget();

signals:
    void setProgressValue(int);

public slots:
    void executeRckq(QString code, QString runway, int type, int fhour, int thour, QList<QString> dateList, QList<QString> weatherList);

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QStandardItemModel *tableModel;
    QTableView *tableView;
    QScrollArea *imageArea;
    QWidget *imageWidget;

    QList<QString> titleList;
    RckqControl *rckqControl;
};

#endif // RCKQRESULTWIDGET

