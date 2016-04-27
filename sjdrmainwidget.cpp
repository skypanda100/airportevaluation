#include "sjdrmainwidget.h"
#include "database/asdatabase.h"
#include "model/treemodel.h"

SjdrMainWidget::SjdrMainWidget(QWidget *parent)
    :QTreeView(parent)
    ,sjdrControl(NULL)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

SjdrMainWidget::~SjdrMainWidget(){
    if(sjdrControl != NULL){
        sjdrControl->setLoop(false);
        sjdrControl->quit();
        sjdrControl->wait();
    }
}

void SjdrMainWidget::initData(){
    headers << "文件"
            << "信息"
            << "类型"
            << "路径";
}

void SjdrMainWidget::initUI(){

    TreeModel *model = new TreeModel(headers, "");
    this->setModel(model);
    for (int column = 0; column < model->columnCount(); ++column){
        this->resizeColumnToContents(column);
    }
    this->setAlternatingRowColors(true);
}

void SjdrMainWidget::initConnect(){

}

void SjdrMainWidget::executeSjdr(Airport airport, QList<QualityControlSource> qualityControlSourceList, QList<QString> fileList){
    //清除之前的数据
    removeAllRows();

    srcHash.clear();
    currentAirport = airport;

    if(sjdrControl == NULL){
        sjdrControl = new SjdrControl;
        connect(sjdrControl, SIGNAL(sendMessage(QStringList)), this, SLOT(receiveMessage(QStringList)), Qt::QueuedConnection);
        sjdrControl->setAirport(currentAirport);
        sjdrControl->start();
    }

    //给数据源分类
    assortSource(qualityControlSourceList, fileList);
}

void SjdrMainWidget::assortSource(QList<QualityControlSource> qualityControlSourceList, QList<QString> fileList){
    int qualityControlSourceCount = qualityControlSourceList.size();
    int fileCount = fileList.size();
    for(int i = 0;i < fileCount;i++){
        QString path = fileList[i];
        QFile file(path);
        QString title;
        if(path.endsWith(".his", Qt::CaseInsensitive)){
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                qDebug() << "不能读取文件[" + path + "]";
            }else{
                QTextStream fileInput(&file);
                QString lineStr;
                while(!fileInput.atEnd())
                {
                    lineStr = fileInput.readLine();
                    if(lineStr.compare("History file") == 0){
                        continue;
                    }else{
                        title = lineStr.replace("	", ",");
                        break;
                    }
                }
                file.close();
            }
            int j = 0;
            QFileInfo fileInfo(file);
            for(;j < qualityControlSourceCount;j++){
                QualityControlSource qualityControlSource = qualityControlSourceList[j];
                //大陆自动站温度气压特殊处理
                if(qualityControlSource.id() == 5){
                    if(title.replace(QRegExp("[0-9]+"), "").compare(qualityControlSource.fields().replace(QRegExp("[0-9]+"), "")) == 0){
                        srcHash[j].append(fileInfo.fileName());
                        dataControl(fileInfo, qualityControlSource);
                        break;
                    }
                }else{
                    if(title.compare(qualityControlSource.fields()) == 0){
                        srcHash[j].append(fileInfo.fileName());
                        dataControl(fileInfo, qualityControlSource);
                        break;
                    }
                }
            }
            if(j == qualityControlSourceCount){
                QualityControlSource qcs;
                qcs.setId(UNKNOWN);
                qcs.setName("未知");
                dataControl(fileInfo, qcs);
            }
        }else if(path.endsWith(".mdb", Qt::CaseInsensitive)){
            AsDataBase asdb(path);
            QStringList tables = asdb.queryTables();
            int tableCount = tables.count();
            for(int k = 0;k < tableCount;k++){
                QString title = asdb.queryFields(tables[k]).join(",");
                int j = 0;
                QFileInfo fileInfo(file);
                for(;j < qualityControlSourceCount;j++){
                    QualityControlSource qualityControlSource = qualityControlSourceList[j];
                    if(title.compare(qualityControlSource.fields()) == 0){
                        srcHash[j].append(fileInfo.fileName());
                        dataControl(fileInfo, qualityControlSource);
                        break;
                    }
                }
                if(j == qualityControlSourceCount){
                    QualityControlSource qcs;
                    qcs.setId(UNKNOWN);
                    qcs.setName("未知");
                    dataControl(fileInfo, qcs);
                }
            }
        }
    }

//    QAbstractItemModel *model = this->model();
//    for (int column = 0; column < model->columnCount(); ++column){
//        this->resizeColumnToContents(column);
//    }
}

void SjdrMainWidget::insertRow(const QString &name, const QString &info, const QString &type, const QString &path){
    QModelIndex index = this->selectionModel()->currentIndex();
    QAbstractItemModel *model = this->model();

    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    updateActions();

    QModelIndex child1 = model->index(index.row()+1, 0, index.parent());
    model->setData(child1, QVariant(name), Qt::EditRole);

    QModelIndex child2 = model->index(index.row()+1, 1, index.parent());
    model->setData(child2, QVariant(info), Qt::EditRole);

    QModelIndex child3 = model->index(index.row()+1, 2, index.parent());
    model->setData(child3, QVariant(type), Qt::EditRole);

    QModelIndex child4 = model->index(index.row()+1, 3, index.parent());
    model->setData(child4, QVariant(path), Qt::EditRole);
}

void SjdrMainWidget::removeAllRows(){
    QAbstractItemModel *model = this->model();
    if(model->rowCount() > 0){
        model->removeRows(0, model->rowCount());
    }
}

void SjdrMainWidget::updateActions()
{
//    bool hasCurrent = this->selectionModel()->currentIndex().isValid();

//    if (hasCurrent) {
//        this->closePersistentEditor(this->selectionModel()->currentIndex());

//        int row = this->selectionModel()->currentIndex().row();
//        int column = this->selectionModel()->currentIndex().column();
//        if (this->selectionModel()->currentIndex().parent().isValid())
//            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
//        else
//            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
//    }
}

void SjdrMainWidget::dataControl(const QFileInfo &fileInfo, const QualityControlSource &qualityControlSource){
    sjdrControl->addTask(SjdrElement(qualityControlSource, fileInfo));
}

void SjdrMainWidget::receiveMessage(QStringList messages){
    insertRow(messages[0], messages[1], messages[2], messages[3]);
}
