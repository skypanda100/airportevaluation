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
    ,m_execute(false)
    ,pgdb(NULL)
{

    //数据库
    pgdb = new PgDataBase;
    //月总簿表需要导入的字段
    monthsummary_fields << "日期时间"
                        << "十分钟平均风向"
                        << "十分钟平均风速"
                        << "阵风"
                        << "主导能见度"
                        << "跑道1方向编号"
                        << "跑道1方向跑道视程"
                        << "跑道2方向编号"
                        << "跑道2方向跑道视程"
                        << "跑道3方向编号"
                        << "跑道3方向跑道视程"
                        << "跑道4方向编号"
                        << "跑道4方向跑道视程"
                        << "跑道5方向编号"
                        << "跑道5方向跑道视程"
                        << "天气现象"
                        << "总云量"
                        << "低云量"
                        << "低云况1"
                        << "低云况2"
                        << "低云况3"
                        << "低云况4"
                        << "低云况5"
                        << "中云况1"
                        << "中云况2"
                        << "中云况3"
                        << "高云况1"
                        << "高云况2"
                        << "高云况3"
                        << "气温"
                        << "相对湿度"
                        << "露点温度"
                        << "场面气压"
                        << "修正海压";
    //极值表需要导入的字段
    extremum_fields << "日期"
                    << "日最高温度"
                    << "日最低温度"
                    << "日降水量"
                    << "日最大积雪深度"
                    << "演变1"
                    << "演变2"
                    << "演变3"
                    << "演变4"
                    << "演变5"
                    << "演变6"
                    << "演变7"
                    << "演变8"
                    << "演变9"
                    << "演变10";

    //自动站风表需要导入的字段（大陆）
    mainland_automaticwind_fields << "CREATEDATE"
                                  << "SITE"
                                  << "WS2A (MPS)"
                                  << "WD2A";

    //自动站风表需要导入的字段（澳门）
    macao_automaticwind_fields << "CREATEDATE"
                               << "SITE"
                               << "WSINS (KT)"
                               << "WDINS";

    //自动站温度气压表需要导入的字段(将源文件中的【QFE 27 (HPA)】的数字去除掉再比较)
    automatictemperature_fields << "CREATEDATE"
                                << "SITE"
                                << "QFE  (HPA)"
                                << "TEMP (癈)"
                                << "RH (%)"
                                << "DEWPOINT (癈)";

    //月总簿要素表插入语句
    insertMonthsummary = QString("insert into %1_monthsummary values(?, ?, ?, ?, ?,"
                                 "?, ?, ?, ?, ?, ?, "
                                 "?, ?, ?, ?, ?, ?, "
                                 "?, ?, ?, ?, "
                                 "?, ?, ?, ?, "
                                 "?, ?, ?, ?, "
                                 "?, ?, ?, ?, ?)");

    //极值表插入语句
    insertExtremum = QString("insert into %1_extremum values(?, ?, ?, ?, ?, "
                             "?, ?, ?, ?, ?, ?, "
                             "?, ?, ?, ?)");

    //自动站风表插入语句（大陆）
    insertMainlandAutomaticWind = QString("insert into %1_automaticwind values(?, ?, ?, ?)");

    //自动站风表插入语句（澳门）
    insertMacaoAutomaticWind = QString("insert into %1_automaticwind values(?, ?, ?, ?)");

    //自动站温度气压表插入语句
    insertAutomaticTeperature = QString("insert into %1_automatictemperature values(?, ?, ?, ?, ?, ?)");
}

SjdrControl::~SjdrControl(){
    delete pgdb;
}

void SjdrControl::addTask(SjdrElement sjdrElement){
    m_tasks.push_back(sjdrElement);
}

void SjdrControl::clearTask(){
    m_tasks.clear();
    m_execute = false;
    emit execute(true);
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
            m_execute = true;
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
            if(m_execute){
                emit singleExecute(true);
            }
        }else{
            if(m_execute){
                m_execute = false;
                emit execute(true);
            }
        }
        this->currentThread()->msleep(10);
    }
}

/**
 * @brief SjdrControl::message
 * @param info
 * @param sjdrElement
 * @param code
 */
