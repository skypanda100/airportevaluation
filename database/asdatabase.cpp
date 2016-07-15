#include "database/asdatabase.h"

AsDataBase::AsDataBase(QString filePath, QString con)
    :current_con(con)
{
    this->openDb(filePath, con);
}

AsDataBase::~AsDataBase(){
    this->closeDb();
}

void AsDataBase::openDb(QString filePath, QString con){
    QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=%1").arg(filePath);
    QSqlDatabase db;
    if(QSqlDatabase::contains(con)){
      db = QSqlDatabase::database(con);
    }else{
      db = QSqlDatabase::addDatabase("QODBC", con);
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
    QSqlDatabase::removeDatabase(current_con);
}

QSqlQueryModel* AsDataBase::queryModel(const QString &queryStr){
    QSqlDatabase db = QSqlDatabase::database(current_con);
    QSqlQueryModel *plainModel = new QSqlQueryModel;
    plainModel->setQuery(queryStr, db);
    while(plainModel->canFetchMore()){
        plainModel->fetchMore();
    }
    return plainModel;
}

QList<QVariant> AsDataBase::queryVariant(const QString &queryStr){
    QSqlDatabase db = QSqlDatabase::database(current_con);
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
    QSqlDatabase db = QSqlDatabase::database(current_con);
    QStringList tableList = db.tables();
    return tableList;
}

QStringList AsDataBase::queryFields(QString tableStr){
    QSqlDatabase db = QSqlDatabase::database(current_con);
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
