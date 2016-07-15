#ifndef ASDATABASE
#define ASDATABASE
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtWidgets>

class AsDataBase{

public:
    AsDataBase(QString, QString);
    ~AsDataBase();

    void openDb(QString, QString);
    void closeDb();

    QSqlQueryModel* queryModel(const QString &);
    QList<QVariant> queryVariant(const QString &);
    QStringList queryTables();
    QStringList queryFields(QString);

private:
    QString current_con;
};

#endif // ASDATABASE

