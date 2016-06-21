#include "excelcontrol.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
QTXLSX_USE_NAMESPACE

ExcelControl::ExcelControl(QObject *parent)
    :m_tableView(NULL)
    ,m_tableModel(NULL)
    ,m_path("")
    ,QThread(parent)
{

}

ExcelControl::~ExcelControl(){

}

void ExcelControl::setTableParam(const QTableView *tableView, const TableModel *tableModel){
    this->m_tableView = tableView;
    this->m_tableModel = tableModel;
}

void ExcelControl::setExportPath(QString path){
    this->m_path = path;
}

void ExcelControl::run(){
    if(m_tableView == NULL || m_tableModel == NULL){
        return;
    }

    int rowCount = m_tableModel->rowCount();
    int colCount = m_tableModel->columnCount();
    qDebug() << rowCount << colCount;

    Document xlsx;
    //单元格格式
    Format cellFormat;
    //颜色
    QColor originColor = cellFormat.patternBackgroundColor();
    //水平居中
    cellFormat.setHorizontalAlignment(Format::AlignHCenter);
    //垂直居中
    cellFormat.setVerticalAlignment(Format::AlignVCenter);
    //边框
    cellFormat.setBorderStyle(Format::BorderThin);
    //设置字体
    cellFormat.setFont(QFont("微软雅黑"));
    cellFormat.setFontSize(9);

    //添加标题
    cellFormat.setPatternBackgroundColor(QColor(0, 176, 200));
    for(int i = 0;i < colCount;i++){
        //获取标题单元格值
        QString headerValue = m_tableModel->headerData(i, Qt::Horizontal).toString();
        //字体加粗
        cellFormat.setFontBold(true);
        //给单元格设值并合并单元格
        xlsx.write(1, i + 1, headerValue, cellFormat);
    }
    //字体不加粗
    cellFormat.setFontBold(false);
    //循环添加单元格内容
    for(int i = 0;i < rowCount;i++){
        for(int j = 0;j < colCount;j++){
            QStandardItem *cellItem = m_tableModel->item(i, j);
            if(cellItem != NULL){
                //获取单元格值
                QString cellValue = cellItem->text();
                //获取单元格背景色
                QColor cellBgColor = cellItem->background().color();
                //给单元格设值并合并单元格
                int rowSpan = m_tableView->rowSpan(i, j);
                int colSpan = m_tableView->columnSpan(i, j);
                cellFormat.setPatternBackgroundColor(cellBgColor);
                xlsx.write(i + 2, j + 1, cellValue, cellFormat);
                xlsx.mergeCells(CellRange(i + 2, j + 1, i + rowSpan + 1, j + colSpan), cellFormat);
            }else{
                //设置空值
                int rowSpan = m_tableView->rowSpan(i, j);
                int colSpan = m_tableView->columnSpan(i, j);
                if(rowSpan == 1 && colSpan == 1){
                    cellFormat.setPatternBackgroundColor(originColor);
                    xlsx.write(i + 2, j + 1, "", cellFormat);
                }
            }
            int progress = (int)((i * colCount + j + 1) / (qreal)(rowCount * colCount) * 100);
            emit setProgressValue(progress);
        }
    }
    //保存excel
    if(m_path.isEmpty()){
        xlsx.save();
    }else{
        xlsx.saveAs(m_path);
    }

    //初始化
    m_tableView = NULL;
    m_tableModel = NULL;

    //发送处理完成的信号
    emit execute(true);

    qDebug() << "excel done";
}
