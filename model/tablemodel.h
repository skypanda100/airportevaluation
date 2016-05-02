#ifndef TABLEMODEL
#define TABLEMODEL

#include <QtWidgets>

class TableModel : public QStandardItemModel{
    Q_OBJECT
public:
    TableModel(int, int, QObject *parent = 0);
    ~TableModel();

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
};

#endif // TABLEMODEL

