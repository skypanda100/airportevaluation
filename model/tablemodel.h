#ifndef TABLEMODEL
#define TABLEMODEL

#include <QtWidgets>

class TableModel : public QStandardItemModel{
    Q_OBJECT
public:
    TableModel(int, int, QObject *parent = 0, bool hasColor = false);
    ~TableModel();

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
    QList<QVariant> m_headerData;
    bool m_hasColor;
};

#endif // TABLEMODEL

