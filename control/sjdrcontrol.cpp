#include "sjdrcontrol.h"

SjdrElement::SjdrElement(){

}

SjdrElement::SjdrElement(SourceType sourceType, QString type, QFileInfo fileInfo){
    this->m_sourceType = sourceType;
    this->m_type = type;
    this->m_fileInfo = fileInfo;
}

SjdrControl::SjdrControl(QObject *parent)
    :QThread(parent)
{

}

void SjdrControl::addTask(SjdrElement sjdrElement){
    m_tasks.push_back(sjdrElement);
}

void SjdrControl::setAirport(const Airport &airport)
{
    m_airport = airport;
}

void SjdrControl::run(){

    while(true){
        if(m_tasks.count() > 0){
            SjdrElement sjdrEle = m_tasks.takeAt(0);
            if(SUMMARY == sjdrEle.m_sourceType){

            }else if(EXTREMUM == sjdrEle.m_sourceType){

            }else if(MAINLAND_AUTOMATICWIND == sjdrEle.m_sourceType){

            }else if(MACAO_AUTOMATICWIND == sjdrEle.m_sourceType){

            }else if(MAINLAND_AUTOMATICTEMPERATURE == sjdrEle.m_sourceType){

            }else{

            }

            QStringList messages;
            messages.append(sjdrEle.m_fileInfo.fileName());
            messages.append("success!");
            messages.append(sjdrEle.m_type);
            messages.append(sjdrEle.m_fileInfo.absoluteFilePath());
            emit sendMessage(messages);
        }
    }
}
