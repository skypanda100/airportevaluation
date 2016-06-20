#ifndef EXCELCONTROL
#define EXCELCONTROL

#include <QThread>
#include <QTableView>
#include "../model/tablemodel.h"

class ExcelControl : public QThread{
    Q_OBJECT

public:
    explicit ExcelControl(QObject *parent = 0);
    ~ExcelControl();

    void run() Q_DECL_OVERRIDE;

    void setTableParam(const QTableView *tableView, const TableModel *tableModel);
    void setExportPath(QString path);

signals:
    void setProgressValue(int);
    void execute(bool isEnd);

private:
    const QTableView *m_tableView;
    const TableModel *m_tableModel;
    QString m_path;
};

#endif // EXCELCONTROL

