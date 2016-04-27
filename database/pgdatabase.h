#ifndef PGDATABASE
#define PGDATABASE
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtWidgets>

class PgDataBase{

public:
    PgDataBase();
    ~PgDataBase();

    void openDb();
    void closeDb();

    QSqlQueryModel* queryModel(const QString &);
    QList<QVariant> queryVariant(const QString &);
    bool save(QString, QList<QVariant>);

private:
    QSqlDatabase db;
    QString server;
    QString port;
    QString uid;
    QString pwd;
    QString database;
};

#endif // PGDATABASE

