#ifndef EXCELCONTROL
#define EXCELCONTROL

#include <QThread>
#include <QTableView>
#include "../model/tablemodel.h"

class ExcelControl : public QThread{
    Q_OBJECT

public:
    explicit ExcelControl(QTableView *tableView, TableModel *tableModel, QObject *parent = 0);
    ~ExcelControl();

    void run() Q_DECL_OVERRIDE;

private:
    QTableView *m_tableView;
    TableModel *m_tableModel;
};

#endif // EXCELCONTROL

