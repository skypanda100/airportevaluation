#include "rckqcontrol.h"

RckqControl::RckqControl(QObject *parent)
    :QThread(parent)
{
    pgdb = new PgDataBase;
}

RckqControl::~RckqControl(){
    delete pgdb;
}

void RckqControl::setData(QString code, QString runway, int type, int fhour, int thour, QList<QString> dateList, QList<QString> weatherList){
    this->m_code = code;
    this->m_runway = runway;
    this->m_type = type;
    this->m_fhour = fhour;
    this->m_thour = thour;
    this->m_dateList = dateList;
    this->m_weatherList = weatherList;
}

QList<QString> RckqControl::createDatetimeSql(QString dateStr, int fhour, int thour){
    QDateTime dateTime = QDateTime::fromString(dateStr, "yyyy-MM-dd");
    QDateTime tdateTime = QDateTime::fromString(QString("%1 %2:00:00").arg(dateStr).arg(thour), "yyyy-MM-dd h:mm:ss");
    QList<QString> datetimeSqlList;
    if(fhour > thour){
        dateTime = dateTime.addDays(-1);
    }
    QString datetimeStr = QString("%1 %2:00:00")
            .arg(dateTime.toString("yyyy-MM-dd"))
            .arg(fhour);
    dateTime = QDateTime::fromString(datetimeStr, "yyyy-MM-dd hh:mm:ss");

    do{
        QString datetimeSql = QString(" datetime >= '%1' and datetime <= '%2' and EXTRACT(SECOND from datetime) = 0 ")
                .arg(dateTime.toString("yyyy-MM-dd hh:mm:ss"))
                .arg(QString("%1:59:00").arg(dateTime.toString("yyyy-MM-dd hh")));
        datetimeSqlList.append(datetimeSql);
        dateTime = dateTime.addSecs(3600);
    }while(tdateTime.secsTo(dateTime) <= 0);
    return datetimeSqlList;
}

void RckqControl::run(){
    int weatherCount = m_weatherList.size();
    for(int i = 0;i < weatherCount;i++){
        QString weatherStr = m_weatherList[i];
        if(weatherStr.compare("全风速") == 0){
            int dateCount = m_dateList.size();
            for(int j = 0;j < dateCount;j++){
                QString dateStr = m_dateList[j];
                QList<QString> datetimeSqlList = createDatetimeSql(dateStr, m_fhour, m_thour);
                int count = datetimeSqlList.size();
                for(int k = 0;k < count;k++){
                    QString datetimeSql = datetimeSqlList[k];
                }
            }
        }else if(weatherStr.compare("侧风速") == 0){

        }else if(weatherStr.compare("顺风速") == 0){

        }else if(weatherStr.compare("逆风速") == 0){

        }else if(weatherStr.compare("气温") == 0){

        }else if(weatherStr.compare("比湿") == 0){

        }
    }
}
