#include "sjdrcontrol.h"

SjdrElement::SjdrElement(){

}

SjdrElement::SjdrElement(QualityControlSource qualityControlSource, QFileInfo fileInfo){
    this->m_qualityControlSource = qualityControlSource;
    this->m_fileInfo = fileInfo;
}

SjdrElement::~SjdrElement(){

}

SjdrControl::SjdrControl(QObject *parent)
    :QThread(parent)
    ,m_loop(true)
{

}

void SjdrControl::addTask(SjdrElement sjdrElement){
    m_tasks.push_back(sjdrElement);
}

void SjdrControl::setAirport(const Airport &airport)
{
    m_airport = airport;
}

void SjdrControl::setLoop(bool loop){
    m_loop = loop;
}

void SjdrControl::run(){

    while(m_loop){
        if(m_tasks.count() > 0){
            SjdrElement sjdrEle = m_tasks.takeAt(0);
            if(SUMMARY == sjdrEle.m_qualityControlSource.id()){
                controlSummary(sjdrEle);
            }else if(EXTREMUM == sjdrEle.m_qualityControlSource.id()){
                controlExtremum(sjdrEle);
            }else if(MAINLAND_AUTOMATICWIND == sjdrEle.m_qualityControlSource.id()){
                controlMainlandAutomaticWind(sjdrEle);
            }else if(MACAO_AUTOMATICWIND == sjdrEle.m_qualityControlSource.id()){
                controlMacaoAutomaticWind(sjdrEle);
            }else if(MAINLAND_AUTOMATICTEMPERATURE == sjdrEle.m_qualityControlSource.id()){
                controlMainlandAutomaticTemperature(sjdrEle);
            }else{
                controlUnknown(sjdrEle);
            }

            QStringList messages;
            messages.append(sjdrEle.m_fileInfo.fileName());
            messages.append("success!");
            messages.append(sjdrEle.m_qualityControlSource.name());
            messages.append(sjdrEle.m_fileInfo.absoluteFilePath());
            emit sendMessage(messages);
        }
    }
}

void SjdrControl::controlSummary(const SjdrElement &sjdrElement){
    QFileInfo fileInfo = sjdrElement.m_fileInfo;
    QualityControlSource qualityControlSource = sjdrElement.m_qualityControlSource;

    AsDataBase asdb(fileInfo.absoluteFilePath());

    QStringList tables = asdb.queryTables();
    int tableCount = tables.count();
    for(int i = 0;i < tableCount;i++){
        QString title = asdb.queryFields(tables[i]).join(",");
        if(title.compare(qualityControlSource.fields()) == 0){
            QString queryStr = QString("select %1 from %2").arg(qualityControlSource.fields()).arg(tables[i]);
            QSqlQueryModel *plainModel = asdb.queryModel(queryStr);
            int rowCount = plainModel->rowCount();
            for(int j = 0;j < rowCount;j++){
                QString result = QString("%1|%2|%3|%4")
                        .arg(plainModel->record(j).value(0).toString())
                        .arg(plainModel->record(j).value(1).toString())
                        .arg(plainModel->record(j).value(2).toString())
                        .arg(plainModel->record(j).value(3).toString());
                qDebug() << result;
            }
            delete plainModel;
            break;
        }
    }
}

void SjdrControl::controlExtremum(const SjdrElement &sjdrElement){

}

void SjdrControl::controlMainlandAutomaticWind(const SjdrElement &sjdrElement){

}

void SjdrControl::controlMacaoAutomaticWind(const SjdrElement &sjdrElement){

}

void SjdrControl::controlMainlandAutomaticTemperature(const SjdrElement &sjdrElement){

}

void SjdrControl::controlUnknown(const SjdrElement &sjdrElement){

}