void SjdrControl::message(const QString &info, const SjdrElement &sjdrElement, ReturnCode code){
    QStringList messages;
    if(MSG_SUCCESS == code){
        messages.append(QString(":/images/success.pngzdt%1").arg(sjdrElement.m_fileInfo.fileName()));
    }else if(MSG_ERROR == code){
        messages.append(QString(":/images/error.pngzdt%1").arg(sjdrElement.m_fileInfo.fileName()));
    }else if(MSG_WARN == code){
        messages.append(QString(":/images/warning.pngzdt%1").arg(sjdrElement.m_fileInfo.fileName()));
    }else{
        messages.append(QString(":/images/unknown.pngzdt%1").arg(sjdrElement.m_fileInfo.fileName()));
    }
    messages.append(info);
    messages.append(sjdrElement.m_qualityControlSource.name());
    messages.append(sjdrElement.m_fileInfo.absoluteFilePath());
    emit sendMessage(messages);
}

/**
 * @brief SjdrControl::controlSummary
 * @param sjdrElement
 * 月总簿要素
 */
void SjdrControl::controlSummary(const SjdrElement &sjdrElement){
    QFileInfo fileInfo = sjdrElement.m_fileInfo;
    QualityControlSource qualityControlSource = sjdrElement.m_qualityControlSource;

    AsDataBase asdb(fileInfo.absoluteFilePath());

    QStringList tables = asdb.queryTables();
    int tableCount = tables.count();
    for(int i = 0;i < tableCount;i++){
        QStringList titleList = asdb.queryFields(tables[i]);
        QString title = titleList.join(",");
        if(title.indexOf(qualityControlSource.fields()) == 0){
            QString queryStr = QString("select %1 from %2 order by 日期时间 asc").arg(qualityControlSource.fields()).arg(tables[i]);
            QSqlQueryModel *plainModel = asdb.queryModel(queryStr);
            int rowCount = plainModel->rowCount();
            bool isAllDone = true;
            for(int j = 0;j < rowCount;j++){
                if(!m_execute){
                    return;
                }
                int fieldCount = monthsummary_fields.size();
                QList<QVariant> values;
                for(int k = 0;k < fieldCount;k++){
                    int valueIndex = titleList.indexOf(monthsummary_fields[k]);
                    if(valueIndex < 0){
                        isAllDone = false;
                        message(QString("未找到字段[%1]").arg(monthsummary_fields[k]), sjdrElement, MSG_ERROR);
                        break;
                    }else{
                        values.append(plainModel->record(j).value(valueIndex));
                    }
                }
                if(values.size() == fieldCount){
                    bool ret = pgdb->save(insertMonthsummary.arg(m_airport.code()), values);
                    if(!ret){
                        isAllDone = false;
                        message(QString("第%1行数据保存失败").arg(j + 1), sjdrElement, MSG_ERROR);
                    }
                }
            }
            if(isAllDone){
                message(QString("所有数据保存成功"), sjdrElement, MSG_SUCCESS);
            }
            delete plainModel;
            break;
        }
    }
}

/**
 * @brief SjdrControl::controlExtremum
 * @param sjdrElement
 * 极值
 */
void SjdrControl::controlExtremum(const SjdrElement &sjdrElement){
    QFileInfo fileInfo = sjdrElement.m_fileInfo;
    QualityControlSource qualityControlSource = sjdrElement.m_qualityControlSource;

    AsDataBase asdb(fileInfo.absoluteFilePath());

    QStringList tables = asdb.queryTables();
    int tableCount = tables.count();
    for(int i = 0;i < tableCount;i++){
        QStringList titleList = asdb.queryFields(tables[i]);
        QString title = titleList.join(",");
        if(title.indexOf(qualityControlSource.fields()) == 0){
            QString queryStr = QString("select %1 from %2 order by 日期 asc").arg(qualityControlSource.fields()).arg(tables[i]);
            QSqlQueryModel *plainModel = asdb.queryModel(queryStr);
            int rowCount = plainModel->rowCount();
            bool isAllDone = true;
            for(int j = 0;j < rowCount;j++){
                if(!m_execute){
                    return;
                }
                int fieldCount = extremum_fields.size();
                QList<QVariant> values;
                for(int k = 0;k < fieldCount;k++){
                    int valueIndex = titleList.indexOf(extremum_fields[k]);
                    if(valueIndex < 0){
                        isAllDone = false;
                        message(QString("未找到字段[%1]").arg(extremum_fields[k]), sjdrElement, MSG_ERROR);
                        break;
                    }else{
                        values.append(plainModel->record(j).value(valueIndex));
                    }
                }
                if(values.size() == fieldCount){
                    bool ret = pgdb->save(insertExtremum.arg(m_airport.code()), values);
                    if(!ret){
                        isAllDone = false;
                        message(QString("第%1行数据保存失败").arg(j + 1), sjdrElement, MSG_ERROR);
                    }
                }
            }
            if(isAllDone){
                message(QString("所有数据保存成功"), sjdrElement, MSG_SUCCESS);
            }
            delete plainModel;
            break;
        }
    }
}

