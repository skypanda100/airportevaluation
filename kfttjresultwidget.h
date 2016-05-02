#ifndef KFTTJRESULTWIDGET
#define KFTTJRESULTWIDGET

#include <QtWidgets>
#include "model/tablemodel.h"
#include "control/kfttjcontrol.h"

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

private:
    QList<QString> titleList;

    TableModel *tableModel;
    QTableView *tableView;

    KfttjControl *kfttjControl;
};

#endif // KFTTJRESULTWIDGET

