#include "fmgcontrol.h"
#include <QDebug>

FmgControl::FmgControl(QObject *parent)
    :QThread(parent)
{
    pgdb = new PgDataBase;

    titleList << "年"
              << "月";
    for(int i = 1;i <= 360;i++){
        titleList.append(QString("%1°").arg(i));
        selectList.append(QString("sum(case winddirection when %1 then 1 end)").arg(i));
    }
}

FmgControl::~FmgControl(){
    delete pgdb;
}

void FmgControl::setData(QString code, QString runway, QString fspeed, QString tspeed, QList<QString> years){
    this->m_code = code;
    this->m_runway = runway;
    this->m_fspeed = fspeed;
    this->m_tspeed = tspeed;
    this->m_years = years;
    if(fspeed.isEmpty() && tspeed.isEmpty()){
        queryStr = QString("select %1 from %2_automaticwind where runwayno = '%3' ")
                .arg(selectList.join(","))
                .arg(code.toLower())
                .arg(runway);
    }else if(!fspeed.isEmpty() && tspeed.isEmpty()){
        queryStr = QString("select %1 from %2_automaticwind where runwayno = '%3' and windspeed >= %4 ")
                .arg(selectList.join(","))
                .arg(code.toLower())
                .arg(runway)
                .arg(fspeed);
    }else if(fspeed.isEmpty() && !tspeed.isEmpty()){
        queryStr = QString("select %1 from %2_automaticwind where runwayno = '%3' and windspeed <= %4 ")
                .arg(selectList.join(","))
                .arg(code.toLower())
                .arg(runway)
                .arg(tspeed);
    }else if(!fspeed.isEmpty() && !tspeed.isEmpty()){
        queryStr = QString("select %1 from %2_automaticwind where runwayno = '%3' and windspeed >= %4 and windspeed <= %5 ")
                .arg(selectList.join(","))
                .arg(code.toLower())
                .arg(runway)
                .arg(fspeed)
                .arg(tspeed);
    }
}

void FmgControl::run(){
    emit setProgressValue(1);
    int yearCount = m_years.size();
    int totalProgress = yearCount * 12;
    int currentProgress = 0;
    for(int i = 0;i < yearCount;i++){
        QString yearStr = m_years[i];
        emit sendMessage(i * 12, yearStr);
        for(int j = 0;j < 12;j++){
            currentProgress += 1;
            QString dateWhereStr = QString("and (extract(year from datetime) * 12 + extract(month from datetime) - %1) = 0")
                    .arg(yearStr.toInt() * 12 + j + 1);
            QString tempQueryStr = QString("%1 %2").arg(queryStr).arg(dateWhereStr);

            QList<QString> windCountArg;
            QSqlQueryModel *plainModel = pgdb->queryModel(tempQueryStr);
            int rowCount = plainModel->rowCount();
            if(rowCount > 0){
                for(int k = 0;k < 360;k++){
                    windCountArg.append(plainModel->record(0).value(k).toString());
                }
            }
            delete plainModel;
            emit sendMessage(i * 12 + j, yearStr, windCountArg);
            emit setProgressValue((int)(((qreal)(currentProgress)/(qreal)totalProgress) * 100));
        }
    }
    emit execute(true);
}

