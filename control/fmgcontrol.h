#ifndef FMGCONTROL
#define FMGCONTROL

#include <QThread>
#include "database/pgdatabase.h"

class FmgControl : public QThread{
    Q_OBJECT

public:
    explicit FmgControl(QObject *parent = 0);
    ~FmgControl();

    void run() Q_DECL_OVERRIDE;

    void setData(QString code, QString runway, QString fspeed, QString tspeed, QList<QString> years);

signals:
    void sendMessage(int row, QString year);
    void sendMessage(int row, QString year, QList<QString> windCountList);
    void execute(bool isEnd);
    void singleExecute(bool isEnd);
    void setProgressValue(int,int);

private:
    QStringList selectList;//最关键的查询语句（将稍复杂的业务逻辑转化为sql语句以减小程序内存的压力：以牺牲速度换取性能的稳定）
    QString queryStr;//完整的查询语句
    QString m_code;
    QString m_runway;
    QString m_fspeed;
    QString m_tspeed;
    QList<QString> m_years;

    QList<QString> titleList;
    PgDataBase *pgdb;

};

#endif // FMGCONTROL

