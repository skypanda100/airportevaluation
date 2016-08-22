#include "sharedmemory.h"

SharedMemory* SharedMemory::sm = NULL;
bool SharedMemory::isWelcome = true;
QMutex SharedMemory::mutex;

/**
 * @brief SharedMemory::SharedMemory
 */
SharedMemory::SharedMemory(){
    pgDb = new PgDataBase;
}

/**
 * @brief SharedMemory::getInstance
 * @return
 */
SharedMemory *SharedMemory::getInstance(){
    if(!sm){
        QMutexLocker locker(&mutex);
        if(!sm){
            sm = new SharedMemory;
        }
    }

    return sm;
}

/**
 * @brief SharedMemory::queryAirportInfomation
 */
void SharedMemory::queryAirportInfomation(){
    m_aiportList.clear();
    m_runwayHash.clear();
    m_windHash.clear();

    this->queryAirport();

    emit airportInfoChanged(m_aiportList, m_runwayHash);
}

/**
 * @brief SharedMemory::getAirportList
 * @return
 */
QList<Airport> SharedMemory::getAirportList() const {
    return this->m_aiportList;
}

/**
 * @brief SharedMemory::getRunwayHash
 * @return
 */
QHash< QString, QList<QString> > SharedMemory::getRunwayHash() const {
    return this->m_runwayHash;
}

/**
 * @brief SharedMemory::getWindHash
 * @return
 */
QHash< QString, QList<QString> > SharedMemory::getWindHash() const {
    return this->m_windHash;
}

/**
 * @brief SharedMemory::queryAirport
 */
void SharedMemory::queryAirport(){
    QString queryStr = QString("select * from airport");
    QSqlQueryModel *plainModel = pgDb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        Airport airport;
        airport.setCode(plainModel->record(i).value(0).toString());
        airport.setName(plainModel->record(i).value(1).toString());
        airport.setPlaneName(plainModel->record(i).value(2).toString());
        airport.setLongitude(plainModel->record(i).value(3).toFloat());
        airport.setLatitude(plainModel->record(i).value(4).toFloat());
        airport.setAltitude(plainModel->record(i).value(5).toFloat());
        airport.setDirection(plainModel->record(i).value(6).toFloat());
        airport.setType(plainModel->record(i).value(7).toString());
        m_aiportList.append(airport);

        //查找跑道
        QList<QString> runwayList = queryRunway(airport.code().toLower());
        m_runwayHash[airport.name()] = runwayList;
        //查找自动站风
        for(QString runwayStr : runwayList){
            QList<QString> windList = queryWind(airport.code().toLower(), runwayStr);
            m_windHash[QString("%1%2").arg(airport.code().toLower()).arg(runwayStr)] = windList;
        }
    }
    delete plainModel;
}

/**
 * @brief SharedMemory::queryRunway
 * @param codeStr
 * @return
 */
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

/**
 * @brief SharedMemory::queryWind
 * @param codeStr
 * @param runwayStr
 * @return
 */
QList<QString> SharedMemory::queryWind(QString codeStr, QString runwayStr){
    QList<QString> dateList;
    //查找自动站风表
    QString queryStr = QString("select distinct to_char(datetime, 'yyyy') as year from %1_automaticwind where runwayno = '%2' order by year desc")
            .arg(codeStr)
            .arg(runwayStr);
    QList<QVariant> resList = pgDb->queryVariant(queryStr);
    int resCount = resList.size();
    for(int i = 0;i < resCount;i++){
        dateList.append(resList[i].toString());
    }
    return dateList;
}
