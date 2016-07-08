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
    dateTime = QDateTime::fromString(datetimeStr, "yyyy-MM-dd h:mm:ss");

    do{
        QString datetimeSql = QString(" datetime >= '%1' and datetime <= '%2' and EXTRACT(SECOND from datetime) = 0 ")
                .arg(dateTime.toString("yyyy-MM-dd hh:mm:ss"))
                .arg(QString("%1:59:00").arg(dateTime.toString("yyyy-MM-dd hh")));
        datetimeSqlList.append(datetimeSql);
        dateTime = dateTime.addSecs(3600);
    }while(dateTime.secsTo(tdateTime) >= 0);
    return datetimeSqlList;
}

void RckqControl::run(){
    emit setProgressValue(1);
    int weatherCount = m_weatherList.size();
    int sumCount = 0;
    if(m_fhour > m_thour){
        sumCount = weatherCount * m_dateList.size() * (24 - m_fhour + 1 + m_thour);
    }else{
        sumCount = weatherCount * m_dateList.size() * (m_thour - m_fhour + 1);
    }
    int row = 0;
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
                    QString queryStr = QString("select * from %1_automaticwind where runwayno = '%2' and %3 order by datetime")
                            .arg(m_code)
                            .arg(m_runway)
                            .arg(datetimeSql);

                    //初始化列0~59的值
                    QList<QString> valueList;
                    for(int value_i = 0;value_i < 60;value_i++){
                        valueList.append("");
                    }
                    //初始化列日期
                    QString ymdhStr("");
                    //查询数据
                    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
                    int rowCount = plainModel->rowCount();
                    for(int row_i = 0;row_i < rowCount;row_i++){
                        QString dateTimeStr = plainModel->record(row_i).value(0).toString();
                        float windSpeed = plainModel->record(row_i).value(2).toFloat();

                        QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-ddThh:mm:ss");
                        int min = dateTime.toString("m").toInt();
                        valueList[min] = QString("%1").arg(windSpeed);

                        if(ymdhStr.compare("") == 0){
                            ymdhStr = dateTime.toString("yyyy年MM月dd日 hh时");
                        }
                    }
                    delete plainModel;

                    //通知显示信息
                    emit sendMessage(row, weatherStr, ymdhStr, valueList, dateStr);
                    row += 1;
                    emit setProgressValue((int)(((qreal)(row)/(qreal)sumCount) * 100));
                }
            }
        }else if(weatherStr.compare("侧风速") == 0){
            int dateCount = m_dateList.size();
            for(int j = 0;j < dateCount;j++){
                QString dateStr = m_dateList[j];
                QList<QString> datetimeSqlList = createDatetimeSql(dateStr, m_fhour, m_thour);
                int count = datetimeSqlList.size();
                for(int k = 0;k < count;k++){
                    QString datetimeSql = datetimeSqlList[k];
                    QString queryStr = QString("select * from %1_automaticwind where runwayno = '%2' and %3 order by datetime")
                            .arg(m_code)
                            .arg(m_runway)
                            .arg(datetimeSql);

                    //初始化列0~59的值
                    QList<QString> valueList;
                    for(int value_i = 0;value_i < 60;value_i++){
                        valueList.append("");
                    }
                    //初始化列日期
                    QString ymdhStr("");
                    //查询数据
                    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
                    int rowCount = plainModel->rowCount();
                    for(int row_i = 0;row_i < rowCount;row_i++){
                        QString dateTimeStr = plainModel->record(row_i).value(0).toString();
                        QString runwayStr = plainModel->record(row_i).value(1).toString();
                        float windSpeed = plainModel->record(row_i).value(2).toFloat();
                        int winddir = plainModel->record(row_i).value(3).toInt();

                        QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-ddThh:mm:ss");
                        int min = dateTime.toString("m").toInt();

                        runwayStr = runwayStr.replace(QRegExp("[A-Za-z]+"), "");
                        int runwayAngle = runwayStr.toInt() * 10;
                        windSpeed = qAbs(qSin((winddir - runwayAngle) * PI / 180) * windSpeed);
                        valueList[min] = QString("%1").arg(windSpeed);

                        if(ymdhStr.compare("") == 0){
                            ymdhStr = dateTime.toString("yyyy年MM月dd日 hh时");
                        }
                    }
                    delete plainModel;

                    //通知显示信息
                    emit sendMessage(row, weatherStr, ymdhStr, valueList, dateStr);
                    row += 1;
                    emit setProgressValue((int)(((qreal)(row)/(qreal)sumCount) * 100));
                }
            }
        }else if(weatherStr.compare("顺风速") == 0){
            int dateCount = m_dateList.size();
            for(int j = 0;j < dateCount;j++){
                QString dateStr = m_dateList[j];
                QList<QString> datetimeSqlList = createDatetimeSql(dateStr, m_fhour, m_thour);
                int count = datetimeSqlList.size();
                for(int k = 0;k < count;k++){
                    QString datetimeSql = datetimeSqlList[k];
                    QString queryStr = QString("select * from %1_automaticwind where runwayno = '%2' and %3 order by datetime")
                            .arg(m_code)
                            .arg(m_runway)
                            .arg(datetimeSql);

                    //初始化列0~59的值
                    QList<QString> valueList;
                    for(int value_i = 0;value_i < 60;value_i++){
                        valueList.append("");
                    }
                    //初始化列日期
                    QString ymdhStr("");
                    //查询数据
                    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
                    int rowCount = plainModel->rowCount();
                    for(int row_i = 0;row_i < rowCount;row_i++){
                        QString dateTimeStr = plainModel->record(row_i).value(0).toString();
                        QString runwayStr = plainModel->record(row_i).value(1).toString();
                        float windSpeed = plainModel->record(row_i).value(2).toFloat();
                        int winddir = plainModel->record(row_i).value(3).toInt();

                        QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-ddThh:mm:ss");
                        int min = dateTime.toString("m").toInt();

                        runwayStr = runwayStr.replace(QRegExp("[A-Za-z]+"), "");
                        int runwayAngle = runwayStr.toInt() * 10;
                        windSpeed = -(qCos((winddir - runwayAngle) * PI / 180) * windSpeed);
                        valueList[min] = QString("%1").arg(windSpeed);

                        if(ymdhStr.compare("") == 0){
                            ymdhStr = dateTime.toString("yyyy年MM月dd日 hh时");
                        }
                    }
                    delete plainModel;

                    //通知显示信息
                    emit sendMessage(row, weatherStr, ymdhStr, valueList, dateStr);
                    row += 1;
                    emit setProgressValue((int)(((qreal)(row)/(qreal)sumCount) * 100));
                }
            }
        }else if(weatherStr.compare("逆风速") == 0){
            int dateCount = m_dateList.size();
            for(int j = 0;j < dateCount;j++){
                QString dateStr = m_dateList[j];
                QList<QString> datetimeSqlList = createDatetimeSql(dateStr, m_fhour, m_thour);
                int count = datetimeSqlList.size();
                for(int k = 0;k < count;k++){
                    QString datetimeSql = datetimeSqlList[k];
                    QString queryStr = QString("select * from %1_automaticwind where runwayno = '%2' and %3 order by datetime")
                            .arg(m_code)
                            .arg(m_runway)
                            .arg(datetimeSql);

                    //初始化列0~59的值
                    QList<QString> valueList;
                    for(int value_i = 0;value_i < 60;value_i++){
                        valueList.append("");
                    }
                    //初始化列日期
                    QString ymdhStr("");
                    //查询数据
                    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
                    int rowCount = plainModel->rowCount();
                    for(int row_i = 0;row_i < rowCount;row_i++){
                        QString dateTimeStr = plainModel->record(row_i).value(0).toString();
                        QString runwayStr = plainModel->record(row_i).value(1).toString();
                        float windSpeed = plainModel->record(row_i).value(2).toFloat();
                        int winddir = plainModel->record(row_i).value(3).toInt();

                        QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-ddThh:mm:ss");
                        int min = dateTime.toString("m").toInt();

                        runwayStr = runwayStr.replace(QRegExp("[A-Za-z]+"), "");
                        int runwayAngle = runwayStr.toInt() * 10;
                        windSpeed = qCos((winddir - runwayAngle) * PI / 180) * windSpeed;
                        valueList[min] = QString("%1").arg(windSpeed);

                        if(ymdhStr.compare("") == 0){
                            ymdhStr = dateTime.toString("yyyy年MM月dd日 hh时");
                        }
                    }
                    delete plainModel;

                    //通知显示信息
                    emit sendMessage(row, weatherStr, ymdhStr, valueList, dateStr);
                    row += 1;
                    emit setProgressValue((int)(((qreal)(row)/(qreal)sumCount) * 100));
                }
            }
        }else if(weatherStr.compare("气温") == 0){
            int dateCount = m_dateList.size();
            for(int j = 0;j < dateCount;j++){
                QString dateStr = m_dateList[j];
                QList<QString> datetimeSqlList = createDatetimeSql(dateStr, m_fhour, m_thour);
                int count = datetimeSqlList.size();
                for(int k = 0;k < count;k++){
                    QString datetimeSql = datetimeSqlList[k];
                    QString queryStr = QString("select * from %1_automatictemperature where runwayno = '%2' and %3 order by datetime")
                            .arg(m_code)
                            .arg(m_runway)
                            .arg(datetimeSql);

                    //初始化列0~59的值
                    QList<QString> valueList;
                    for(int value_i = 0;value_i < 60;value_i++){
                        valueList.append("");
                    }
                    //初始化列日期
                    QString ymdhStr("");
                    //查询数据
                    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
                    int rowCount = plainModel->rowCount();
                    for(int row_i = 0;row_i < rowCount;row_i++){
                        QString dateTimeStr = plainModel->record(row_i).value(0).toString();
                        float temperature = plainModel->record(row_i).value(3).toFloat();

                        QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-ddThh:mm:ss");
                        int min = dateTime.toString("m").toInt();
                        valueList[min] = QString("%1").arg(temperature);

                        if(ymdhStr.compare("") == 0){
                            ymdhStr = dateTime.toString("yyyy年MM月dd日 hh时");
                        }
                    }
                    delete plainModel;

                    //通知显示信息
                    emit sendMessage(row, weatherStr, ymdhStr, valueList, dateStr);
                    row += 1;
                    emit setProgressValue((int)(((qreal)(row)/(qreal)sumCount) * 100));
                }
            }
        }else if(weatherStr.compare("比湿") == 0){
            int dateCount = m_dateList.size();
            for(int j = 0;j < dateCount;j++){
                QString dateStr = m_dateList[j];
                QList<QString> datetimeSqlList = createDatetimeSql(dateStr, m_fhour, m_thour);
                int count = datetimeSqlList.size();
                for(int k = 0;k < count;k++){
                    QString datetimeSql = datetimeSqlList[k];
                    QString queryStr = QString("select * from %1_automatictemperature where runwayno = '%2' and %3 order by datetime")
                            .arg(m_code)
                            .arg(m_runway)
                            .arg(datetimeSql);

                    //初始化列0~59的值
                    QList<QString> valueList;
                    for(int value_i = 0;value_i < 60;value_i++){
                        valueList.append("");
                    }
                    //初始化列日期
                    QString ymdhStr("");
                    //查询数据
                    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
                    int rowCount = plainModel->rowCount();
                    for(int row_i = 0;row_i < rowCount;row_i++){
                        QString dateTimeStr = plainModel->record(row_i).value(0).toString();
                        float pressure = plainModel->record(row_i).value(1).toFloat();
                        float humidity = plainModel->record(row_i).value(2).toFloat();
                        float temperature = plainModel->record(row_i).value(3).toFloat();

                        QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-ddThh:mm:ss");
                        int min = dateTime.toString("m").toInt();
                        if(temperature >= -15){
                            double a = 17.269;
                            double b = 35.86;
                            double es0 = 6.1078;
                            double es = qPow(es0, a * temperature / (273.15 + temperature - b));
                            double e = es * humidity;
                            double q = 0.622 * e / (pressure - 0.378 * e) * 1000;
                            valueList[min] = QString("%1").arg(q);
                        }else if(temperature < -40){
                            double a = 21.8746;
                            double b = 7.66;
                            double es0 = 6.1078;
                            double es = qPow(es0, a * temperature / (273.15 + temperature - b));
                            double e = es * humidity;
                            double q = 0.622 * e / (pressure - 0.378 * e) * 1000;
                            valueList[min] = QString("%1").arg(q);
                        }else{
                            double a_water = 17.269;
                            double b_water = 35.86;
                            double es0_water = 6.1078;
                            double es_water = qPow(es0_water, a_water * temperature / (273.15 + temperature - b_water));
                            double a_ice = 21.8746;
                            double b_ice = 7.66;
                            double es0_ice = 6.1078;
                            double es_ice = qPow(es0_ice, a_ice * temperature / (273.15 + temperature - b_ice));
                            double es = 0.002 * ((80 + 2 * temperature) * es_water - (30 + 2 * temperature) * es_ice);
                            double e = es * humidity;
                            double q = 0.622 * e / (pressure - 0.378 * e) * 1000;
                            valueList[min] = QString("%1").arg(q);
                        }

                        if(ymdhStr.compare("") == 0){
                            ymdhStr = dateTime.toString("yyyy年MM月dd日 hh时");
                        }
                    }
                    delete plainModel;

                    //通知显示信息
                    emit sendMessage(row, weatherStr, ymdhStr, valueList, dateStr);
                    row += 1;
                    emit setProgressValue((int)(((qreal)(row)/(qreal)sumCount) * 100));
                }
            }
        }
    }

    emit execute(true);
}
