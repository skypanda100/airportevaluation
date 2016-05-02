#include "tablemodel.h"

TableModel::TableModel(int rows, int columns, QObject *parent)
    :QStandardItemModel(rows, columns, parent)
{

}

TableModel::~TableModel(){

}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role){
    bool ret = QStandardItemModel::setData(index, value, role);
    if(value.toString().compare("3") == 0){
        this->item(index.row(), index.column())->setBackground(QColor(50, 150, 0));
        this->item(index.row(), index.column())->setTextAlignment(Qt::AlignCenter);
    }else if(value.toString().compare("2") == 0){
        this->item(index.row(), index.column())->setBackground(QColor(255, 180, 0));
        this->item(index.row(), index.column())->setTextAlignment(Qt::AlignCenter);
    }else if(value.toString().compare("1") == 0){
        this->item(index.row(), index.column())->setBackground(QColor(220, 50, 0));
        this->item(index.row(), index.column())->setTextAlignment(Qt::AlignCenter);
    }

    return ret;
}
