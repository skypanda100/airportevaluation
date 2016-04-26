#include "sjdrmainwidget.h"
#include "database/asdatabase.h"

/*** 左边 ***/
SjdrMainLeftWidget::SjdrMainLeftWidget(QWidget *parent)
    :QTreeWidget(parent)
{
    this->initUI();
    this->initConnect();
}

SjdrMainLeftWidget::~SjdrMainLeftWidget(){
    deleteItems();
}

void SjdrMainLeftWidget::initUI(){
    this->setHeaderLabel("数据源");
}

void SjdrMainLeftWidget::initConnect(){

}

void SjdrMainLeftWidget::addItem(const QString &item, const QString &parentItem){
    if(parentItem.isEmpty()){
        QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(this, QStringList(item));
        Q_UNUSED(treeWidgetItem);
    }else{
        int count = this->topLevelItemCount();
        for(int i = 0;i < count;i++){
            QTreeWidgetItem *topTreeWidgetItem = this->topLevelItem(i);
            if(topTreeWidgetItem->text(0).compare(parentItem) == 0){
                QTreeWidgetItem *subTreeWidgetItem = new QTreeWidgetItem(topTreeWidgetItem);
                subTreeWidgetItem->setText(0, item);
                this->expandItem(topTreeWidgetItem);
                break;
            }
        }
    }
}

void SjdrMainLeftWidget::deleteItems(){
    int count = this->topLevelItemCount();
    for(int i = count - 1;i >= 0;i--){
        QTreeWidgetItem *topTreeWidgetItem = this->topLevelItem(i);
        delete topTreeWidgetItem;
    }
}

/*** 右边 ***/
SjdrMainRightWidget::SjdrMainRightWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initUI();
    this->initConnect();
}

SjdrMainRightWidget::~SjdrMainRightWidget(){

}

void SjdrMainRightWidget::initUI(){

}

void SjdrMainRightWidget::initConnect(){

}

/*** 主 ***/
SjdrMainWidget::SjdrMainWidget(QWidget *parent)
    :QSplitter(parent)
{
    this->initUI();
    this->initConnect();
}

SjdrMainWidget::~SjdrMainWidget(){
    delete sjdrMainLeftWidget;
    delete sjdrMainRightWidget;
}

void SjdrMainWidget::initUI(){
    this->setAutoFillBackground(true);
    QPalette palette(QColor(255, 255, 255));
    this->setPalette(palette);

    sjdrMainLeftWidget = new SjdrMainLeftWidget;
    sjdrMainRightWidget = new SjdrMainRightWidget;

    this->setOrientation(Qt::Horizontal);
    this->addWidget(sjdrMainLeftWidget);
    this->addWidget(sjdrMainRightWidget);
}

void SjdrMainWidget::initConnect(){

}

void SjdrMainWidget::executeSjdr(Airport airport, QList<QualityControlSource> qualityControlSourceList, QList<QString> fileList){
    //清除之前的数据
    sjdrMainLeftWidget->deleteItems();
    srcHash.clear();
    currentAirport = airport;

    //在右侧显示数据源
    int qualityControlSourceCount = qualityControlSourceList.size();
    for(int i = 0;i < qualityControlSourceCount;i++){
        QualityControlSource qualityControlSource = qualityControlSourceList[i];
        sjdrMainLeftWidget->addItem(qualityControlSource.name(), "");
    }
    sjdrMainLeftWidget->addItem("未知", "");

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
                        sjdrMainLeftWidget->addItem(fileInfo.fileName(), qualityControlSource.name());
                        break;
                    }
                }else{
                    if(title.compare(qualityControlSource.fields()) == 0){
                        srcHash[j].append(fileInfo.fileName());
                        sjdrMainLeftWidget->addItem(fileInfo.fileName(), qualityControlSource.name());
                        break;
                    }
                }
            }
            if(j == qualityControlSourceCount){
                sjdrMainLeftWidget->addItem(fileInfo.fileName(), "未知");
            }
        }else if(path.endsWith(".mdb", Qt::CaseInsensitive)){
            AsDataBase asDb(path);
            QStringList tables = asDb.queryTables();
            int tableCount = tables.count();
            for(int k = 0;k < tableCount;k++){
                QString title = asDb.queryFields(tables[k]).join(",");
                int j = 0;
                QFileInfo fileInfo(file);
                for(;j < qualityControlSourceCount;j++){
                    QualityControlSource qualityControlSource = qualityControlSourceList[j];
                    if(title.compare(qualityControlSource.fields()) == 0){
                        srcHash[j].append(fileInfo.fileName());
                        sjdrMainLeftWidget->addItem(fileInfo.fileName(), qualityControlSource.name());
                        break;
                    }
                }
                if(j == qualityControlSourceCount){
                    sjdrMainLeftWidget->addItem(fileInfo.fileName(), "未知");
                }
            }
        }
    }
}
