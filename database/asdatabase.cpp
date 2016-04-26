#include "database/asdatabase.h"

AsDataBase::AsDataBase(QString filePath)
{
    this->openDb(filePath);
}

AsDataBase::~AsDataBase(){
    this->closeDb();
}

void AsDataBase::openDb(QString filePath){
    db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=%1").arg(filePath);
    db.setDatabaseName(dsn);
    if(!db.open()){
        QMessageBox::critical(0, QObject::tr("错误提示"),
                              db.lastError().text());
    }
}

void AsDataBase::closeDb(){
    db.close();
}

QSqlQueryModel* AsDataBase::queryModel(const QString &queryStr){
    QSqlQueryModel *plainModel = new QSqlQueryModel;
    plainModel->setQuery(queryStr);
    return plainModel;
}

QList<QVariant> AsDataBase::queryVariant(const QString &queryStr){
    QList<QVariant> variants;
    QSqlQueryModel *plainModel = new QSqlQueryModel;
    plainModel->setQuery(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        variants.append(plainModel->record(i).value(0));
    }
    delete plainModel;
    return variants;
}

QStringList AsDataBase::queryTables(){
    return db.tables();
}

QStringList AsDataBase::queryFields(QString tableStr){
    QString queryStr = QString("select * from %1").arg(tableStr);
    QSqlQuery query(queryStr);
    QSqlRecord record = query.record();
    int fieldCount = record.count();
    QStringList fields;
    for(int i = 0;i < fieldCount;i++){
        QString fieldName = record.fieldName(i);
        fields.append(fieldName);
    }
    return fields;
}