/**
 * @brief SjdrControl::controlMainlandAutomaticWind
 * @param sjdrElement
 * 大陆自动站风
 */
void SjdrControl::controlMainlandAutomaticWind(const SjdrElement &sjdrElement){
    QFileInfo fileInfo = sjdrElement.m_fileInfo;
    QualityControlSource qualityControlSource = sjdrElement.m_qualityControlSource;

    QFile file(fileInfo.absoluteFilePath());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        message(QString("文件无法打开"), sjdrElement, MSG_ERROR);
        return;
    }
    QTextStream fileInput(&file);
    QString lineStr;
    bool isStart = false;
    int line = 0;
    QStringList titleList = qualityControlSource.fields().split(",");
    bool isAllDone = true;
    while(!fileInput.atEnd())
    {
        if(!m_execute){
            return;
        }
        line++;
        lineStr = fileInput.readLine();
        if(!isStart){
            if(lineStr.replace("	", ",").compare(qualityControlSource.fields()) == 0){
                isStart = true;
            }
        }else{
            QStringList valueList = lineStr.split(QRegExp("\\t"));
            int fieldCount = mainland_automaticwind_fields.size();
            if(fieldCount > valueList.size()){
                isAllDone = false;
                message(QString("第%1行数据保存失败").arg(line), sjdrElement, MSG_ERROR);
                continue;
            }else{
                QList<QVariant> values;
                for(int i = 0;i < fieldCount;i++){
                    int valueIndex = titleList.indexOf(mainland_automaticwind_fields[i]);
                    if(valueIndex < 0){
                        isAllDone = false;
                        message(QString("未找到字段[%1]").arg(mainland_automaticwind_fields[i]), sjdrElement, MSG_ERROR);
                        break;
                    }else{
                        values.append(QVariant(valueList[valueIndex]));
                    }
                }
                if(values.size() == fieldCount){
                    bool ret = pgdb->save(insertMainlandAutomaticWind.arg(m_airport.code()), values);
                    if(!ret){
                        isAllDone = false;
                        message(QString("第%1行数据保存失败").arg(line), sjdrElement, MSG_ERROR);
                    }
                }
            }
        }
    }
    if(isAllDone){
        message(QString("所有数据保存成功"), sjdrElement, MSG_SUCCESS);
    }
    file.close();
}

/**
 * @brief SjdrControl::controlMacaoAutomaticWind
 * @param sjdrElement
 * 澳门自动站风
 */
