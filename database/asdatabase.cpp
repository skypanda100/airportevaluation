#include "database/asdatabase.h"

AsDataBase::AsDataBase(QString filePath)
{
    this->openDb(filePath);
}

AsDataBase::~AsDataBase(){
    this->closeDb();
}

void AsDataBase::openDb(QString filePath){
    QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=%1").arg(filePath);
    if(QSqlDatabase::contains("access")){
      db = QSqlDatabase::database("access");
    }else{
      db = QSqlDatabase::addDatabase("QODBC", "access");
    }
    db.setDatabaseName(dsn);
    if(!db.isOpen()){
        if(!db.open()){
            QMessageBox::critical(0, QObject::tr("错误提示"),
                                  db.lastError().text());
        }
    }
}

//关于access数据库得好好想想
void AsDataBase::closeDb(){
//    db.close();
}

QSqlQueryModel* AsDataBase::queryModel(const QString &queryStr){
    if(!db.isOpen()){
        if(!db.open()){

        }
    }
    QSqlQueryModel *plainModel = new QSqlQueryModel;
    plainModel->setQuery(queryStr, db);
    return plainModel;
}

QList<QVariant> AsDataBase::queryVariant(const QString &queryStr){
    if(!db.isOpen()){
        if(!db.open()){

        }
    }
    QList<QVariant> variants;
    QSqlQueryModel *plainModel = new QSqlQueryModel;
    plainModel->setQuery(queryStr, db);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        variants.append(plainModel->record(i).value(0));
    }
    delete plainModel;
    return variants;
}

QStringList AsDataBase::queryTables(){
    if(!db.isOpen()){
        if(!db.open()){

        }
    }
    QStringList tableList = db.tables();
    return tableList;
}

QStringList AsDataBase::queryFields(QString tableStr){
    if(!db.isOpen()){
        if(!db.open()){

        }
    }
    QString queryStr = QString("select * from %1").arg(tableStr);
    QSqlQuery query(queryStr, db);
    QSqlRecord record = query.record();
    int fieldCount = record.count();
    QStringList fields;
    for(int i = 0;i < fieldCount;i++){
        QString fieldName = record.fieldName(i);
        fields.append(fieldName);
    }
    return fields;
}
