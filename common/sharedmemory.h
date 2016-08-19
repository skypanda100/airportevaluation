#ifndef SHAREDMEMORY
#define SHAREDMEMORY

#include <QObject>
#include <QMutex>
#include "../bean/airport.h"
#include "../database/pgdatabase.h"

class SharedMemory : public QObject{
    Q_OBJECT

public:
    static SharedMemory *getInstance();
    void queryAirportInfomation();
    QList<Airport> getAirportList() const;
    QHash< QString, QList<QString> > getRunwayHash() const;
    QHash< QString, QList<QString> > getWindHash() const;

signals:
    void airportInfoChanged(QList<Airport>, QHash< QString, QList<QString> >);

public:
    static bool isWelcome;

private:
    SharedMemory();
    void queryAirport();
    QList<QString> queryRunway(QString);
    QList<QString> queryWind(QString, QString);

private:
    //互斥锁
    static QMutex mutex;
    //本对象
    static SharedMemory* sm;

    //数据库
    PgDataBase *pgDb;
    //机场列表
    QList<Airport> m_aiportList;
    //跑道hash(key:机场code value:跑道列表)
    QHash< QString, QList<QString> > m_runwayHash;
    //自动站风hash(key:机场code+跑道 value:年列表)
    QHash< QString, QList<QString> > m_windHash;

};

#endif // SHAREDMEMORY

