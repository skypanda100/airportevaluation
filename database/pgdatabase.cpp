#include "database/pgdatabase.h"

PgDataBase::PgDataBase()
    :server("127.0.0.1")
    ,port("5432")
    ,uid("postgres")
    ,pwd("123456")
    ,database("postgres")
{
    this->openDb();
}

PgDataBase::~PgDataBase(){
    this->closeDb();
}

void PgDataBase::openDb(){
    db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString("Driver={PostgreSQL ODBC Driver(ANSI)};server=%1;port=%2;uid=%3;pwd=%4;database=%5")
            .arg(server)
            .arg(port)
            .arg(uid)
            .arg(pwd)
            .arg(database);
    db.setDatabaseName(dsn);
    if(!db.open()) {
        QMessageBox::critical(0, QObject::tr("错误提示"),
                              db.lastError().text());
    }
}

void PgDataBase::closeDb(){
    db.close();
}

QSqlQueryModel* PgDataBase::queryModel(const QString &queryStr){
    QSqlQueryModel *plainModel = new QSqlQueryModel;
    plainModel->setQuery(queryStr);
    return plainModel;
}

QList<QVariant> PgDataBase::queryVariant(const QString &queryStr){
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
