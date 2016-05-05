#include "tablemodel.h"

TableModel::TableModel(int rows, int columns, QObject *parent)
    :QStandardItemModel(rows, columns, parent)
{

}

TableModel::~TableModel(){

}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    bool ret = QStandardItemModel::setHeaderData(section, orientation, value, role);
    m_headerData.append(value);
    return ret;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role){
    bool ret = QStandardItemModel::setData(index, value, role);

    if(m_headerData[index.column()].toString().compare("完全可飞") == 0){
        this->item(index.row(), index.column())->setBackground(QColor(50, 150, 0));
        this->item(index.row(), index.column())->setTextAlignment(Qt::AlignCenter);
    }else if(m_headerData[index.column()].toString().compare("限制可飞") == 0){
        this->item(index.row(), index.column())->setBackground(QColor(255, 180, 0));
        this->item(index.row(), index.column())->setTextAlignment(Qt::AlignCenter);
    }else if(m_headerData[index.column()].toString().compare("不可飞") == 0){
        this->item(index.row(), index.column())->setBackground(QColor(220, 50, 0));
        this->item(index.row(), index.column())->setTextAlignment(Qt::AlignCenter);
    }else if(m_headerData[index.column()].toString().compare("影响原因") == 0){
        this->item(index.row(), index.column())->setBackground(QColor(178, 161, 199));
    }else{
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
    }

    return ret;
}
