#include "kfttjcontrol.h"
#include "common/sharedmemory.h"

KfttjControl::KfttjControl(QObject *parent)
    :QThread(parent)
{
    pgdb = new PgDataBase;
}

KfttjControl::~KfttjControl(){
    delete pgdb;
}

void KfttjControl::setTitleList(QStringList titleList){
    this->m_titleList = titleList;
}

void KfttjControl::setSummarySql(QString summarySql){
    this->m_summarySql = summarySql;
}

void KfttjControl::setExtremumSql(QString extremumSql){
    this->m_extremumSql = extremumSql;
}

void KfttjControl::setAirportCode(QString airportCode){
    this->m_airportCode = airportCode;
}

void KfttjControl::setPlaneName(QString planeName){
    this->m_planeName = planeName;
}

void KfttjControl::setMultiWeather(bool isMultiWeather){
    this->m_isMultiWeather = isMultiWeather;
}

void KfttjControl::setWeatherParamList(QList<WeatherParam> wpList){
    this->m_wpList = wpList;
}

QHash< QString, QList<float> > KfttjControl::getKfttjHash() const {
    return this->kfttjHash;
}

QHash< QString, QStringList > KfttjControl::getXzkfEffectHash() const {
    return this->xzkfEffectHash;
}

QHash< QString, QStringList > KfttjControl::getBkfEffectHash() const {
    return this->bkfEffectHash;
}

void KfttjControl::initData(){
    kfttjHash.clear();
    xzkfEffectHash.clear();
    bkfEffectHash.clear();
    effectHash.clear();
    resAll.clear();
    summaryList.clear();
    extremumList.clear();

    //需要统计的气象参数个数
    elementCount = m_wpList.count() + 1;

    QList<Airport> airportList = SharedMemory::getInstance()->getAirportList();
    Airport currentAirport;
    for(Airport airport : airportList){
        if(airport.code().compare(m_airportCode, Qt::CaseInsensitive) == 0){
            currentAirport = airport;
            break;
        }
    }
    //跑道方向
    m_runwayDirection = currentAirport.direction() * PI / 180;
    //白天,整天,半天
    if(currentAirport.longitude() >= 110){
        //东经110度以东
        jan_day = JAN_DAY_E;
        jan_day_len = sizeof(JAN_DAY_E) / sizeof(JAN_DAY_E[0]);
        feb_day = FEB_DAY_E;
        feb_day_len = sizeof(FEB_DAY_E) / sizeof(FEB_DAY_E[0]);
        mar_day = MAR_DAY_E;
        mar_day_len = sizeof(MAR_DAY_E) / sizeof(MAR_DAY_E[0]);
        apr_day = APR_DAY_E;
        apr_day_len = sizeof(APR_DAY_E) / sizeof(APR_DAY_E[0]);
        may_day = MAY_DAY_E;
        may_day_len = sizeof(MAY_DAY_E) / sizeof(MAY_DAY_E[0]);
        jun_day = JUN_DAY_E;
        jun_day_len = sizeof(JUN_DAY_E) / sizeof(JUN_DAY_E[0]);
        jul_day = JUL_DAY_E;
        jul_day_len = sizeof(JUL_DAY_E) / sizeof(JUL_DAY_E[0]);
        aug_day = AUG_DAY_E;
        aug_day_len = sizeof(AUG_DAY_E) / sizeof(AUG_DAY_E[0]);
        sep_day = SEP_DAY_E;
        sep_day_len = sizeof(SEP_DAY_E) / sizeof(SEP_DAY_E[0]);
        oct_day = OCT_DAY_E;
        oct_day_len = sizeof(OCT_DAY_E) / sizeof(OCT_DAY_E[0]);
        nov_day = NOV_DAY_E;
        nov_day_len = sizeof(NOV_DAY_E) / sizeof(NOV_DAY_E[0]);
        dec_day = DEC_DAY_E;
        dec_day_len = sizeof(DEC_DAY_E) / sizeof(DEC_DAY_E[0]);
        half_day = HALF_DAY_E;
        whole_day = WHOLE_DAY_E;
    }else if(currentAirport.longitude() < 110 && currentAirport.longitude() >= 95){
        //东经95度以东~东经110度
        jan_day = JAN_DAY_M;
        jan_day_len = sizeof(JAN_DAY_M) / sizeof(JAN_DAY_M[0]);
        feb_day = FEB_DAY_M;
        feb_day_len = sizeof(FEB_DAY_M) / sizeof(FEB_DAY_M[0]);
        mar_day = MAR_DAY_M;
        mar_day_len = sizeof(MAR_DAY_M) / sizeof(MAR_DAY_M[0]);
        apr_day = APR_DAY_M;
        apr_day_len = sizeof(APR_DAY_M) / sizeof(APR_DAY_M[0]);
        may_day = MAY_DAY_M;
        may_day_len = sizeof(MAY_DAY_M) / sizeof(MAY_DAY_M[0]);
        jun_day = JUN_DAY_M;
        jun_day_len = sizeof(JUN_DAY_M) / sizeof(JUN_DAY_M[0]);
        jul_day = JUL_DAY_M;
        jul_day_len = sizeof(JUL_DAY_M) / sizeof(JUL_DAY_M[0]);
        aug_day = AUG_DAY_M;
        aug_day_len = sizeof(AUG_DAY_M) / sizeof(AUG_DAY_M[0]);
        sep_day = SEP_DAY_M;
        sep_day_len = sizeof(SEP_DAY_M) / sizeof(SEP_DAY_M[0]);
        oct_day = OCT_DAY_M;
        oct_day_len = sizeof(OCT_DAY_M) / sizeof(OCT_DAY_M[0]);
        nov_day = NOV_DAY_M;
        nov_day_len = sizeof(NOV_DAY_M) / sizeof(NOV_DAY_M[0]);
        dec_day = DEC_DAY_M;
        dec_day_len = sizeof(DEC_DAY_M) / sizeof(DEC_DAY_M[0]);
        half_day = HALF_DAY_M;
        whole_day = WHOLE_DAY_M;
    }else{
        //东经95度以西
        jan_day = JAN_DAY_W;
        jan_day_len = sizeof(JAN_DAY_W) / sizeof(JAN_DAY_W[0]);
        feb_day = FEB_DAY_W;
        feb_day_len = sizeof(FEB_DAY_W) / sizeof(FEB_DAY_W[0]);
        mar_day = MAR_DAY_W;
        mar_day_len = sizeof(MAR_DAY_W) / sizeof(MAR_DAY_W[0]);
        apr_day = APR_DAY_W;
        apr_day_len = sizeof(APR_DAY_W) / sizeof(APR_DAY_W[0]);
        may_day = MAY_DAY_W;
        may_day_len = sizeof(MAY_DAY_W) / sizeof(MAY_DAY_W[0]);
        jun_day = JUN_DAY_W;
        jun_day_len = sizeof(JUN_DAY_W) / sizeof(JUN_DAY_W[0]);
        jul_day = JUL_DAY_W;
        jul_day_len = sizeof(JUL_DAY_W) / sizeof(JUL_DAY_W[0]);
        aug_day = AUG_DAY_W;
        aug_day_len = sizeof(AUG_DAY_W) / sizeof(AUG_DAY_W[0]);
        sep_day = SEP_DAY_W;
        sep_day_len = sizeof(SEP_DAY_W) / sizeof(SEP_DAY_W[0]);
        oct_day = OCT_DAY_W;
        oct_day_len = sizeof(OCT_DAY_W) / sizeof(OCT_DAY_W[0]);
        nov_day = NOV_DAY_W;
        nov_day_len = sizeof(NOV_DAY_W) / sizeof(NOV_DAY_W[0]);
        dec_day = DEC_DAY_W;
        dec_day_len = sizeof(DEC_DAY_W) / sizeof(DEC_DAY_W[0]);
        half_day = HALF_DAY_W;
        whole_day = WHOLE_DAY_W;
    }
    //气象要素阀值
    weatherParamSetupList.clear();
    QString queryStr = QString("select * from weatherparamsetup where code = '%1' and planename = '%2' order by paramid")
            .arg(currentAirport.code())
            .arg(currentAirport.planeName());
    QSqlQueryModel *plainModel = pgdb->queryModel(queryStr);
    int rowCount = plainModel->rowCount();
    for(int i = 0;i < rowCount;i++){
        WeatherParamSetup weatherParamSetup;
        weatherParamSetup.setCode(plainModel->record(i).value(0).toString());
        weatherParamSetup.setPlaneName(plainModel->record(i).value(1).toString());
        weatherParamSetup.setParamid(plainModel->record(i).value(2).toInt());
        weatherParamSetup.setLimits(plainModel->record(i).value(3).toString());
        weatherParamSetupList.append(weatherParamSetup);
    }
    delete plainModel;
}

void KfttjControl::run(){
    //更新进度
    emit setProgressValue(1, 100);
    //初始化
    initData();
    //查询月总薄及极值
    query();
    //数据分析
    analysis();
    //发送执行完成的信号
    emit execute(true);
}

void KfttjControl::query(){
    //查询月总簿表
    QSqlQueryModel *summaryPlainModel = pgdb->queryModel(m_summarySql);
    int summaryRowCount = summaryPlainModel->rowCount();
    for(int i = 0;i < summaryRowCount;i++){
        Monthsummary monthsummary;
        monthsummary.setDatetime(summaryPlainModel->record(i).value(0).toString());
        monthsummary.setWinddirection(summaryPlainModel->record(i).value(1).toString());
        monthsummary.setWindspeed(summaryPlainModel->record(i).value(2).toString());
        monthsummary.setGustspeed(summaryPlainModel->record(i).value(3).toString());
        monthsummary.setLeadingvisibility(summaryPlainModel->record(i).value(4).toString());
        monthsummary.setRunway1no(summaryPlainModel->record(i).value(5).toString());
        monthsummary.setRunway1vr(summaryPlainModel->record(i).value(6).toString());
        monthsummary.setRunway2no(summaryPlainModel->record(i).value(7).toString());
        monthsummary.setRunway2vr(summaryPlainModel->record(i).value(8).toString());
        monthsummary.setRunway3no(summaryPlainModel->record(i).value(9).toString());
        monthsummary.setRunway3vr(summaryPlainModel->record(i).value(10).toString());
        monthsummary.setRunway4no(summaryPlainModel->record(i).value(11).toString());
        monthsummary.setRunway4vr(summaryPlainModel->record(i).value(12).toString());
        monthsummary.setRunway5no(summaryPlainModel->record(i).value(13).toString());
        monthsummary.setRunway5vr(summaryPlainModel->record(i).value(14).toString());
        monthsummary.setPhenomena(summaryPlainModel->record(i).value(15).toString());
        monthsummary.setTotalcloudcover(summaryPlainModel->record(i).value(16).toString());
        monthsummary.setLowcloudcover(summaryPlainModel->record(i).value(17).toString());
        monthsummary.setLowcloudstate1(summaryPlainModel->record(i).value(18).toString());
        monthsummary.setLowcloudstate2(summaryPlainModel->record(i).value(19).toString());
        monthsummary.setLowcloudstate3(summaryPlainModel->record(i).value(20).toString());
        monthsummary.setLowcloudstate4(summaryPlainModel->record(i).value(21).toString());
        monthsummary.setLowcloudstate5(summaryPlainModel->record(i).value(22).toString());
        monthsummary.setMiddlecloudstate1(summaryPlainModel->record(i).value(23).toString());
        monthsummary.setMiddlecloudstate2(summaryPlainModel->record(i).value(24).toString());
        monthsummary.setMiddlecloudstate3(summaryPlainModel->record(i).value(25).toString());
        monthsummary.setHighcloudstate1(summaryPlainModel->record(i).value(26).toString());
        monthsummary.setHighcloudstate2(summaryPlainModel->record(i).value(27).toString());
        monthsummary.setHighcloudstate3(summaryPlainModel->record(i).value(28).toString());
        monthsummary.setTemperature(summaryPlainModel->record(i).value(29).toString());
        monthsummary.setRelativehumidity(summaryPlainModel->record(i).value(30).toString());
        monthsummary.setDewpoint(summaryPlainModel->record(i).value(31).toString());
        monthsummary.setAirdromepressure(summaryPlainModel->record(i).value(32).toString());
        monthsummary.setCorrectedseapressure(summaryPlainModel->record(i).value(33).toString());
        summaryList.append(monthsummary);
    }
    delete summaryPlainModel;

    //查询极值表
    QSqlQueryModel *extremumPlainModel = pgdb->queryModel(m_extremumSql);
    int extremumRowCount = extremumPlainModel->rowCount();
    for(int i = 0;i < extremumRowCount;i++){
        Extremum extremum;
        extremum.setDatetime(extremumPlainModel->record(i).value(0).toString());
        extremum.setHightemperature(extremumPlainModel->record(i).value(1).toString());
        extremum.setLowtemperature(extremumPlainModel->record(i).value(2).toString());
        extremum.setPrecipitation(extremumPlainModel->record(i).value(3).toString());
        extremum.setMaxsnowdepth(extremumPlainModel->record(i).value(4).toString());
        extremum.setEvolution1(extremumPlainModel->record(i).value(5).toString());
        extremum.setEvolution2(extremumPlainModel->record(i).value(6).toString());
        extremum.setEvolution3(extremumPlainModel->record(i).value(7).toString());
        extremum.setEvolution4(extremumPlainModel->record(i).value(8).toString());
        extremum.setEvolution5(extremumPlainModel->record(i).value(9).toString());
        extremum.setEvolution6(extremumPlainModel->record(i).value(10).toString());
        extremum.setEvolution7(extremumPlainModel->record(i).value(11).toString());
        extremum.setEvolution8(extremumPlainModel->record(i).value(12).toString());
        extremum.setEvolution9(extremumPlainModel->record(i).value(13).toString());
        extremum.setEvolution10(extremumPlainModel->record(i).value(14).toString());
        extremumList.append(extremum);
    }
    delete extremumPlainModel;
}