void SjdrControl::controlMacaoAutomaticWind(const SjdrElement &sjdrElement){
    QFileInfo fileInfo = sjdrElement.m_fileInfo;
    QualityControlSource qualityControlSource = sjdrElement.m_qualityControlSource;

    QFile file(fileInfo.absoluteFilePath());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        message(QString("文件无法打开"), sjdrElement, MSG_ERROR);
        return;
    }
    QTextStream fileInput(&file);
    QString lineStr;
    bool isStart = false;
    int line = 0;
    QStringList titleList = qualityControlSource.fields().split(",");
    bool isAllDone = true;
    while(!fileInput.atEnd())
    {
        if(!m_execute){
            return;
        }
        line++;
        lineStr = fileInput.readLine();
        if(!isStart){
            if(lineStr.replace("	", ",").compare(qualityControlSource.fields()) == 0){
                isStart = true;
            }
        }else{
            QStringList valueList = lineStr.split(QRegExp("\\t"));
            int fieldCount = macao_automaticwind_fields.size();
            if(fieldCount > valueList.size()){
                isAllDone = false;
                message(QString("第%1行数据保存失败").arg(line), sjdrElement, MSG_ERROR);
                continue;
            }else{
                //前半部分
                QList<QVariant> values;
                for(int i = 0;i < fieldCount;i++){
                    int valueIndex = titleList.indexOf(macao_automaticwind_fields[i]);
                    if(valueIndex < 0){
                        isAllDone = false;
                        message(QString("未找到字段[%1]").arg(macao_automaticwind_fields[i]), sjdrElement, MSG_ERROR);
                        break;
                    }else{
                        if(macao_automaticwind_fields[i].indexOf("(KT)") >= 0){
                            float windSpeed = (valueList[valueIndex]).toFloat();
                            values.append(QVariant(QString("%1").arg(windSpeed * 0.5144)));
                        }else{
                            values.append(QVariant(valueList[valueIndex]));
                        }
                    }
                }
                if(values.size() == fieldCount){
                    bool ret = pgdb->save(insertMacaoAutomaticWind.arg(m_airport.code()), values);
                    if(!ret){
                        isAllDone = false;
                        message(QString("第%1行数据保存失败").arg(line), sjdrElement, MSG_ERROR);
                    }
                }
                //后半部分
                values.clear();
                for(int i = 0;i < fieldCount;i++){
                    int valueIndex = titleList.lastIndexOf(macao_automaticwind_fields[i]);
                    if(valueIndex < 0){
                        isAllDone = false;
                        message(QString("未找到字段[%1]").arg(macao_automaticwind_fields[i]), sjdrElement, MSG_ERROR);
                        break;
                    }else{
                        if(macao_automaticwind_fields[i].indexOf("(KT)") >= 0){
                            float windSpeed = (valueList[valueIndex]).toFloat();
                            values.append(QVariant(QString("%1").arg(windSpeed * 0.5144)));
                        }else{
                            values.append(QVariant(valueList[valueIndex]));
                        }
                    }
                }
                if(values.size() == fieldCount){
                    bool ret = pgdb->save(insertMacaoAutomaticWind.arg(m_airport.code()), values);
                    if(!ret){
                        isAllDone = false;
                        message(QString("第%1行数据保存失败").arg(line), sjdrElement, MSG_ERROR);
                    }
                }
            }
        }
    }
    if(isAllDone){
        message(QString("所有数据保存成功"), sjdrElement, MSG_SUCCESS);
    }
    file.close();
}

/**
 * @brief SjdrControl::controlMainlandAutomaticTemperature
 * @param sjdrElement
 * 大陆自动站温度气压
 */
void SjdrControl::controlMainlandAutomaticTemperature(const SjdrElement &sjdrElement){
    QFileInfo fileInfo = sjdrElement.m_fileInfo;
    QualityControlSource qualityControlSource = sjdrElement.m_qualityControlSource;

    QFile file(fileInfo.absoluteFilePath());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        message(QString("文件无法打开"), sjdrElement, MSG_ERROR);
        return;
    }
    QTextStream fileInput(&file);
    QString lineStr;
    bool isStart = false;
    int line = 0;
    QStringList titleList = qualityControlSource.fields().replace(QRegExp("[0-9]+"), "").split(",");
    bool isAllDone = true;
    while(!fileInput.atEnd())
    {
        if(!m_execute){
            return;
        }
        line++;
        lineStr = fileInput.readLine();
        if(!isStart){
            if(lineStr.replace("	", ",").replace(QRegExp("[0-9]+"), "").compare(titleList.join(",")) == 0){
                isStart = true;
            }
        }else{
            QStringList valueList = lineStr.split(QRegExp("\\t"));
            int fieldCount = automatictemperature_fields.size();
            if(fieldCount > valueList.size()){
                isAllDone = false;
                message(QString("第%1行数据保存失败").arg(line), sjdrElement, MSG_ERROR);
                continue;
            }else{
                QList<QVariant> values;
                for(int i = 0;i < fieldCount;i++){
                    int valueIndex = titleList.indexOf(automatictemperature_fields[i]);
                    if(valueIndex < 0){
                        isAllDone = false;
                        message(QString("未找到字段[%1]").arg(automatictemperature_fields[i]), sjdrElement, MSG_ERROR);
                        break;
                    }else{
                        values.append(QVariant(valueList[valueIndex]));
                    }
                }
                if(values.size() == fieldCount){
                    bool ret = pgdb->save(insertAutomaticTeperature.arg(m_airport.code()), values);
                    if(!ret){
                        isAllDone = false;
                        message(QString("第%1行数据保存失败").arg(line), sjdrElement, MSG_ERROR);
                    }
                }
            }
        }
    }
    if(isAllDone){
        message(QString("所有数据保存成功"), sjdrElement, MSG_SUCCESS);
    }
    file.close();
}

/**
 * @brief SjdrControl::controlUnknown
 * @param sjdrElement
 * 未知
 */
void SjdrControl::controlUnknown(const SjdrElement &sjdrElement){
    message(QString("未识别数据的数据源"), sjdrElement, MSG_UNDEFINE);
}
