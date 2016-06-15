#include "sharedmemory.h"

SharedMemory* SharedMemory::sm = NULL;
QMutex SharedMemory::mutex;

SharedMemory::SharedMemory(){
    pgDb = new PgDataBase;
}

SharedMemory *SharedMemory::getInstance(){
    if(!sm){
        QMutexLocker locker(&mutex);
        if(!sm){
            sm = new SharedMemory;
        }
    }

    return sm;
}

void SharedMemory::queryAirportInfomation(){
    m_aiportList.clear();
    m_runwayHash.clear();

    this->queryAirport();

    emit airportInfoChanged(m_aiportList, m_runwayHash);
}

QList<Airport> SharedMemory::getAirportList() const {
    return this->m_aiportList;
}

QHash< QString, QList<QString> > SharedMemory::getRunwayHash() const {
    return this->m_runwayHash;
}

void SharedMemory::queryAirport(){
    QString queryStr = QString("select * from airport");
    QSqlQueryModel *plainModel = pgDb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        Airport airport;
        airport.setCode(plainModel->record(i).value(0).toString());
        airport.setName(plainModel->record(i).value(1).toString());
        airport.setLongitude(plainModel->record(i).value(2).toFloat());
        airport.setLatitude(plainModel->record(i).value(3).toFloat());
        airport.setAltitude(plainModel->record(i).value(4).toFloat());
        airport.setDirection(plainModel->record(i).value(5).toFloat());
        airport.setType(plainModel->record(i).value(6).toString());
        m_aiportList.append(airport);

        //查找跑道
        QList<QString> runwayList = queryRunway(airport.code().toLower());
        m_runwayHash[airport.name()] = runwayList;
    }
    delete plainModel;
}

QList<QString> SharedMemory::queryRunway(QString codeStr){
    QList<QString> runwayList;
    //查找自动站风表
    QString queryStr = QString("select distinct runwayno from %1_automaticwind").arg(codeStr);
    QList<QVariant> resList = pgDb->queryVariant(queryStr);
    int resCount = resList.size();
    for(int i = 0;i < resCount;i++){
        runwayList.append(resList[i].toString());
    }
    //查找温度气压表
    queryStr = QString("select distinct runwayno from %1_automatictemperature").arg(codeStr);
    resList = pgDb->queryVariant(queryStr);
    resCount = resList.size();
    for(int i = 0;i < resCount;i++){
        if(!runwayList.contains(resList[i].toString())){
            runwayList.append(resList[i].toString());
        }
    }
    return runwayList;
//    QList<QString> runwayList;
//    QString queryStr = QString("select distinct runwayno from %1_automaticwind").arg(codeStr);
//    QList<QVariant> resList = pgDb->queryVariant(queryStr);
//    int resCount = resList.size();
//    for(int i = 0;i < resCount;i++){
//        runwayList.append(resList[i].toString());
//    }
//    return runwayList;
}