/**
 * @brief KfttjControl::isDayTime
 * 判断是否为白天时间
 * @param dateTime 世界时
 * @return
 */
bool KfttjControl::isDayTime(QDateTime currentDateTime_utc){
    bool canExecute = false;
    QDateTime currentDateTime_local = currentDateTime_utc.addSecs(3600 * 8 - 1);
    int month = currentDateTime_local.toString("M").toInt(); //用中国时取得月份
    int hour = currentDateTime_utc.toString("h").toInt(); //用世界时取得小时
    if(1 == month){
        if(jan_day[0] > jan_day[jan_day_len - 1]){
            if(hour >= jan_day[0] || hour <= jan_day[jan_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= jan_day[0] && hour <= jan_day[jan_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(2 == month){
        if(feb_day[0] > feb_day[feb_day_len - 1]){
            if(hour >= feb_day[0] || hour <= feb_day[feb_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= feb_day[0] && hour <= feb_day[feb_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(3 == month){
        if(mar_day[0] > mar_day[mar_day_len - 1]){
            if(hour >= mar_day[0] || hour <= mar_day[mar_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= mar_day[0] && hour <= mar_day[mar_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(4 == month){
        if(apr_day[0] > apr_day[apr_day_len - 1]){
            if(hour >= apr_day[0] || hour <= apr_day[apr_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= apr_day[0] && hour <= apr_day[apr_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(5 == month){
        if(may_day[0] > may_day[may_day_len - 1]){
            if(hour >= may_day[0] || hour <= may_day[may_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= may_day[0] && hour <= may_day[may_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(6 == month){
        if(jun_day[0] > jun_day[jun_day_len - 1]){
            if(hour >= jun_day[0] || hour <= jun_day[jun_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= jun_day[0] && hour <= jun_day[jun_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(7 == month){
        if(jul_day[0] > jul_day[jul_day_len - 1]){
            if(hour >= jul_day[0] || hour <= jul_day[jul_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= jul_day[0] && hour <= jul_day[jul_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(8 == month){
        if(aug_day[0] > aug_day[aug_day_len - 1]){
            if(hour >= aug_day[0] || hour <= aug_day[aug_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= aug_day[0] && hour <= aug_day[aug_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(9 == month){
        if(sep_day[0] > sep_day[sep_day_len - 1]){
            if(hour >= sep_day[0] || hour <= sep_day[sep_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= sep_day[0] && hour <= sep_day[sep_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(10 == month){
        if(oct_day[0] > oct_day[oct_day_len - 1]){
            if(hour >= oct_day[0] || hour <= oct_day[oct_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= oct_day[0] && hour <= oct_day[oct_day_len - 1]){
                canExecute = true;
            }
        }
    }else if(11 == month){
        if(nov_day[0] > nov_day[nov_day_len - 1]){
            if(hour >= nov_day[0] || hour <= nov_day[nov_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= nov_day[0] && hour <= nov_day[nov_day_len - 1]){
                canExecute = true;
            }
        }
    }else{
        if(dec_day[0] > dec_day[dec_day_len - 1]){
            if(hour >= dec_day[0] || hour <= dec_day[dec_day_len - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= dec_day[0] && hour <= dec_day[dec_day_len - 1]){
                canExecute = true;
            }
        }
    }

    return canExecute;
}

QList<QString> KfttjControl::getDataFromJson(QString jsonStr){
    QList<QString> valueList;
    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8(), &jsonErr);
    if(jsonErr.error == QJsonParseError::NoError){
        if(!jsonDoc.isEmpty()){
            if(jsonDoc.isObject()){
                QJsonObject jobj = jsonDoc.object();
                QJsonObject::iterator it = jobj.begin();
                while(it != jobj.end()){
                    if(QJsonValue::Array == it.value().type()){
                        QJsonArray array = it.value().toArray();
                        int subArrayCount = array.count();
                        for(int i = 0;i < subArrayCount;i++){
                            QJsonArray subArray = array.at(i).toArray();
                            valueList.append(subArray.at(0).toString());
                            valueList.append(subArray.at(1).toString());
                        }
                    }
                    it++;
                }
            }
        }
    }
    return valueList;
}

/**
 * @brief KfttjControl::isMatchArgs
 * 判断value是否>=fromStr并且<toStr
 * @param value
 * @param fromStr
 * @param toStr
 * @return
 */
bool KfttjControl::isMatchArgs(float value, QString fromStr, QString toStr){
    if(fromStr.isEmpty() && toStr.isEmpty()){
        return true;
    }else if(!fromStr.isEmpty() && toStr.isEmpty()){
        float fromValue = fromStr.toFloat();
        if(value >= fromValue){
            return true;
        }else{
            return false;
        }
    }else if(fromStr.isEmpty() && !toStr.isEmpty()){
        float toValue = toStr.toFloat();
        if(value < toValue){
            return true;
        }else{
            return false;
        }
    }else{
        float fromValue = fromStr.toFloat();
        float toValue = toStr.toFloat();
        if(value >= fromValue && value < toValue){
            return true;
        }else{
            return false;
        }
    }
}

/**
 * @brief KfttjControl::getHalfOrWholeDay
 * 判断是否为半天或者整天
 * @param currentDateTime_utc   0:什么也不是 1:半天 2:整天
 * @return
 */
int KfttjControl::getHalfOrWholeDay(QDateTime currentDateTime_utc){
    int hour = currentDateTime_utc.toString("h").toInt();
    if(hour - half_day == 1 || hour - half_day == 2){
        return 1;
    }else if(hour - whole_day == 1 || hour - whole_day == 2){
        return 2;
    }
    return 0;
}

int KfttjControl::getExtremumIndex(QDateTime currentDateTime_utc){
    int index = -1;
    int extremumCount = extremumList.size();
    for(int i = 0;i < extremumCount;i++){
        Extremum extremum = extremumList[i];
        QDateTime dateTime = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
        if(dateTime.daysTo(currentDateTime_utc) == 0){
            index = i;
            break;
        }
    }
    return index;
}

/**
 * @brief KfttjControl::analysis
 * 数据分析
 */
void KfttjControl::analysis(){
    int summaryCount = summaryList.size();
    int dateCount = 0;
    QDateTime lastDateTime_local;
    for(int i = 0;i < summaryCount;i++){
        /***表格整体布局***/
        Monthsummary monthsummary = summaryList[i];
        QDateTime currentDateTime_local = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss").addSecs(3600 * 8 - 1);
        QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
        bool canInsertRows = false;
        if(i == 0){
            lastDateTime_local = currentDateTime_local;
            canInsertRows = true;
        }else{
            if(lastDateTime_local.daysTo(currentDateTime_local) >= 1){
                dateCount += 1;
                canInsertRows = true;
            }
        }
        if(canInsertRows){
            emit sendMessage(dateCount * elementCount, elementCount);
            //日期
            emit sendMessage(currentDateTime_local.toString("yyyy年MM月dd日"), dateCount * elementCount, 0, elementCount, 1);
            int weatherParamCount = m_wpList.count();
            for(int j = 0;j < weatherParamCount;j++){
                WeatherParam weatherParam = m_wpList[j];
                emit sendMessage(weatherParam.name(), dateCount * elementCount + j, 1, 1, 1);
            }
            //综合
            emit sendMessage("综合", dateCount * elementCount + weatherParamCount, 1, 1, 1);
            //完全可飞
            emit sendMessage("", dateCount * elementCount, m_titleList.indexOf("完全可飞"), elementCount, 1);
            //限制可飞
            emit sendMessage("", dateCount * elementCount, m_titleList.indexOf("限制可飞"), elementCount, 1);
            //不可飞
            emit sendMessage("", dateCount * elementCount, m_titleList.indexOf("不可飞"), elementCount, 1);
            //影响原因
            emit sendMessage("", dateCount * elementCount, m_titleList.size() - 1, elementCount, 1);

            //日可飞天统计
            if(i > 0){
                analysisDay(lastDateTime_local, (dateCount - 1) * elementCount);
            }
        }

        lastDateTime_local = currentDateTime_local;

        /***数据分析***/
        bool canExecute = m_isMultiWeather ? isDayTime(currentDateTime_utc) : true;
        int hour = currentDateTime_utc.toString("h").toInt();
        if(canExecute){
            QStringList results;
            int weatherParamCount = m_wpList.count();
            for(int j = 0;j < weatherParamCount;j++){
                WeatherParam weatherParam = m_wpList[j];
                switch(weatherParam.id()){
                case 0:
                    //风.非强风模式.矢量风.顺风
                    results.append(analysisMultiSf(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 1:
                    //风.非强风模式.矢量风.逆风
                    results.append(analysisMultiNf(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 2:
                    //风.非强风模式.矢量风.侧风
                    results.append(analysisMultiCf(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 3:
                    //风.非强风模式.标量风
                    results.append(analysisMultiBlf(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 4:
                    //温度.常规温
                    results.append(analysisMultiCgw(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 5:
                    //气压.场面气压
                    results.append(analysisMultiCmqy(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 6:
                    //气压.修正海面气压
                    results.append(analysisMultiXzhmqy(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 7:
                    //湿度.常规湿.比湿
                    results.append(analysisMultiCgs(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 8:
                    //湿度.极湿.比湿
                    results.append(analysisMultiJs(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 9:
                    //能见度.水平能见度
                    results.append(analysisMultiSpnjd(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 10:
                    //云
                    results.append(analysisMultiYlyg(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 11:
                    //降水.非强降水模式
                    results.append(analysisMultiFqjs(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 12:
                    //危险天气.浓积云
                    results.append(analysisMultiNjy(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 13:
                    //危险天气.积雨云
                    results.append(analysisMultiJyy(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 14:
                    //危险天气.闪电
                    results.append(analysisMultiSd(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 15:
                    //危险天气.雷暴
                    results.append(analysisMultiLb(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 16:
                    //危险天气.冰雹
                    results.append(analysisMultiBb(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 17:
                    //危险天气.飑线
                    results.append(analysisMultiBx(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 18:
                    //危险天气.龙卷
                    results.append(analysisMultiLj(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 19:
                    //危险天气.沙尘暴
                    results.append(analysisMultiScb(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 20:
                    //危险天气.风切变
                    results.append(analysisMultiFqb(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 21:
                    //风.强风模式.矢量风.顺风
                    results.append(analysisSingleSf(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 22:
                    //风.强风模式.矢量风.逆风
                    results.append(analysisSingleNf(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 23:
                    //风.强风模式.矢量风.侧风
                    results.append(analysisSingleCf(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 24:
                    //风.强风模式.标量风
                    results.append(analysisSingleBlf(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                case 25:
                    //温度.极高温
                    results.append(analysisSingleJgw(monthsummary, dateCount * elementCount + j, 2));
                    break;
                case 26:
                    //温度.极低温
                    results.append(analysisSingleJdw(monthsummary, dateCount * elementCount + j, 2));
                    break;
                case 27:
                    //降水.强降水模式
                    results.append(analysisSingleQjs(monthsummary, dateCount * elementCount + j, m_titleList.indexOf(QString::number(hour))));
                    break;
                }
            }
            //综合
            if(m_isMultiWeather){
                analysisAll(currentDateTime_local, results, dateCount * elementCount + weatherParamCount, m_titleList.indexOf(QString::number(hour)));
            }else{
                WeatherParam weatherParam = m_wpList[0];
                switch(weatherParam.id()){
                case 21:
                case 22:
                case 23:
                case 24:
                case 27:
                    analysisAll(currentDateTime_local, results, dateCount * elementCount + weatherParamCount, m_titleList.indexOf(QString::number(hour)));
                    break;
                default:
                    analysisAll(currentDateTime_local, results, dateCount * elementCount + weatherParamCount, 2);
                }
            }
        }else{
            resAll.append(0);
        }
        //更新进度
        emit setProgressValue(i + 1, summaryCount);
    }
    //日可飞天统计
    analysisDay(lastDateTime_local, dateCount * elementCount);
}

/**
 * @brief KfttjControl::guessVisibilityEvolution
 * 推算能见度
 * @param extremum
 * @param hour
 * @return
 */
QString KfttjControl::guessVisibilityEvolution(const Extremum &extremum, int hour, QList<QString> limitList){
    QString valueStr("");
    QString retStr("");
    QRegExp regExp("(BR|FG)([\\(\\)\\d\\-]*([\\(\\)\\d]+\\-)$)");

    QStringList evolutionList;
    evolutionList.append(extremum.evolution1().trimmed());
    evolutionList.append(extremum.evolution2().trimmed());
    evolutionList.append(extremum.evolution3().trimmed());
    evolutionList.append(extremum.evolution4().trimmed());
    evolutionList.append(extremum.evolution5().trimmed());
    evolutionList.append(extremum.evolution6().trimmed());
    evolutionList.append(extremum.evolution7().trimmed());
    evolutionList.append(extremum.evolution8().trimmed());
    evolutionList.append(extremum.evolution9().trimmed());
    evolutionList.append(extremum.evolution10().trimmed());

    int evolutionCount = evolutionList.size();
    for(int i = 0;i < evolutionCount;i++){
        QString evolution = evolutionList[i];
        int pos = regExp.indexIn(evolution);
        if(pos >= 0){
            QStringList retList = regExp.capturedTexts();
            retStr = retList[retList.size() - 1];
            break;
        }
    }

    /*
     * 1.水平能见度在05时的值为1000，06时的值为2000，能见度与天气现象演变中出现"BR(2500)0525-",则07，08时的能见度均为2000。
     * 2.水平能见度在05时的值为1000，06时的值为2000，能见度与天气现象演变中出现"BR(2500)0625-",则07，08时的能见度均为2500。
     * 3.水平能见度在05时的值为1000，06时的值为2000，能见度与天气现象演变中出现"BR(2500)0425-",则07时的能见度均为1000。
     * 4.如果1，2，3均不满足的话，则07，08时的能见度与06时保持一致。
     *
     */
    if(retStr.compare("") != 0){
        QString hh1 = QString("0%1\\d{2}\\-").arg(hour - 1);
        QRegExp regExp1(hh1);
        int pos1 = regExp1.indexIn(retStr);
        if(pos1 >= 0){
            valueStr = resVisibilityStr2;
        }else{
            QString hh2 = QString("\\((\\d+)\\)0%1\\d{2}\\-").arg(hour);
            QRegExp regExp2(hh2);
            int pos2 = regExp2.indexIn(retStr);
            if(pos2 >= 0){
                QStringList resList = regExp2.capturedTexts();
                int leadingvisibility = resList[resList.size() - 1].toInt();
                if(isMatchArgs(leadingvisibility, limitList[0], limitList[1])){
                    valueStr = QString("1");
                }else if(isMatchArgs(leadingvisibility, limitList[2], limitList[3])){
                    valueStr = QString("2");
                }else if(isMatchArgs(leadingvisibility, limitList[4], limitList[5])){
                    valueStr = QString("3");
                }else{

                }
            }else{
                QString hh3 = QString("0%1\\d{2}\\-").arg(hour - 2);
                QRegExp regExp3(hh3);
                int pos3 = regExp3.indexIn(retStr);
                if(pos3 >= 0){
                    valueStr = resVisibilityStr1;
                }else{
                    valueStr = resVisibilityStr2;
                }
            }
        }
    }else{
        valueStr = resVisibilityStr2;
    }

    return valueStr;
}

QString KfttjControl::guessCloudEvolution(const Extremum &extremum, int hour){
    QString valueStr("");
    QString retStr("");
    QRegExp regExp("(BR|FG)([\\(\\)\\d\\-]*([\\(\\)\\d]+\\-)$)");

    QStringList evolutionList;
    evolutionList.append(extremum.evolution1().trimmed());
    evolutionList.append(extremum.evolution2().trimmed());
    evolutionList.append(extremum.evolution3().trimmed());
    evolutionList.append(extremum.evolution4().trimmed());
    evolutionList.append(extremum.evolution5().trimmed());
    evolutionList.append(extremum.evolution6().trimmed());
    evolutionList.append(extremum.evolution7().trimmed());
    evolutionList.append(extremum.evolution8().trimmed());
    evolutionList.append(extremum.evolution9().trimmed());
    evolutionList.append(extremum.evolution10().trimmed());

    int evolutionCount = evolutionList.size();
    for(int i = 0;i < evolutionCount;i++){
        QString evolution = evolutionList[i];
        int pos = regExp.indexIn(evolution);
        if(pos >= 0){
            QStringList retList = regExp.capturedTexts();
            retStr = retList[retList.size() - 1];
            break;
        }
    }

    /*
     * 1.云量在05时的值为1000，06时的值为2000，能见度与天气现象演变中出现"BR(2500)0525-",则07，08时的云量均为2000。
     * 2.云量在05时的值为1000，06时的值为2000，能见度与天气现象演变中出现"BR(2500)0425-",则07时的云量均为1000。
     * 3.如果1，2均不满足的话，则07，08时的云量与06时保持一致。
     *
     */
    if(retStr.compare("") != 0){
        QString hh1 = QString("0%1\\d{2}\\-").arg(hour - 1);
        QRegExp regExp1(hh1);
        int pos1 = regExp1.indexIn(retStr);
        if(pos1 >= 0){
            valueStr = resCloudStr2;
        }else{
            QString hh2 = QString("0%1\\d{2}\\-").arg(hour - 2);
            QRegExp regExp2(hh2);
            int pos2 = regExp2.indexIn(retStr);
            if(pos2 >= 0){
                valueStr = resCloudStr1;
            }else{
                valueStr = resCloudStr2;
            }
        }
    }else{
        valueStr = resCloudStr2;
    }

    return valueStr;
}

/**
 * @brief KfttjControl::analysisMultiSf
 * 风.非强风模式.矢量风.顺风
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiSf(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 0){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QString windspeedStr = monthsummary.windspeed().trimmed();
    QString gustspeedStr = monthsummary.gustspeed().trimmed();
    QString winddirectionStr = monthsummary.winddirection().trimmed();

    if(windspeedStr.compare("") != 0 || gustspeedStr.compare("") != 0){
        float windspeed = windspeedStr.toFloat();
        float gustspeed = gustspeedStr.toFloat();
        float speed = qMax(windspeed, gustspeed);
        float tailspeed = speed;
        if(winddirectionStr.compare("C") != 0 && winddirectionStr.compare("VRB") != 0){
            float winddirection = winddirectionStr.toFloat();
            winddirection = winddirection * PI / 180;
            tailspeed = qCos(winddirection - m_runwayDirection) * speed;
        }
        //顺风
        if(isMatchArgs(-tailspeed, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(-tailspeed, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(-tailspeed, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiNf
 * 风.非强风模式.矢量风.逆风
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiNf(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 1){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QString windspeedStr = monthsummary.windspeed().trimmed();
    QString gustspeedStr = monthsummary.gustspeed().trimmed();
    QString winddirectionStr = monthsummary.winddirection().trimmed();
    if(windspeedStr.compare("") != 0 || gustspeedStr.compare("") != 0){
        float windspeed = windspeedStr.toFloat();
        float gustspeed = gustspeedStr.toFloat();
        float speed = qMax(windspeed, gustspeed);
        float headspeed = speed;
        if(winddirectionStr.compare("C") != 0 && winddirectionStr.compare("VRB") != 0){
            float winddirection = winddirectionStr.toFloat();
            winddirection = winddirection * PI / 180;
            headspeed = qCos(winddirection - m_runwayDirection) * speed;
        }
        //逆风
        if(isMatchArgs(headspeed, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(headspeed, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(headspeed, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiCf
 * 风.非强风模式.矢量风.侧风
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiCf(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 2){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QString windspeedStr = monthsummary.windspeed().trimmed();
    QString gustspeedStr = monthsummary.gustspeed().trimmed();
    QString winddirectionStr = monthsummary.winddirection().trimmed();
    if(windspeedStr.compare("") != 0 || gustspeedStr.compare("") != 0){
        float windspeed = windspeedStr.toFloat();
        float gustspeed = gustspeedStr.toFloat();
        float speed = qMax(windspeed, gustspeed);
        float crossspeed = speed;
        if(winddirectionStr.compare("C") != 0 && winddirectionStr.compare("VRB") != 0){
            float winddirection = winddirectionStr.toFloat();
            winddirection = winddirection * PI / 180;
            crossspeed = qAbs(qSin(winddirection - m_runwayDirection) * speed);
        }
        //侧风
        if(isMatchArgs(crossspeed, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(crossspeed, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(crossspeed, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiBlf
 * 风.非强风模式.标量风
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiBlf(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 3){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QString windspeedStr = monthsummary.windspeed().trimmed();
    QString gustspeedStr = monthsummary.gustspeed().trimmed();
    if(windspeedStr.compare("") != 0 || gustspeedStr.compare("") != 0){
        float windspeed = windspeedStr.toFloat();
        float gustspeed = gustspeedStr.toFloat();
        float speed = qMax(windspeed, gustspeed);
        //标量风
        if(isMatchArgs(speed, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(speed, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(speed, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiCgw
 * 温度.常规温
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiCgw(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 4){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 10){
        return resultStr;
    }

    QString temperatureStr = monthsummary.temperature().trimmed();
    if(temperatureStr.compare("") != 0){
        float temperature = temperatureStr.toFloat();
        //常规温
        if(isMatchArgs(temperature, limitList[0], limitList[1]) || isMatchArgs(temperature, limitList[8], limitList[9])){
            resultStr = QString("1");
        }else if(isMatchArgs(temperature, limitList[2], limitList[3]) || isMatchArgs(temperature, limitList[6], limitList[7])){
            resultStr = QString("2");
        }else if(isMatchArgs(temperature, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiCmqy
 * 气压.场面气压
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiCmqy(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 5){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 10){
        return resultStr;
    }

    QString airdromepressureStr = monthsummary.airdromepressure().trimmed();
    if(airdromepressureStr.compare("") != 0){
        float airdromepressure = airdromepressureStr.toFloat();
        //场面气压
        if(isMatchArgs(airdromepressure, limitList[0], limitList[1]) || isMatchArgs(airdromepressure, limitList[8], limitList[9])){
            resultStr = QString("1");
        }else if(isMatchArgs(airdromepressure, limitList[2], limitList[3]) || isMatchArgs(airdromepressure, limitList[6], limitList[7])){
            resultStr = QString("2");
        }else if(isMatchArgs(airdromepressure, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiXzhmqy
 * 气压.修正海面气压
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiXzhmqy(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 6){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 10){
        return resultStr;
    }

    QString correctedseapressureStr = monthsummary.correctedseapressure().trimmed();
    if(correctedseapressureStr.compare("") != 0){
        float correctedseapressure = correctedseapressureStr.toFloat();
        //修正海面气压
        if(isMatchArgs(correctedseapressure, limitList[0], limitList[1]) || isMatchArgs(correctedseapressure, limitList[8], limitList[9])){
            resultStr = QString("1");
        }else if(isMatchArgs(correctedseapressure, limitList[2], limitList[3]) || isMatchArgs(correctedseapressure, limitList[6], limitList[7])){
            resultStr = QString("2");
        }else if(isMatchArgs(correctedseapressure, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiCgs
 * 湿度.常规湿.比湿
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiCgs(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 7){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 10){
        return resultStr;
    }

    QString pressureStr = monthsummary.airdromepressure().trimmed();
    QString humidityStr = monthsummary.relativehumidity().trimmed();
    QString temperatureStr = monthsummary.temperature().trimmed();
    if(pressureStr.compare("") != 0 && humidityStr.compare("") != 0 && temperatureStr.compare("") != 0){
        float pressure = pressureStr.toFloat();
        float humidity = humidityStr.toFloat();
        float temperature = temperatureStr.toFloat();
        double q = 0.0;
        if(temperature >= -15){
            double a = 17.269;
            double b = 35.86;
            double es0 = 6.1078;
            double es = es0 * qExp(a * temperature / (273.15 + temperature - b));
            double e = es * humidity / 100;
            q = 0.622 * e / (pressure - 0.378 * e) * 1000;
        }else if(temperature < -40){
            double a = 21.8746;
            double b = 7.66;
            double es0 = 6.1078;
            double es = es0 * qExp(a * temperature / (273.15 + temperature - b));
            double e = es * humidity / 100;
            q = 0.622 * e / (pressure - 0.378 * e) * 1000;
        }else{
            double a_water = 17.269;
            double b_water = 35.86;
            double es0_water = 6.1078;
            double es_water = es0_water * qExp(a_water * temperature / (273.15 + temperature - b_water));
            double a_ice = 21.8746;
            double b_ice = 7.66;
            double es0_ice = 6.1078;
            double es_ice = es0_ice * qExp(a_ice * temperature / (273.15 + temperature - b_ice));
            double es = 0.002 * ((80 + 2 * temperature) * es_water - (30 + 2 * temperature) * es_ice);
            double e = es * humidity / 100;
            q = 0.622 * e / (pressure - 0.378 * e) * 1000;
        }
        //比湿
        if(isMatchArgs(q, limitList[0], limitList[1]) || isMatchArgs(q, limitList[8], limitList[9])){
            resultStr = QString("1");
        }else if(isMatchArgs(q, limitList[2], limitList[3]) || isMatchArgs(q, limitList[6], limitList[7])){
            resultStr = QString("2");
        }else if(isMatchArgs(q, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }

    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiJs
 * 湿度.极湿.比湿
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiJs(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 8){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QString pressureStr = monthsummary.airdromepressure().trimmed();
    QString humidityStr = monthsummary.relativehumidity().trimmed();
    QString temperatureStr = monthsummary.temperature().trimmed();
    if(pressureStr.compare("") != 0 && humidityStr.compare("") != 0 && temperatureStr.compare("") != 0){
        float pressure = pressureStr.toFloat();
        float humidity = humidityStr.toFloat();
        float temperature = temperatureStr.toFloat();
        double q = 0.0;
        if(temperature >= -15){
            double a = 17.269;
            double b = 35.86;
            double es0 = 6.1078;
            double es = es0 * qExp(a * temperature / (273.15 + temperature - b));
            double e = es * humidity / 100;
            q = 0.622 * e / (pressure - 0.378 * e) * 1000;
        }else if(temperature < -40){
            double a = 21.8746;
            double b = 7.66;
            double es0 = 6.1078;
            double es = es0 * qExp(a * temperature / (273.15 + temperature - b));
            double e = es * humidity / 100;
            q = 0.622 * e / (pressure - 0.378 * e) * 1000;
        }else{
            double a_water = 17.269;
            double b_water = 35.86;
            double es0_water = 6.1078;
            double es_water = es0_water * qExp(a_water * temperature / (273.15 + temperature - b_water));
            double a_ice = 21.8746;
            double b_ice = 7.66;
            double es0_ice = 6.1078;
            double es_ice = es0_ice * qExp(a_ice * temperature / (273.15 + temperature - b_ice));
            double es = 0.002 * ((80 + 2 * temperature) * es_water - (30 + 2 * temperature) * es_ice);
            double e = es * humidity / 100;
            q = 0.622 * e / (pressure - 0.378 * e) * 1000;
        }
        //比湿
        if(isMatchArgs(q, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(q, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(q, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }

    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiSpnjd
 * 能见度.水平能见度
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiSpnjd(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 9){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() == 0){
        return resultStr;
    }

    QString leadingvisibilityStr = monthsummary.leadingvisibility().trimmed();
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int hour = currentDateTime_utc.toString("h").toInt();
    if(leadingvisibilityStr.compare("///") == 0){
        resultStr = QString("1");
    }else if(leadingvisibilityStr.compare("") != 0){
        int leadingvisibility = leadingvisibilityStr.toInt();
        if(isMatchArgs(leadingvisibility, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(leadingvisibility, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(leadingvisibility, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }else{
        //推测
        int how = getHalfOrWholeDay(currentDateTime_utc);
        if(how == 1){
            int index = getExtremumIndex(currentDateTime_utc);
            if(index > -1){
                Extremum extremum = extremumList[index];
                resultStr = guessVisibilityEvolution(extremum, half_day, limitList);
            }
        }else if(how == 2){
            int index = getExtremumIndex(currentDateTime_utc);
            if(index > -1){
                Extremum extremum = extremumList[index];
                resultStr = guessVisibilityEvolution(extremum, whole_day, limitList);
            }
        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    //
    if(!(hour - half_day == 1 || hour - half_day == 2 || hour - whole_day == 1 || hour - whole_day == 2)){
        if(hour - half_day == -1){
            resVisibilityStr1 = resultStr;
        }else if(hour - half_day == 0){
            resVisibilityStr2 = resultStr;
        }else if(hour - whole_day == -1){
            resVisibilityStr1 = resultStr;
        }else if(hour - whole_day == 0){
            resVisibilityStr2 = resultStr;
        }else{
            resVisibilityStr1 = QString("");
            resVisibilityStr2 = QString("");
        }
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiYlyg
 * 云
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiYlyg(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 10){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() == 0){
        return resultStr;
    }

    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int hour = currentDateTime_utc.toString("h").toInt();
    QString totalcloudcoverStr = monthsummary.totalcloudcover().trimmed();
    QStringList cloudstateList;
    cloudstateList.append(monthsummary.lowcloudstate1().trimmed());
    cloudstateList.append(monthsummary.lowcloudstate2().trimmed());
    cloudstateList.append(monthsummary.lowcloudstate3().trimmed());
    cloudstateList.append(monthsummary.lowcloudstate4().trimmed());
    cloudstateList.append(monthsummary.lowcloudstate5().trimmed());
    cloudstateList.append(monthsummary.middlecloudstate1().trimmed());
    cloudstateList.append(monthsummary.middlecloudstate2().trimmed());
    cloudstateList.append(monthsummary.middlecloudstate3().trimmed());
    cloudstateList.append(monthsummary.highcloudstate1().trimmed());
    cloudstateList.append(monthsummary.highcloudstate2().trimmed());
    cloudstateList.append(monthsummary.highcloudstate3().trimmed());

    QRegExp totalcloudExp("([0-8]{1})");
    QRegExp cloudstateExp("(.*\\D+)([0-9]+)");

    int totalcloudPos = totalcloudExp.indexIn(totalcloudcoverStr);
    if(totalcloudPos >= 0){
        int totalcloudcover = totalcloudExp.capturedTexts()[0].toInt();
        if(totalcloudcover == 8){
            int cloudState = -1;
            int cloudstateCount = cloudstateList.size();
            for(int i = 0;i < cloudstateCount;i++){
                QString cloudstateStr = cloudstateList[i];
                if(cloudstateStr.compare("") != 0){
                    int cloudstatePos = cloudstateExp.indexIn(cloudstateStr);
                    if(cloudstatePos >= 0){
                        QStringList texts = cloudstateExp.capturedTexts();
                        if(cloudState == -1 || cloudState > texts[texts.size() - 1].toInt()){
                            cloudState = texts[texts.size() - 1].toInt();
                        }
                    }else{
                        resultStr = QString("1");
                        emit sendMessage(resultStr, row, col, 1, 1);
                        return resultStr;
                    }
                }
            }

            if(cloudState > -1){
                if(isMatchArgs(cloudState, limitList[0], limitList[1])){
                    resultStr = QString("1");
                }else if(isMatchArgs(cloudState, limitList[2], limitList[3])){
                    resultStr = QString("2");
                }else if(isMatchArgs(cloudState, limitList[4], limitList[5])){
                    resultStr = QString("3");
                }else{

                }
            }else{
                //推测
                int how = getHalfOrWholeDay(currentDateTime_utc);
                if(how == 1){
                    int index = getExtremumIndex(currentDateTime_utc);
                    if(index > -1){
                        Extremum extremum = extremumList[index];
                        resultStr = guessCloudEvolution(extremum, half_day);
                    }
                }else if(how == 2){
                    int index = getExtremumIndex(currentDateTime_utc);
                    if(index > -1){
                        Extremum extremum = extremumList[index];
                        resultStr = guessCloudEvolution(extremum, whole_day);
                    }
                }else{
                    resultStr = QString("3");
                }
            }
        }else{
            resultStr = QString("3");
        }
    }else if(totalcloudcoverStr.compare("-") == 0){
        resultStr = QString("1");
    }else{
        //推测
        int how = getHalfOrWholeDay(currentDateTime_utc);
        if(how == 1){
            int index = getExtremumIndex(currentDateTime_utc);
            if(index > -1){
                Extremum extremum = extremumList[index];
                resultStr = guessCloudEvolution(extremum, half_day);
            }
        }else if(how == 2){
            int index = getExtremumIndex(currentDateTime_utc);
            if(index > -1){
                Extremum extremum = extremumList[index];
                resultStr = guessCloudEvolution(extremum, whole_day);
            }
        }
    }

    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }

    //
    if(!(hour - half_day == 1 || hour - half_day == 2 || hour - whole_day == 1 || hour - whole_day == 2)){
        if(hour - half_day == -1){
            resCloudStr1 = resultStr;
        }else if(hour - half_day == 0){
            resCloudStr2 = resultStr;
        }else if(hour - whole_day == -1){
            resCloudStr1 = resultStr;
        }else if(hour - whole_day == 0){
            resCloudStr2 = resultStr;
        }else{
            resCloudStr1 = QString("");
            resCloudStr2 = QString("");
        }
    }

    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiFqjs
 * 降水.非强降水模式
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiFqjs(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    bool isFindJs = false;
    bool isMissing = true;
    if(index > -1){
        isMissing = false;
        Extremum extremum = extremumList[index];
        QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
        QDateTime yeasterday = today.addDays(-1);
        QHash< QString, int > jsHash;
        //天气演变列表
        QStringList evolutionList;
        evolutionList.append(extremum.evolution1());
        evolutionList.append(extremum.evolution2());
        evolutionList.append(extremum.evolution3());
        evolutionList.append(extremum.evolution4());
        evolutionList.append(extremum.evolution5());
        evolutionList.append(extremum.evolution6());
        evolutionList.append(extremum.evolution7());
        evolutionList.append(extremum.evolution8());
        evolutionList.append(extremum.evolution9());
        evolutionList.append(extremum.evolution10());

        for(QString evolution : evolutionList){
            jsHash = this->analysisJs(evolution, today, yeasterday);
            if(!jsHash.isEmpty()){
                QList<QString> keyList = jsHash.keys();
                for(QString timeStr : keyList){
                    if(timeStr.compare(monthsummary.datetime()) == 0){
                        if(jsHash[timeStr] != 0){
                            isFindJs = true;
                            resultStr = QString("%1").arg(jsHash[timeStr]);
                            break;
                        }
                    }
                }
            }
        }
    }
    if(!isFindJs){
        currentDateTime_utc = currentDateTime_utc.addDays(1);
        index = getExtremumIndex(currentDateTime_utc);
        if(index > -1){
            isMissing = false;
            Extremum extremum = extremumList[index];
            QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
            QDateTime yeasterday = today.addDays(-1);
            QHash< QString, int > jsHash;
            //天气演变列表
            QStringList evolutionList;
            evolutionList.append(extremum.evolution1());
            evolutionList.append(extremum.evolution2());
            evolutionList.append(extremum.evolution3());
            evolutionList.append(extremum.evolution4());
            evolutionList.append(extremum.evolution5());
            evolutionList.append(extremum.evolution6());
            evolutionList.append(extremum.evolution7());
            evolutionList.append(extremum.evolution8());
            evolutionList.append(extremum.evolution9());
            evolutionList.append(extremum.evolution10());

            for(QString evolution : evolutionList){
                jsHash = this->analysisJs(evolution, today, yeasterday);
                if(!jsHash.isEmpty()){
                    QList<QString> keyList = jsHash.keys();
                    for(QString timeStr : keyList){
                        if(timeStr.compare(monthsummary.datetime()) == 0){
                            if(jsHash[timeStr] != 0){
                                resultStr = QString("%1").arg(jsHash[timeStr]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if(isMissing){
        return resultStr;
    }else{
        if(resultStr.isEmpty()){
            resultStr = QString("3");
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);
    return resultStr;
}

/**
 * @brief KfttjControl::analysisJs
 * 存储降水量的hash(key:QString value:int -> 1:不可飞,2:限制可飞,3:可飞)
 * @param evolution
 * @param today
 * @param yeasterday
 * @return
 */
QHash< QString, int > KfttjControl::analysisJs(QString evolution, QDateTime today, QDateTime yeasterday){
    //存储降水量的hash(key:QString value:int -> 1:不可飞,2:限制可飞,3:可飞)
    QHash< QString,  int > jsHash;
    if(evolution.indexOf("RA", 0, Qt::CaseInsensitive) >= 0
            || evolution.indexOf("SN", 0, Qt::CaseInsensitive) >= 0){
        //去除降水量
        evolution = evolution.replace(QRegExp("\\([0-9]+\\)"), "");
        //替换开头的"--"为"-"
        if(evolution.startsWith("--")){
            evolution = evolution.mid(1);
        }

        //上一次天气现象(1:不可飞,2:限制可飞,3:可飞)
        int lastWeatherDegree = 0;
        //重组为字符串列表
        QStringList evoLst_1 = evolution.split(" ", QString::SkipEmptyParts);
        //解析字符串并将结果存入hash
        for(QString evo : evoLst_1){
            //先将"--"替换为"-*"
            evo = evo.replace("--", "-*");
            //再将开头的"-"替换为"*"
            if(evo.startsWith("-")){
                evo = evo.replace(0, 1, "*");
            }
            //判断是否是以降水开头的天气现象
            QRegExp startRegExp("[\\*\\+]?([a-zA-Z]*)(RA|SN)");
            startRegExp.setMinimal(true);
            int startPos = startRegExp.indexIn(evo);
            if(startPos == 0){
                //再以间隔线重组为字符串列表
                QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                for(QString element : elements){
                    startPos = startRegExp.indexIn(element);
                    if(startPos == 0){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            //给上一次天气现象赋值
                            if(element.startsWith("*")){
                                lastWeatherDegree = m_isMultiWeather ? 2 : 1;
                            }else if(element.startsWith("+")){
                                lastWeatherDegree = m_isMultiWeather ? 1 : 3;
                            }else{
                                lastWeatherDegree = m_isMultiWeather ? 1 : 2;
                            }
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //降水开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //降水开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //降水结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //降水结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }else{
                if(lastWeatherDegree > 0){
                    //再以间隔线重组为字符串列表
                    QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                    for(QString element : elements){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //降水开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //降水开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //降水结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //降水结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }

        }
    }
    return jsHash;
}

/**
 * @brief KfttjControl::analysisMultiNjy
 * 危险天气.浓积云
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiNjy(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("3");
    QList<QString> cloudStateList;
    cloudStateList.append(monthsummary.lowcloudstate1());
    cloudStateList.append(monthsummary.lowcloudstate2());
    cloudStateList.append(monthsummary.lowcloudstate3());
    cloudStateList.append(monthsummary.lowcloudstate4());
    cloudStateList.append(monthsummary.lowcloudstate5());
    cloudStateList.append(monthsummary.middlecloudstate1());
    cloudStateList.append(monthsummary.middlecloudstate2());
    cloudStateList.append(monthsummary.middlecloudstate3());
    cloudStateList.append(monthsummary.highcloudstate1());
    cloudStateList.append(monthsummary.highcloudstate2());
    cloudStateList.append(monthsummary.highcloudstate3());
    for(QString cloudState : cloudStateList){
        if(cloudState.indexOf("TCU", 0, Qt::CaseInsensitive) > -1){
            resultStr = QString("1");
            break;
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);

    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiJyy
 * 危险天气.积雨云
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiJyy(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("3");
    QList<QString> cloudStateList;
    cloudStateList.append(monthsummary.lowcloudstate1());
    cloudStateList.append(monthsummary.lowcloudstate2());
    cloudStateList.append(monthsummary.lowcloudstate3());
    cloudStateList.append(monthsummary.lowcloudstate4());
    cloudStateList.append(monthsummary.lowcloudstate5());
    cloudStateList.append(monthsummary.middlecloudstate1());
    cloudStateList.append(monthsummary.middlecloudstate2());
    cloudStateList.append(monthsummary.middlecloudstate3());
    cloudStateList.append(monthsummary.highcloudstate1());
    cloudStateList.append(monthsummary.highcloudstate2());
    cloudStateList.append(monthsummary.highcloudstate3());
    for(QString cloudState : cloudStateList){
        if(cloudState.indexOf("CB", 0, Qt::CaseInsensitive) > -1){
            resultStr = QString("1");
            break;
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);

    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiSd
 * 危险天气.闪电
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiSd(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    bool isFindJs = false;
    bool isMissing = true;
    if(index > -1){
        isMissing = false;
        Extremum extremum = extremumList[index];
        QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
        QDateTime yeasterday = today.addDays(-1);
        QHash< QString, int > jsHash;
        //天气演变列表
        QStringList evolutionList;
        evolutionList.append(extremum.evolution1());
        evolutionList.append(extremum.evolution2());
        evolutionList.append(extremum.evolution3());
        evolutionList.append(extremum.evolution4());
        evolutionList.append(extremum.evolution5());
        evolutionList.append(extremum.evolution6());
        evolutionList.append(extremum.evolution7());
        evolutionList.append(extremum.evolution8());
        evolutionList.append(extremum.evolution9());
        evolutionList.append(extremum.evolution10());

        for(QString evolution : evolutionList){
            jsHash = this->analysisLb(evolution, today, yeasterday);
            if(!jsHash.isEmpty()){
                QList<QString> keyList = jsHash.keys();
                for(QString timeStr : keyList){
                    if(timeStr.compare(monthsummary.datetime()) == 0){
                        if(jsHash[timeStr] != 0){
                            isFindJs = true;
                            resultStr = QString("%1").arg(jsHash[timeStr]);
                            break;
                        }
                    }
                }
            }
        }
    }
    if(!isFindJs){
        currentDateTime_utc = currentDateTime_utc.addDays(1);
        index = getExtremumIndex(currentDateTime_utc);
        if(index > -1){
            isMissing = false;
            Extremum extremum = extremumList[index];
            QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
            QDateTime yeasterday = today.addDays(-1);
            QHash< QString, int > jsHash;
            //天气演变列表
            QStringList evolutionList;
            evolutionList.append(extremum.evolution1());
            evolutionList.append(extremum.evolution2());
            evolutionList.append(extremum.evolution3());
            evolutionList.append(extremum.evolution4());
            evolutionList.append(extremum.evolution5());
            evolutionList.append(extremum.evolution6());
            evolutionList.append(extremum.evolution7());
            evolutionList.append(extremum.evolution8());
            evolutionList.append(extremum.evolution9());
            evolutionList.append(extremum.evolution10());

            for(QString evolution : evolutionList){
                jsHash = this->analysisLb(evolution, today, yeasterday);
                if(!jsHash.isEmpty()){
                    QList<QString> keyList = jsHash.keys();
                    for(QString timeStr : keyList){
                        if(timeStr.compare(monthsummary.datetime()) == 0){
                            if(jsHash[timeStr] != 0){
                                resultStr = QString("%1").arg(jsHash[timeStr]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if(isMissing){
        return resultStr;
    }else{
        if(resultStr.isEmpty()){
            resultStr = QString("3");
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);
    return resultStr;
}

/**
 * @brief KfttjControl::analysisMultiLb
 * 危险天气.雷暴
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiLb(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    bool isFindJs = false;
    bool isMissing = true;
    if(index > -1){
        isMissing = false;
        Extremum extremum = extremumList[index];
        QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
        QDateTime yeasterday = today.addDays(-1);
        QHash< QString, int > jsHash;
        //天气演变列表
        QStringList evolutionList;
        evolutionList.append(extremum.evolution1());
        evolutionList.append(extremum.evolution2());
        evolutionList.append(extremum.evolution3());
        evolutionList.append(extremum.evolution4());
        evolutionList.append(extremum.evolution5());
        evolutionList.append(extremum.evolution6());
        evolutionList.append(extremum.evolution7());
        evolutionList.append(extremum.evolution8());
        evolutionList.append(extremum.evolution9());
        evolutionList.append(extremum.evolution10());

        for(QString evolution : evolutionList){
            jsHash = this->analysisLb(evolution, today, yeasterday);
            if(!jsHash.isEmpty()){
                QList<QString> keyList = jsHash.keys();
                for(QString timeStr : keyList){
                    if(timeStr.compare(monthsummary.datetime()) == 0){
                        if(jsHash[timeStr] != 0){
                            isFindJs = true;
                            resultStr = QString("%1").arg(jsHash[timeStr]);
                            break;
                        }
                    }
                }
            }
        }
    }
    if(!isFindJs){
        currentDateTime_utc = currentDateTime_utc.addDays(1);
        index = getExtremumIndex(currentDateTime_utc);
        if(index > -1){
            isMissing = false;
            Extremum extremum = extremumList[index];
            QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
            QDateTime yeasterday = today.addDays(-1);
            QHash< QString, int > jsHash;
            //天气演变列表
            QStringList evolutionList;
            evolutionList.append(extremum.evolution1());
            evolutionList.append(extremum.evolution2());
            evolutionList.append(extremum.evolution3());
            evolutionList.append(extremum.evolution4());
            evolutionList.append(extremum.evolution5());
            evolutionList.append(extremum.evolution6());
            evolutionList.append(extremum.evolution7());
            evolutionList.append(extremum.evolution8());
            evolutionList.append(extremum.evolution9());
            evolutionList.append(extremum.evolution10());

            for(QString evolution : evolutionList){
                jsHash = this->analysisLb(evolution, today, yeasterday);
                if(!jsHash.isEmpty()){
                    QList<QString> keyList = jsHash.keys();
                    for(QString timeStr : keyList){
                        if(timeStr.compare(monthsummary.datetime()) == 0){
                            if(jsHash[timeStr] != 0){
                                resultStr = QString("%1").arg(jsHash[timeStr]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if(isMissing){
        return resultStr;
    }else{
        if(resultStr.isEmpty()){
            resultStr = QString("3");
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);
    return resultStr;
}

/**
 * @brief KfttjControl::analysisLb
 * 分析雷暴
 * @param evolution
 * @param today
 * @param yeasterday
 * @return
 */
QHash< QString, int > KfttjControl::analysisLb(QString evolution, QDateTime today, QDateTime yeasterday){
    //存储雷暴的hash(key:QString value:int -> 1:不可飞,2:限制可飞,3:可飞)
    QHash< QString,  int > jsHash;
    if(evolution.indexOf("TS", 0, Qt::CaseInsensitive) >= 0){
        //去除方位
        evolution = evolution.replace(QRegExp("([0-9]{4})[a-zA-Z]{1,2}"), "\\1");
        //替换开头的"--"为"-"
        if(evolution.startsWith("--")){
            evolution = evolution.mid(1);
        }

        //上一次天气现象(1:不可飞,2:限制可飞,3:可飞)
        int lastWeatherDegree = 0;
        //重组为字符串列表
        QStringList evoLst_1 = evolution.split(" ", QString::SkipEmptyParts);
        //解析字符串并将结果存入hash
        for(QString evo : evoLst_1){
            //先将"--"替换为"-*"
            evo = evo.replace("--", "-*");
            //再将开头的"-"替换为"*"
            if(evo.startsWith("-")){
                evo = evo.replace(0, 1, "*");
            }
            //判断是否是以雷暴开头的天气现象
            QRegExp startRegExp("[\\*\\+]?(TS)");
            startRegExp.setMinimal(true);
            int startPos = startRegExp.indexIn(evo);
            if(startPos == 0){
                //再以间隔线重组为字符串列表
                QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                for(QString element : elements){
                    startPos = startRegExp.indexIn(element);
                    if(startPos == 0){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            //给上一次天气现象赋值
                            lastWeatherDegree = 1;
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //雷暴开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //雷暴开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //雷暴结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //雷暴结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }else{
                if(lastWeatherDegree > 0){
                    //再以间隔线重组为字符串列表
                    QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                    for(QString element : elements){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //雷暴开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //雷暴开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //雷暴结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //雷暴结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }

        }
    }
    return jsHash;
}


/**
 * @brief KfttjControl::analysisMultiBb
 * 危险天气.冰雹
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiBb(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    bool isFindJs = false;
    bool isMissing = true;
    if(index > -1){
        isMissing = false;
        Extremum extremum = extremumList[index];
        QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
        QDateTime yeasterday = today.addDays(-1);
        QHash< QString, int > jsHash;
        //天气演变列表
        QStringList evolutionList;
        evolutionList.append(extremum.evolution1());
        evolutionList.append(extremum.evolution2());
        evolutionList.append(extremum.evolution3());
        evolutionList.append(extremum.evolution4());
        evolutionList.append(extremum.evolution5());
        evolutionList.append(extremum.evolution6());
        evolutionList.append(extremum.evolution7());
        evolutionList.append(extremum.evolution8());
        evolutionList.append(extremum.evolution9());
        evolutionList.append(extremum.evolution10());

        for(QString evolution : evolutionList){
            jsHash = this->analysisBb(evolution, today, yeasterday);
            if(!jsHash.isEmpty()){
                QList<QString> keyList = jsHash.keys();
                for(QString timeStr : keyList){
                    if(timeStr.compare(monthsummary.datetime()) == 0){
                        if(jsHash[timeStr] != 0){
                            isFindJs = true;
                            resultStr = QString("%1").arg(jsHash[timeStr]);
                            break;
                        }
                    }
                }
            }
        }
    }
    if(!isFindJs){
        currentDateTime_utc = currentDateTime_utc.addDays(1);
        index = getExtremumIndex(currentDateTime_utc);
        if(index > -1){
            isMissing = false;
            Extremum extremum = extremumList[index];
            QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
            QDateTime yeasterday = today.addDays(-1);
            QHash< QString, int > jsHash;
            //天气演变列表
            QStringList evolutionList;
            evolutionList.append(extremum.evolution1());
            evolutionList.append(extremum.evolution2());
            evolutionList.append(extremum.evolution3());
            evolutionList.append(extremum.evolution4());
            evolutionList.append(extremum.evolution5());
            evolutionList.append(extremum.evolution6());
            evolutionList.append(extremum.evolution7());
            evolutionList.append(extremum.evolution8());
            evolutionList.append(extremum.evolution9());
            evolutionList.append(extremum.evolution10());

            for(QString evolution : evolutionList){
                jsHash = this->analysisBb(evolution, today, yeasterday);
                if(!jsHash.isEmpty()){
                    QList<QString> keyList = jsHash.keys();
                    for(QString timeStr : keyList){
                        if(timeStr.compare(monthsummary.datetime()) == 0){
                            if(jsHash[timeStr] != 0){
                                resultStr = QString("%1").arg(jsHash[timeStr]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if(isMissing){
        return resultStr;
    }else{
        if(resultStr.isEmpty()){
            resultStr = QString("3");
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);
    return resultStr;
}

/**
 * @brief KfttjControl::analysisBb
 * 分析冰雹
 * @param evolution
 * @param today
 * @param yeasterday
 * @return
 */
QHash< QString, int > KfttjControl::analysisBb(QString evolution, QDateTime today, QDateTime yeasterday){
    //存储冰雹的hash(key:QString value:int -> 1:不可飞,2:限制可飞,3:可飞)
    QHash< QString,  int > jsHash;
    if(evolution.indexOf("GR", 0, Qt::CaseInsensitive) >= 0
            || evolution.indexOf("GS", 0, Qt::CaseInsensitive) >= 0){
        //去除方位
        evolution = evolution.replace(QRegExp("([0-9]{4})[a-zA-Z]{1,2}"), "\\1");
        //替换开头的"--"为"-"
        if(evolution.startsWith("--")){
            evolution = evolution.mid(1);
        }

        //上一次天气现象(1:不可飞,2:限制可飞,3:可飞)
        int lastWeatherDegree = 0;
        //重组为字符串列表
        QStringList evoLst_1 = evolution.split(" ", QString::SkipEmptyParts);
        //解析字符串并将结果存入hash
        for(QString evo : evoLst_1){
            //先将"--"替换为"-*"
            evo = evo.replace("--", "-*");
            //再将开头的"-"替换为"*"
            if(evo.startsWith("-")){
                evo = evo.replace(0, 1, "*");
            }
            //判断是否是以冰雹开头的天气现象
            QRegExp startRegExp("[\\*\\+]?(GR|GS)");
            startRegExp.setMinimal(true);
            int startPos = startRegExp.indexIn(evo);
            if(startPos == 0){
                //再以间隔线重组为字符串列表
                QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                for(QString element : elements){
                    startPos = startRegExp.indexIn(element);
                    if(startPos == 0){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            //给上一次天气现象赋值
                            lastWeatherDegree = 1;
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //冰雹开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //冰雹开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //冰雹结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //冰雹结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }else{
                if(lastWeatherDegree > 0){
                    //再以间隔线重组为字符串列表
                    QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                    for(QString element : elements){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //冰雹开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //冰雹开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //冰雹结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //冰雹结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }

        }
    }
    return jsHash;
}

/**
 * @brief KfttjControl::analysisMultiBx
 * 危险天气.飑线
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiBx(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    bool isFindJs = false;
    bool isMissing = true;
    if(index > -1){
        isMissing = false;
        Extremum extremum = extremumList[index];
        QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
        QDateTime yeasterday = today.addDays(-1);
        QHash< QString, int > jsHash;
        //天气演变列表
        QStringList evolutionList;
        evolutionList.append(extremum.evolution1());
        evolutionList.append(extremum.evolution2());
        evolutionList.append(extremum.evolution3());
        evolutionList.append(extremum.evolution4());
        evolutionList.append(extremum.evolution5());
        evolutionList.append(extremum.evolution6());
        evolutionList.append(extremum.evolution7());
        evolutionList.append(extremum.evolution8());
        evolutionList.append(extremum.evolution9());
        evolutionList.append(extremum.evolution10());

        for(QString evolution : evolutionList){
            jsHash = this->analysisBx(evolution, today, yeasterday);
            if(!jsHash.isEmpty()){
                QList<QString> keyList = jsHash.keys();
                for(QString timeStr : keyList){
                    if(timeStr.compare(monthsummary.datetime()) == 0){
                        if(jsHash[timeStr] != 0){
                            isFindJs = true;
                            resultStr = QString("%1").arg(jsHash[timeStr]);
                            break;
                        }
                    }
                }
            }
        }
    }
    if(!isFindJs){
        currentDateTime_utc = currentDateTime_utc.addDays(1);
        index = getExtremumIndex(currentDateTime_utc);
        if(index > -1){
            isMissing = false;
            Extremum extremum = extremumList[index];
            QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
            QDateTime yeasterday = today.addDays(-1);
            QHash< QString, int > jsHash;
            //天气演变列表
            QStringList evolutionList;
            evolutionList.append(extremum.evolution1());
            evolutionList.append(extremum.evolution2());
            evolutionList.append(extremum.evolution3());
            evolutionList.append(extremum.evolution4());
            evolutionList.append(extremum.evolution5());
            evolutionList.append(extremum.evolution6());
            evolutionList.append(extremum.evolution7());
            evolutionList.append(extremum.evolution8());
            evolutionList.append(extremum.evolution9());
            evolutionList.append(extremum.evolution10());

            for(QString evolution : evolutionList){
                jsHash = this->analysisBx(evolution, today, yeasterday);
                if(!jsHash.isEmpty()){
                    QList<QString> keyList = jsHash.keys();
                    for(QString timeStr : keyList){
                        if(timeStr.compare(monthsummary.datetime()) == 0){
                            if(jsHash[timeStr] != 0){
                                resultStr = QString("%1").arg(jsHash[timeStr]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if(isMissing){
        return resultStr;
    }else{
        if(resultStr.isEmpty()){
            resultStr = QString("3");
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);
    return resultStr;
}

/**
 * @brief KfttjControl::analysisBx
 * 分析飑线
 * @param evolution
 * @param today
 * @param yeasterday
 * @return
 */
QHash< QString, int > KfttjControl::analysisBx(QString evolution, QDateTime today, QDateTime yeasterday){
    //存储飑线的hash(key:QString value:int -> 1:不可飞,2:限制可飞,3:可飞)
    QHash< QString,  int > jsHash;
    if(evolution.indexOf("SQ", 0, Qt::CaseInsensitive) >= 0){
        //去除方位
        evolution = evolution.replace(QRegExp("([0-9]{4})[a-zA-Z]{1,2}"), "\\1");
        //替换开头的"--"为"-"
        if(evolution.startsWith("--")){
            evolution = evolution.mid(1);
        }

        //上一次天气现象(1:不可飞,2:限制可飞,3:可飞)
        int lastWeatherDegree = 0;
        //重组为字符串列表
        QStringList evoLst_1 = evolution.split(" ", QString::SkipEmptyParts);
        //解析字符串并将结果存入hash
        for(QString evo : evoLst_1){
            //先将"--"替换为"-*"
            evo = evo.replace("--", "-*");
            //再将开头的"-"替换为"*"
            if(evo.startsWith("-")){
                evo = evo.replace(0, 1, "*");
            }
            //判断是否是以飑线开头的天气现象
            QRegExp startRegExp("[\\*\\+]?(SQ)");
            startRegExp.setMinimal(true);
            int startPos = startRegExp.indexIn(evo);
            if(startPos == 0){
                //再以间隔线重组为字符串列表
                QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                for(QString element : elements){
                    startPos = startRegExp.indexIn(element);
                    if(startPos == 0){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            //给上一次天气现象赋值
                            lastWeatherDegree = 1;
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //飑线开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //飑线开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //飑线结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //飑线结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }else{
                if(lastWeatherDegree > 0){
                    //再以间隔线重组为字符串列表
                    QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                    for(QString element : elements){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //飑线开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //飑线开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //飑线结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //飑线结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }

        }
    }
    return jsHash;
}

/**
 * @brief KfttjControl::analysisMultiLj
 * 危险天气.龙卷
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiLj(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    bool isFindJs = false;
    bool isMissing = true;
    if(index > -1){
        isMissing = false;
        Extremum extremum = extremumList[index];
        QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
        QDateTime yeasterday = today.addDays(-1);
        QHash< QString, int > jsHash;
        //天气演变列表
        QStringList evolutionList;
        evolutionList.append(extremum.evolution1());
        evolutionList.append(extremum.evolution2());
        evolutionList.append(extremum.evolution3());
        evolutionList.append(extremum.evolution4());
        evolutionList.append(extremum.evolution5());
        evolutionList.append(extremum.evolution6());
        evolutionList.append(extremum.evolution7());
        evolutionList.append(extremum.evolution8());
        evolutionList.append(extremum.evolution9());
        evolutionList.append(extremum.evolution10());

        for(QString evolution : evolutionList){
            jsHash = this->analysisLj(evolution, today, yeasterday);
            if(!jsHash.isEmpty()){
                QList<QString> keyList = jsHash.keys();
                for(QString timeStr : keyList){
                    if(timeStr.compare(monthsummary.datetime()) == 0){
                        if(jsHash[timeStr] != 0){
                            isFindJs = true;
                            resultStr = QString("%1").arg(jsHash[timeStr]);
                            break;
                        }
                    }
                }
            }
        }
    }
    if(!isFindJs){
        currentDateTime_utc = currentDateTime_utc.addDays(1);
        index = getExtremumIndex(currentDateTime_utc);
        if(index > -1){
            isMissing = false;
            Extremum extremum = extremumList[index];
            QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
            QDateTime yeasterday = today.addDays(-1);
            QHash< QString, int > jsHash;
            //天气演变列表
            QStringList evolutionList;
            evolutionList.append(extremum.evolution1());
            evolutionList.append(extremum.evolution2());
            evolutionList.append(extremum.evolution3());
            evolutionList.append(extremum.evolution4());
            evolutionList.append(extremum.evolution5());
            evolutionList.append(extremum.evolution6());
            evolutionList.append(extremum.evolution7());
            evolutionList.append(extremum.evolution8());
            evolutionList.append(extremum.evolution9());
            evolutionList.append(extremum.evolution10());

            for(QString evolution : evolutionList){
                jsHash = this->analysisLj(evolution, today, yeasterday);
                if(!jsHash.isEmpty()){
                    QList<QString> keyList = jsHash.keys();
                    for(QString timeStr : keyList){
                        if(timeStr.compare(monthsummary.datetime()) == 0){
                            if(jsHash[timeStr] != 0){
                                resultStr = QString("%1").arg(jsHash[timeStr]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if(isMissing){
        return resultStr;
    }else{
        if(resultStr.isEmpty()){
            resultStr = QString("3");
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);
    return resultStr;
}

/**
 * @brief KfttjControl::analysisLj
 * 分析龙卷
 * @param evolution
 * @param today
 * @param yeasterday
 * @return
 */
QHash< QString, int > KfttjControl::analysisLj(QString evolution, QDateTime today, QDateTime yeasterday){
    //存储龙卷的hash(key:QString value:int -> 1:不可飞,2:限制可飞,3:可飞)
    QHash< QString,  int > jsHash;
    if(evolution.indexOf("FC", 0, Qt::CaseInsensitive) >= 0){
        //去除方位
        evolution = evolution.replace(QRegExp("([0-9]{4})[a-zA-Z]{1,2}"), "\\1");
        //替换开头的"--"为"-"
        if(evolution.startsWith("--")){
            evolution = evolution.mid(1);
        }

        //上一次天气现象(1:不可飞,2:限制可飞,3:可飞)
        int lastWeatherDegree = 0;
        //重组为字符串列表
        QStringList evoLst_1 = evolution.split(" ", QString::SkipEmptyParts);
        //解析字符串并将结果存入hash
        for(QString evo : evoLst_1){
            //先将"--"替换为"-*"
            evo = evo.replace("--", "-*");
            //再将开头的"-"替换为"*"
            if(evo.startsWith("-")){
                evo = evo.replace(0, 1, "*");
            }
            //判断是否是以龙卷开头的天气现象
            QRegExp startRegExp("[\\*\\+]?(FC)");
            startRegExp.setMinimal(true);
            int startPos = startRegExp.indexIn(evo);
            if(startPos == 0){
                //再以间隔线重组为字符串列表
                QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                for(QString element : elements){
                    startPos = startRegExp.indexIn(element);
                    if(startPos == 0){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            //给上一次天气现象赋值
                            lastWeatherDegree = 1;
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //龙卷开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //龙卷开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //龙卷结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //龙卷结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }else{
                if(lastWeatherDegree > 0){
                    //再以间隔线重组为字符串列表
                    QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                    for(QString element : elements){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //龙卷开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //龙卷开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //龙卷结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //龙卷结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }

        }
    }
    return jsHash;
}

/**
 * @brief KfttjControl::analysisMultiScb
 * 危险天气.沙尘暴
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiScb(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    bool isFindJs = false;
    bool isMissing = true;
    if(index > -1){
        isMissing = false;
        Extremum extremum = extremumList[index];
        QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
        QDateTime yeasterday = today.addDays(-1);
        QHash< QString, int > jsHash;
        //天气演变列表
        QStringList evolutionList;
        evolutionList.append(extremum.evolution1());
        evolutionList.append(extremum.evolution2());
        evolutionList.append(extremum.evolution3());
        evolutionList.append(extremum.evolution4());
        evolutionList.append(extremum.evolution5());
        evolutionList.append(extremum.evolution6());
        evolutionList.append(extremum.evolution7());
        evolutionList.append(extremum.evolution8());
        evolutionList.append(extremum.evolution9());
        evolutionList.append(extremum.evolution10());

        for(QString evolution : evolutionList){
            jsHash = this->analysisScb(evolution, today, yeasterday);
            if(!jsHash.isEmpty()){
                QList<QString> keyList = jsHash.keys();
                for(QString timeStr : keyList){
                    if(timeStr.compare(monthsummary.datetime()) == 0){
                        if(jsHash[timeStr] != 0){
                            isFindJs = true;
                            resultStr = QString("%1").arg(jsHash[timeStr]);
                            break;
                        }
                    }
                }
            }
        }
    }
    if(!isFindJs){
        currentDateTime_utc = currentDateTime_utc.addDays(1);
        index = getExtremumIndex(currentDateTime_utc);
        if(index > -1){
            isMissing = false;
            Extremum extremum = extremumList[index];
            QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
            QDateTime yeasterday = today.addDays(-1);
            QHash< QString, int > jsHash;
            //天气演变列表
            QStringList evolutionList;
            evolutionList.append(extremum.evolution1());
            evolutionList.append(extremum.evolution2());
            evolutionList.append(extremum.evolution3());
            evolutionList.append(extremum.evolution4());
            evolutionList.append(extremum.evolution5());
            evolutionList.append(extremum.evolution6());
            evolutionList.append(extremum.evolution7());
            evolutionList.append(extremum.evolution8());
            evolutionList.append(extremum.evolution9());
            evolutionList.append(extremum.evolution10());

            for(QString evolution : evolutionList){
                jsHash = this->analysisScb(evolution, today, yeasterday);
                if(!jsHash.isEmpty()){
                    QList<QString> keyList = jsHash.keys();
                    for(QString timeStr : keyList){
                        if(timeStr.compare(monthsummary.datetime()) == 0){
                            if(jsHash[timeStr] != 0){
                                resultStr = QString("%1").arg(jsHash[timeStr]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if(isMissing){
        return resultStr;
    }else{
        if(resultStr.isEmpty()){
            resultStr = QString("3");
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);
    return resultStr;
}

/**
 * @brief KfttjControl::analysisScb
 * 分析沙尘暴
 * @param evolution
 * @param today
 * @param yeasterday
 * @return
 */
QHash< QString, int > KfttjControl::analysisScb(QString evolution, QDateTime today, QDateTime yeasterday){
    //存储沙尘暴的hash(key:QString value:int -> 1:不可飞,2:限制可飞,3:可飞)
    QHash< QString,  int > jsHash;
    if(evolution.indexOf("SS", 0, Qt::CaseInsensitive) >= 0
            || evolution.indexOf("DS", 0, Qt::CaseInsensitive) >= 0){
        //去除方位
        evolution = evolution.replace(QRegExp("([0-9]{4})[a-zA-Z]{1,2}"), "\\1");
        //替换开头的"--"为"-"
        if(evolution.startsWith("--")){
            evolution = evolution.mid(1);
        }

        //上一次天气现象(1:不可飞,2:限制可飞,3:可飞)
        int lastWeatherDegree = 0;
        //重组为字符串列表
        QStringList evoLst_1 = evolution.split(" ", QString::SkipEmptyParts);
        //解析字符串并将结果存入hash
        for(QString evo : evoLst_1){
            //先将"--"替换为"-*"
            evo = evo.replace("--", "-*");
            //再将开头的"-"替换为"*"
            if(evo.startsWith("-")){
                evo = evo.replace(0, 1, "*");
            }
            //判断是否是以沙尘暴开头的天气现象
            QRegExp startRegExp("[\\*\\+]?(SS|DS|SA)");
            startRegExp.setMinimal(true);
            int startPos = startRegExp.indexIn(evo);
            if(startPos == 0){
                //再以间隔线重组为字符串列表
                QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                for(QString element : elements){
                    startPos = startRegExp.indexIn(element);
                    if(startPos == 0){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            //给上一次天气现象赋值
                            if(element.indexOf("SA") >= 0){
                                lastWeatherDegree = 3;
                            }else{
                                lastWeatherDegree = 1;
                            }
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //沙尘暴开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //沙尘暴开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //沙尘暴结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //沙尘暴结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }else{
                if(lastWeatherDegree > 0){
                    //再以间隔线重组为字符串列表
                    QList<QString> elements = evo.split("-", QString::SkipEmptyParts);
                    for(QString element : elements){
                        //查找时间(hhmm)
                        QRegExp regExp("([0-9]{4})");
                        regExp.setMinimal(true);
                        int pos = regExp.indexIn(element, 0);
                        if(pos != -1){
                            //获取时间
                            QString hhmmStr = regExp.cap(1);
                            int hour = hhmmStr.mid(0, 2).toInt();
                            int minute = hhmmStr.mid(2, 2).toInt();
                            //计算开始时间
                            QDateTime startDate;
                            //如果是16点以后说明是上一天的记录
                            if(hour >= 16){
                                QString yeasterdayStr = QString("%1T%2:00")
                                        .arg(yeasterday.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                //沙尘暴开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }else{
                                QString todayStr = QString("%1T%2:00")
                                        .arg(today.toString("yyyy-MM-dd"))
                                        .arg(hhmmStr);
                                startDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                //沙尘暴开始往后推
                                if(minute > 0){
                                    QString startDateStr = startDate.toString("yyyy-MM-ddThh:00:00");
                                    startDate = QDateTime::fromString(startDateStr, "yyyy-MM-ddThh:mm:ss");
                                    startDate = startDate.addSecs(3600);
                                }
                            }
                            //计算结束时间
                            QDateTime endDate = startDate;
                            int elementCount = elements.count();
                            QString finalElement = elements[elementCount - 1];
                            pos = regExp.indexIn(finalElement, 0);
                            if(pos != -1){
                                //获取时间
                                hhmmStr = regExp.cap(1);
                                hour = hhmmStr.mid(0, 2).toInt();
                                minute = hhmmStr.mid(2, 2).toInt();

                                //如果是16点以后说明是上一天的记录
                                if(hour >= 16){
                                    QString yeasterdayStr = QString("%1T%2:00")
                                            .arg(yeasterday.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(yeasterdayStr, "yyyy-MM-ddThhmm:ss");
                                }else{
                                    QString todayStr = QString("%1T%2:00")
                                            .arg(today.toString("yyyy-MM-dd"))
                                            .arg(hhmmStr);
                                    endDate = QDateTime::fromString(todayStr, "yyyy-MM-ddThhmm:ss");
                                }

                                if(evo.endsWith("-")){
                                    //沙尘暴结束往后推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                    if(minute > 0){
                                        endDate = endDate.addSecs(3600);
                                    }
                                }else{
                                    //沙尘暴结束往前推
                                    QString endDateStr = endDate.toString("yyyy-MM-ddThh:00:00");
                                    endDate = QDateTime::fromString(endDateStr, "yyyy-MM-ddThh:mm:ss");
                                }
                            }
                            //给hash赋值
                            while(startDate.secsTo(endDate) >= 0){
                                jsHash[startDate.toString("yyyy-MM-ddThh:mm:ss")] = lastWeatherDegree;
                                startDate = startDate.addSecs(3600);
                            }
                        }
                    }
                }
            }

        }
    }
    return jsHash;
}

/**
 * @brief KfttjControl::analysisMultiFqb
 * 危险天气.风切变
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisMultiFqb(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("3");
    QString phenomena = monthsummary.phenomena();
    if(phenomena.indexOf("WS", 0, Qt::CaseInsensitive) > -1){
        resultStr = QString("1");
    }
    emit sendMessage(resultStr, row, col, 1, 1);

    return resultStr;
}

/**
 * @brief KfttjControl::analysisSingleSf
 * 风.强风模式.矢量风.顺风
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisSingleSf(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 21){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QString windspeedStr = monthsummary.windspeed().trimmed();
    QString gustspeedStr = monthsummary.gustspeed().trimmed();
    QString winddirectionStr = monthsummary.winddirection().trimmed();
    if(windspeedStr.compare("") != 0 || gustspeedStr.compare("") != 0){
        float windspeed = windspeedStr.toFloat();
        float gustspeed = gustspeedStr.toFloat();
        float speed = qMax(windspeed, gustspeed);
        float tailspeed = speed;
        if(winddirectionStr.compare("C") != 0 && winddirectionStr.compare("VRB") != 0){
            float winddirection = winddirectionStr.toFloat();
            winddirection = winddirection * PI / 180;
            tailspeed = qCos(winddirection - m_runwayDirection) * speed;
        }
        //顺风
        if(isMatchArgs(-tailspeed, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(-tailspeed, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(-tailspeed, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisSingleNf
 * 风.强风模式.矢量风.逆风
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisSingleNf(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 22){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QString windspeedStr = monthsummary.windspeed().trimmed();
    QString gustspeedStr = monthsummary.gustspeed().trimmed();
    QString winddirectionStr = monthsummary.winddirection().trimmed();
    if(windspeedStr.compare("") != 0 || gustspeedStr.compare("") != 0){
        float windspeed = windspeedStr.toFloat();
        float gustspeed = gustspeedStr.toFloat();
        float speed = qMax(windspeed, gustspeed);
        float headspeed = speed;
        if(winddirectionStr.compare("C") != 0 && winddirectionStr.compare("VRB") != 0){
            float winddirection = winddirectionStr.toFloat();
            winddirection = winddirection * PI / 180;
            headspeed = qCos(winddirection - m_runwayDirection) * speed;
        }
        //逆风
        if(isMatchArgs(headspeed, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(headspeed, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(headspeed, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisSingleCf
 * 风.强风模式.矢量风.侧风
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisSingleCf(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 23){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QString windspeedStr = monthsummary.windspeed().trimmed();
    QString gustspeedStr = monthsummary.gustspeed().trimmed();
    QString winddirectionStr = monthsummary.winddirection().trimmed();
    if(windspeedStr.compare("") != 0 || gustspeedStr.compare("") != 0){
        float windspeed = windspeedStr.toFloat();
        float gustspeed = gustspeedStr.toFloat();
        float speed = qMax(windspeed, gustspeed);
        float crossspeed = speed;
        if(winddirectionStr.compare("C") != 0 && winddirectionStr.compare("VRB") != 0){
            float winddirection = winddirectionStr.toFloat();
            winddirection = winddirection * PI / 180;
            crossspeed = qAbs(qSin(winddirection - m_runwayDirection) * speed);
        }
        //侧风
        if(isMatchArgs(crossspeed, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(crossspeed, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(crossspeed, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisSingleBlf
 * 风.强风模式.标量风
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisSingleBlf(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 24){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QString windspeedStr = monthsummary.windspeed().trimmed();
    QString gustspeedStr = monthsummary.gustspeed().trimmed();
    if(windspeedStr.compare("") != 0 || gustspeedStr.compare("") != 0){
        float windspeed = windspeedStr.toFloat();
        float gustspeed = gustspeedStr.toFloat();
        float speed = qMax(windspeed, gustspeed);
        //标量风
        if(isMatchArgs(speed, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(speed, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(speed, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisSingleJgw
 * 温度.极高温
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisSingleJgw(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 25){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    if(index > -1){
        Extremum extremum = extremumList[index];
        float temperature = extremum.hightemperature().toFloat();
        //常规温
        if(isMatchArgs(temperature, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(temperature, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(temperature, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisSingleJdw
 * 温度.极低温
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisSingleJdw(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");

    QList<QString> limitList;
    for(WeatherParamSetup wpSetup : weatherParamSetupList){
        if(wpSetup.paramid() == 26){
            limitList = this->getDataFromJson(wpSetup.limits());
            break;
        }
    }

    if(limitList.count() != 6){
        return resultStr;
    }

    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    if(index > -1){
        Extremum extremum = extremumList[index];
        float temperature = extremum.lowtemperature().toFloat();
        //常规温
        if(isMatchArgs(temperature, limitList[0], limitList[1])){
            resultStr = QString("1");
        }else if(isMatchArgs(temperature, limitList[2], limitList[3])){
            resultStr = QString("2");
        }else if(isMatchArgs(temperature, limitList[4], limitList[5])){
            resultStr = QString("3");
        }else{

        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    return resultStr;
}

/**
 * @brief KfttjControl::analysisSingleQjs
 * 降水.强降水
 * @param monthsummary
 * @param row
 * @param col
 * @return
 */
QString KfttjControl::analysisSingleQjs(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int index = getExtremumIndex(currentDateTime_utc);
    bool isFindJs = false;
    bool isMissing = true;
    if(index > -1){
        isMissing = false;
        Extremum extremum = extremumList[index];
        QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
        QDateTime yeasterday = today.addDays(-1);
        QHash< QString, int > jsHash;
        //天气演变列表
        QStringList evolutionList;
        evolutionList.append(extremum.evolution1());
        evolutionList.append(extremum.evolution2());
        evolutionList.append(extremum.evolution3());
        evolutionList.append(extremum.evolution4());
        evolutionList.append(extremum.evolution5());
        evolutionList.append(extremum.evolution6());
        evolutionList.append(extremum.evolution7());
        evolutionList.append(extremum.evolution8());
        evolutionList.append(extremum.evolution9());
        evolutionList.append(extremum.evolution10());

        for(QString evolution : evolutionList){
            jsHash = this->analysisJs(evolution, today, yeasterday);
            if(!jsHash.isEmpty()){
                QList<QString> keyList = jsHash.keys();
                for(QString timeStr : keyList){
                    if(timeStr.compare(monthsummary.datetime()) == 0){
                        if(jsHash[timeStr] != 0){
                            isFindJs = true;
                            resultStr = QString("%1").arg(jsHash[timeStr]);
                            break;
                        }
                    }
                }
            }
        }
    }
    if(!isFindJs){
        currentDateTime_utc = currentDateTime_utc.addDays(1);
        index = getExtremumIndex(currentDateTime_utc);
        if(index > -1){
            isMissing = false;
            Extremum extremum = extremumList[index];
            QDateTime today = QDateTime::fromString(extremum.datetime(), "yyyy-MM-ddThh:mm:ss");
            QDateTime yeasterday = today.addDays(-1);
            QHash< QString, int > jsHash;
            //天气演变列表
            QStringList evolutionList;
            evolutionList.append(extremum.evolution1());
            evolutionList.append(extremum.evolution2());
            evolutionList.append(extremum.evolution3());
            evolutionList.append(extremum.evolution4());
            evolutionList.append(extremum.evolution5());
            evolutionList.append(extremum.evolution6());
            evolutionList.append(extremum.evolution7());
            evolutionList.append(extremum.evolution8());
            evolutionList.append(extremum.evolution9());
            evolutionList.append(extremum.evolution10());

            for(QString evolution : evolutionList){
                jsHash = this->analysisJs(evolution, today, yeasterday);
                if(!jsHash.isEmpty()){
                    QList<QString> keyList = jsHash.keys();
                    for(QString timeStr : keyList){
                        if(timeStr.compare(monthsummary.datetime()) == 0){
                            if(jsHash[timeStr] != 0){
                                resultStr = QString("%1").arg(jsHash[timeStr]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if(isMissing){
        return resultStr;
    }else{
        if(resultStr.isEmpty()){
            resultStr = QString("1");
        }
    }
    emit sendMessage(resultStr, row, col, 1, 1);
    return resultStr;
}

/**
 * @brief KfttjControl::analysisAll
 * 综合所有气象要素进行分析
 * @return
 */
QString KfttjControl::analysisAll(QDateTime currentDateTime_local, QStringList results, int row, int col){
    QList<QString> elementList;
    for(WeatherParam weatherParam : m_wpList){
        elementList.append(weatherParam.name());
    }
    QString key = currentDateTime_local.toString("yyyy-MM-dd");
    QStringList xzkfEffectList;
    QStringList bkfEffectList;
    QString resultStr("");
    int resultCount = results.size();
    for(int i = 0;i < resultCount;i++){
        QString resStr = results[i];
        if(resStr.compare("2") == 0){
            xzkfEffectList.append(elementList[i]);
        }else if(resStr.compare("1") == 0){
            bkfEffectList.append(elementList[i]);
        }
        if(resStr.compare("") == 0){
            resultStr = resStr;
            break;
        }else{
            if(resultStr.compare("") == 0){
                resultStr = resStr;
            }else{
                if(resultStr.compare(resStr) > 0){
                    resultStr = resStr;
                }
            }
        }
    }
    if(resultStr.compare("") != 0){
        if(resultStr.compare("2") == 0){
            if(effectHash.contains(key)){
                QList<QStringList> effectList = effectHash[key];
                QStringList xzkfEffectList_g = effectList[0];
                int count = xzkfEffectList.size();
                for(int i = 0;i < count;i++){
                    if(xzkfEffectList_g.indexOf(xzkfEffectList[i]) < 0){
                        xzkfEffectList_g.append(xzkfEffectList[i]);
                    }
                }
                effectList[0] = xzkfEffectList_g;
                effectHash[key] = effectList;
            }else{
                QList<QStringList> effectList;
                effectList.append(xzkfEffectList);
                QStringList blankList;
                effectList.append(blankList);
                effectHash[key] = effectList;
            }
        }else if(resultStr.compare("1") == 0){
            if(effectHash.contains(key)){
                QList<QStringList> effectList = effectHash[key];
                QStringList bkfEffectList_g = effectList[1];
                int count = bkfEffectList.size();
                for(int i = 0;i < count;i++){
                    if(bkfEffectList_g.indexOf(bkfEffectList[i]) < 0){
                        bkfEffectList_g.append(bkfEffectList[i]);
                    }
                }
                effectList[1] = bkfEffectList_g;
                effectHash[key] = effectList;
            }else{
                QList<QStringList> effectList;
                QStringList blankList;
                effectList.append(blankList);
                effectList.append(bkfEffectList);
                effectHash[key] = effectList;
            }
        }
        emit sendMessage(resultStr, row, col, 1, 1);
    }

    //日可飞天统计添加
    resAll.append(resultStr.toInt());

    return resultStr;
}

void KfttjControl::analysisDay(QDateTime lastDateTime_local, int row){
    //初始化出图用的hash表
    QString kfttjKey = lastDateTime_local.toString("yyyy-MM-dd");
    QList<float> kfttjValue;
    kfttjValue.append(0);
    kfttjValue.append(0);
    kfttjValue.append(0);
    if(m_isMultiWeather){
        //取得对应月份开始统计的位置
        int month = lastDateTime_local.toString("M").toInt();
        int startIndex = 0;
        switch (month) {
        case 1:
            startIndex = m_titleList.indexOf(QString("%1").arg(jan_day[0])) - 2;
            break;
        case 2:
            startIndex = m_titleList.indexOf(QString("%1").arg(feb_day[0])) - 2;
            break;
        case 3:
            startIndex = m_titleList.indexOf(QString("%1").arg(mar_day[0])) - 2;
            break;
        case 4:
            startIndex = m_titleList.indexOf(QString("%1").arg(apr_day[0])) - 2;
            break;
        case 5:
            startIndex = m_titleList.indexOf(QString("%1").arg(may_day[0])) - 2;
            break;
        case 6:
            startIndex = m_titleList.indexOf(QString("%1").arg(jun_day[0])) - 2;
            break;
        case 7:
            startIndex = m_titleList.indexOf(QString("%1").arg(jul_day[0])) - 2;
            break;
        case 8:
            startIndex = m_titleList.indexOf(QString("%1").arg(aug_day[0])) - 2;
            break;
        case 9:
            startIndex = m_titleList.indexOf(QString("%1").arg(sep_day[0])) - 2;
            break;
        case 10:
            startIndex = m_titleList.indexOf(QString("%1").arg(oct_day[0])) - 2;
            break;
        case 11:
            startIndex = m_titleList.indexOf(QString("%1").arg(nov_day[0])) - 2;
            break;
        case 12:
            startIndex = m_titleList.indexOf(QString("%1").arg(dec_day[0])) - 2;
            break;
        default:
            break;
        }
        //取得半天及整天所在的位置
        int halfIndex = m_titleList.indexOf(QString("%1").arg(half_day)) - 2;
        int wholeIndex = m_titleList.indexOf(QString("%1").arg(whole_day)) - 2;

        //判断是采用半天的统计算法还是整天的统计算法
        int resCount = resAll.size();
        QStringList valueStrList;
        for(int i = startIndex;i < resCount;i++){
            int val = resAll[i];
            if(val == 0){
                break;
            }else{
                valueStrList.append(QString("%1").arg(val));
            }
        }
        int minHalfCount = halfIndex - startIndex + 1;
        int minWholeCount = wholeIndex - startIndex + 1;
        int valueCount = valueStrList.size();
        QString valueStr = valueStrList.join("");

        if(valueCount < minHalfCount){
            //缺测
            kfttjHash[kfttjKey] = kfttjValue;
        }else if(valueCount >= minHalfCount && valueCount < minWholeCount){
            //半天统计
            /*
             * 1.存在一个连续4~6小时均为“完全可飞”的时段，记“0.5个可飞天”
             * 2.存在一个连续4~6小时均不出现“不可飞”的时段，同时又不满足“0.5个可飞天”的条件，记“0.5个限制可飞天”
             * 3.既不满足“0.5个可飞天”的条件，又不满足“0.5个限制可飞天” 的条件，记“0.5个不可飞天”
             */
            QRegExp halfRegExp1("(3{4,})");
            QRegExp halfRegExp2("([2|3]{4,})");
            int halfPos1 = halfRegExp1.indexIn(valueStr);
            if(halfPos1 >= 0){
                emit sendMessage("0.5", row, m_titleList.indexOf("完全可飞"), 1, 1);
                kfttjValue[0] = 0.5;
                kfttjHash[kfttjKey] = kfttjValue;
            }else{
                int halfPos2 = halfRegExp2.indexIn(valueStr);
                if(halfPos2 >= 0){
                    emit sendMessage("0.5", row, m_titleList.indexOf("限制可飞"), 1, 1);
                    kfttjValue[1] = 0.5;
                    kfttjHash[kfttjKey] = kfttjValue;

                    if(effectHash.contains(kfttjKey)){
                        QStringList xzkfEffectList = effectHash[kfttjKey][0];
                        emit sendMessage(xzkfEffectList.join("\r\n"), row, m_titleList.size() - 1, 1, 1);
                        xzkfEffectHash[kfttjKey] = xzkfEffectList;
                    }
                }else{
                    emit sendMessage("0.5", row, m_titleList.indexOf("不可飞"), 1, 1);
                    kfttjValue[2] = 0.5;
                    kfttjHash[kfttjKey] = kfttjValue;

                    if(effectHash.contains(kfttjKey)){
                        QStringList bkfEffectList = effectHash[kfttjKey][1];
                        emit sendMessage(bkfEffectList.join("\r\n"), row, m_titleList.size() - 1, 1, 1);
                        bkfEffectHash[kfttjKey] = bkfEffectList;
                    }
                }
            }
        }else{
            //整天统计
            /*
             * 1.连续7个小时以上均为“完全可飞”，或存在两个及以上4~6小时连续的“完全可飞”时段，记为“1个完全可飞天”
             * 2.连续7个小时以上无“不可飞”，或存在两个以上4~6小时连续不出现“不可飞”的时段，记为“1个限制可飞天”
             * 3.仅存在一个连续4~6小时均为“完全可飞”的时段，记为“0.5个完全可飞天”和“0.5个不可飞天”
             * 4.仅存在一个连续4~6小时不出现“不可飞”的时段，记为“0.5个限制可飞天”和“0.5个不可飞天”
             * 5.存在一个连续4~6小时均为“完全可飞”的时段和一个连续4~6小时不出现“不可飞”的时段，记为“0.5个完全可飞天”和“0.5个限制可飞天”
             * 6.没有连续超过4小时不出现“不可飞”的时段，记为“1个不可飞天”
             */
            QRegExp wholeRegExp1("(3{7,})|(3{4}[1|2|3]*3{4})");
            QRegExp wholeRegExp2("([2|3]{7,})|([2|3]{4}[1|2|3]*[2|3]{4})");
            QRegExp wholeRegExp3("([1|2]|3{,3})*(3{4,6})([1|2]|3{,3})*");
            QRegExp wholeRegExp4("(1|[2|3]{,3})*([2|3]{4,6})(1|[2|3]{,3})*");
            QRegExp wholeRegExp5("(3{4}[1|2|3]*[2|3]{4})|([2|3]{4}[1|2|3]*3{4})");
            QRegExp wholeRegExp6("[2|3]{4,}");
            int wholePos1 = wholeRegExp1.indexIn(valueStr);
            if(wholePos1 >= 0){
                emit sendMessage("1", row, m_titleList.indexOf("完全可飞"), 1, 1);
                kfttjValue[0] = 1;
                kfttjHash[kfttjKey] = kfttjValue;
            }else{
                int wholePos2 = wholeRegExp2.indexIn(valueStr);
                if(wholePos2 >= 0){
                    emit sendMessage("1", row, m_titleList.indexOf("限制可飞"), 1, 1);
                    kfttjValue[1] = 1;
                    kfttjHash[kfttjKey] = kfttjValue;

                    if(effectHash.contains(kfttjKey)){
                        QStringList xzkfEffectList = effectHash[kfttjKey][0];
                        emit sendMessage(xzkfEffectList.join("\r\n"), row, m_titleList.size() - 1, 1, 1);
                        xzkfEffectHash[kfttjKey] = xzkfEffectList;
                    }
                }else{
                    int wholePos3 = wholeRegExp3.indexIn(valueStr);
                    if(wholePos3 >= 0){
                        emit sendMessage("0.5", row, m_titleList.indexOf("完全可飞"), 1, 1);
                        emit sendMessage("0.5", row, m_titleList.indexOf("不可飞"), 1, 1);
                        kfttjValue[0] = 0.5;
                        kfttjValue[2] = 0.5;
                        kfttjHash[kfttjKey] = kfttjValue;

                        if(effectHash.contains(kfttjKey)){
                            QStringList bkfEffectList = effectHash[kfttjKey][1];
                            emit sendMessage(bkfEffectList.join("\r\n"), row, m_titleList.size() - 1, 1, 1);
                            bkfEffectHash[kfttjKey] = bkfEffectList;
                        }
                    }else{
                        int wholePos4 = wholeRegExp4.indexIn(valueStr);
                        if(wholePos4 >= 0){
                            emit sendMessage("0.5", row, m_titleList.indexOf("限制可飞"), 1, 1);
                            emit sendMessage("0.5", row, m_titleList.indexOf("不可飞"), 1, 1);
                            kfttjValue[1] = 0.5;
                            kfttjValue[2] = 0.5;
                            kfttjHash[kfttjKey] = kfttjValue;

                            if(effectHash.contains(kfttjKey)){
                                QStringList xzkfEffectList = effectHash[kfttjKey][0];
                                QStringList bkfEffectList = effectHash[kfttjKey][1];
                                QStringList sumEffectList;
                                int xzkfEffectCount = xzkfEffectList.size();
                                for(int i = 0;i < xzkfEffectCount;i++){
                                    if(!sumEffectList.contains(xzkfEffectList[i])){
                                        sumEffectList.append(xzkfEffectList[i]);
                                    }
                                }
                                int bkfEffectCount = bkfEffectList.size();
                                for(int i = 0;i < bkfEffectCount;i++){
                                    if(!sumEffectList.contains(bkfEffectList[i])){
                                        sumEffectList.append(bkfEffectList[i]);
                                    }
                                }
                                emit sendMessage(sumEffectList.join("\r\n"), row, m_titleList.size() - 1, 1, 1);
                                xzkfEffectHash[kfttjKey] = xzkfEffectList;
                                bkfEffectHash[kfttjKey] = bkfEffectList;
                            }
                        }else{
                            int wholePos5 = wholeRegExp5.indexIn(valueStr);
                            if(wholePos5 >= 0){
                                emit sendMessage("0.5", row, m_titleList.indexOf("完全可飞"), 1, 1);
                                emit sendMessage("0.5", row, m_titleList.indexOf("限制可飞"), 1, 1);
                                kfttjValue[0] = 0.5;
                                kfttjValue[1] = 0.5;
                                kfttjHash[kfttjKey] = kfttjValue;

                                if(effectHash.contains(kfttjKey)){
                                    QStringList xzkfEffectList = effectHash[kfttjKey][0];
                                    emit sendMessage(xzkfEffectList.join("\r\n"), row, m_titleList.size() - 1, 1, 1);
                                    xzkfEffectHash[kfttjKey] = xzkfEffectList;
                                }
                            }else{
                                int wholePos6 = wholeRegExp6.indexIn(valueStr);
                                if(wholePos6 < 0){
                                    emit sendMessage("1", row, m_titleList.indexOf("不可飞"), 1, 1);
                                    kfttjValue[2] = 1;
                                    kfttjHash[kfttjKey] = kfttjValue;

                                    if(effectHash.contains(kfttjKey)){
                                        QStringList bkfEffectList = effectHash[kfttjKey][1];
                                        emit sendMessage(bkfEffectList.join("\r\n"), row, m_titleList.size() - 1, 1, 1);
                                        bkfEffectHash[kfttjKey] = bkfEffectList;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }else{
        int resCount = resAll.size();
        QStringList valueStrList;
        for(int i = 0;i < resCount;i++){
            int val = resAll[i];
            if(val != 0){
                valueStrList.append(QString("%1").arg(val));
            }
        }
        int valueCount = valueStrList.size();
        QString valueStr = valueStrList.join("");

        if(valueCount == 0){
            //缺测
            kfttjHash[kfttjKey] = kfttjValue;
        }else{
            if(valueStr.indexOf("3") > -1){
                emit sendMessage("1", row, m_titleList.indexOf("完全可飞"), 1, 1);
                kfttjValue[0] = 1;
                kfttjHash[kfttjKey] = kfttjValue;
            }else if(valueStr.indexOf("2") > -1){
                emit sendMessage("1", row, m_titleList.indexOf("限制可飞"), 1, 1);
                kfttjValue[1] = 1;
                kfttjHash[kfttjKey] = kfttjValue;
            }else{
                emit sendMessage("1", row, m_titleList.indexOf("不可飞"), 1, 1);
                kfttjValue[2] = 1;
                kfttjHash[kfttjKey] = kfttjValue;
            }
        }
    }

    resAll.clear();
}
