#ifndef FMGRESULTWIDGET
#define FMGRESULTWIDGET

#include <QtWidgets>
#include "control/fmgcontrol.h"

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
    void receiveMessage(int row, QList<QString> windCountList);

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QList<QString> titleList;
    FmgControl *fmgControl;

    QStandardItemModel *tableModel;
    QTableView *tableView;
};

#endif // FMGRESULTWIDGET

