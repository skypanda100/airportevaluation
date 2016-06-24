#include "kfttjcontrol.h"

KfttjControl::KfttjControl(QObject *parent)
    :QThread(parent)
{
    titleList << "日期"
              << "气象要素"
              << "17"
              << "18"
              << "19"
              << "20"
              << "21"
              << "22"
              << "23"
              << "0"
              << "1"
              << "2"
              << "3"
              << "4"
              << "5"
              << "6"
              << "7"
              << "8"
              << "9"
              << "10"
              << "11"
              << "12"
              << "13"
              << "14"
              << "15"
              << "16"
              << "完全可飞"
              << "限制可飞"
              << "不可飞"
              << "影响原因";
    pgdb = new PgDataBase;

    //需要统计的气象参数个数
    elementCount = 4 + 1; ////4:能见度,云,侧风,逆风 1:综合
}

KfttjControl::~KfttjControl(){
    delete pgdb;
}

void KfttjControl::setSummarySql(QString summarySql){
    this->m_summarySql = summarySql;
}

void KfttjControl::setExtremumSql(QString extremumSql){
    this->m_extremumSql = extremumSql;
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
}

void KfttjControl::run(){
    //更新进度
    emit setProgressValue(1);
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

//能见度->云->侧风->逆风
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
            sendMessage(dateCount * elementCount, elementCount);//4为4个气象要素
            //日期
            emit sendMessage(currentDateTime_local.toString("yyyy年MM月dd日"), dateCount * elementCount, 0, elementCount, 1);
            //能见度
            emit sendMessage("能见度", dateCount * elementCount, 1, 1, 1);
            //云
            emit sendMessage("云", dateCount * elementCount + 1, 1, 1, 1);
            //侧风
            emit sendMessage("侧风", dateCount * elementCount + 2, 1, 1, 1);
            //逆风
            emit sendMessage("逆风", dateCount * elementCount + 3, 1, 1, 1);
            //综合
            emit sendMessage("综合", dateCount * elementCount + 4, 1, 1, 1);
            //完全可飞
            emit sendMessage("", dateCount * elementCount, titleList.indexOf("完全可飞"), elementCount, 1);
            //限制可飞
            emit sendMessage("", dateCount * elementCount, titleList.indexOf("限制可飞"), elementCount, 1);
            //不可飞
            emit sendMessage("", dateCount * elementCount, titleList.indexOf("不可飞"), elementCount, 1);
            //影响原因
            emit sendMessage("", dateCount * elementCount, titleList.size() - 1, elementCount, 1);

            //日可飞天统计
            if(i > 0){
                analysisDay(lastDateTime_local, (dateCount - 1) * elementCount);
            }
        }

        lastDateTime_local = currentDateTime_local;

        /***数据分析***/
        bool canExecute = isDayTime(currentDateTime_utc);
        int hour = currentDateTime_utc.toString("h").toInt();
        if(canExecute){
            QStringList results;
            //能见度
            results.append(analysisVisibility(monthsummary, dateCount * elementCount, titleList.indexOf(QString::number(hour))));
            //云
            results.append(analysisCloud(monthsummary, dateCount * elementCount + 1, titleList.indexOf(QString::number(hour))));
            //侧风
            results.append(analysisCrossWind(monthsummary, dateCount * elementCount + 2, titleList.indexOf(QString::number(hour))));
            //逆风
            results.append(analysisHeadWind(monthsummary, dateCount * elementCount + 3, titleList.indexOf(QString::number(hour))));
            //综合
            analysisAll(currentDateTime_local, results, dateCount * elementCount + 4, titleList.indexOf(QString::number(hour)));
        }else{
            resAll.append(0);
        }
        //更新进度
        emit setProgressValue((int)(((qreal)(i + 1)/(qreal)summaryCount) * 100));
    }
    //日可飞天统计
    analysisDay(lastDateTime_local, (dateCount - 1) * elementCount);
}

/**
 * @brief KfttjControl::guessVisibilityEvolution
 * 推算能见度
 * @param extremum
 * @param hour
 * @return
 */
QString KfttjControl::guessVisibilityEvolution(const Extremum &extremum, int hour){
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
                if(leadingvisibility < 3000){
                    valueStr = QString("1");
                }else if(leadingvisibility >= 3000 && leadingvisibility < 5000){
                    valueStr = QString("2");
                }else if(leadingvisibility >= 5000){
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
 * @brief KfttjControl::analysisVisibility
 * 能见度分析
 * @param monthsummary
 */
QString KfttjControl::analysisVisibility(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
    QString leadingvisibilityStr = monthsummary.leadingvisibility().trimmed();
    QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
    int hour = currentDateTime_utc.toString("h").toInt();
    if(leadingvisibilityStr.compare("///") == 0){
        resultStr = QString("1");
    }else if(leadingvisibilityStr.compare("") != 0){
        int leadingvisibility = leadingvisibilityStr.toInt();
        if(leadingvisibility < 3000){
            resultStr = QString("1");
        }else if(leadingvisibility >= 3000 && leadingvisibility < 5000){
            resultStr = QString("2");
        }else if(leadingvisibility >= 5000){
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
                resultStr = guessVisibilityEvolution(extremum, half_day);
            }
        }else if(how == 2){
            int index = getExtremumIndex(currentDateTime_utc);
            if(index > -1){
                Extremum extremum = extremumList[index];
                resultStr = guessVisibilityEvolution(extremum, whole_day);
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
 * @brief KfttjControl::analysisCloud
 * 云分析
 * @param monthsummary
 */
QString KfttjControl::analysisCloud(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
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
                if(cloudState < 600){
                    resultStr = QString("1");
                }else if(cloudState < 2500 && cloudState >= 600){
                    resultStr = QString("2");
                }else if(cloudState >= 2500){
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
 * @brief KfttjControl::analysisCrossWind
 * 侧风分析
 * @param monthsummary
 */
QString KfttjControl::analysisCrossWind(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
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
            crossspeed = qAbs(qSin(winddirection - 180) * speed);
        }
        //侧风
        if(crossspeed >= 12){
            resultStr = QString("1");
        }else if(crossspeed < 12 && crossspeed >= 8){
            resultStr = QString("2");
        }else if(crossspeed < 8){
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
 * @brief KfttjControl::analysisHeadWind
 * 逆风分析
 * @param monthsummary
 */
QString KfttjControl::analysisHeadWind(const Monthsummary &monthsummary, int row, int col){
    QString resultStr("");
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
            headspeed = qCos(winddirection - 180) * speed;
        }
        //逆风
        if(headspeed >= 15){
            resultStr = QString("1");
        }else if(headspeed < 15 && headspeed >= 10){
            resultStr = QString("2");
        }else if(headspeed < 10){
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
 * @brief KfttjControl::analysisAll
 * 综合所有气象要素进行分析
 * @return
 */
QString KfttjControl::analysisAll(QDateTime currentDateTime_local, QStringList results, int row, int col){
    /***简单这样写，这块得做活***/
    QList<QString> elementList;
    elementList.append("能见度");
    elementList.append("云");
    elementList.append("侧风");
    elementList.append("逆风");
    /************************/
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
    //取得对应月份开始统计的位置
    int month = lastDateTime_local.toString("M").toInt();
    int startIndex = 0;
    switch (month) {
    case 1:
        startIndex = titleList.indexOf(QString("%1").arg(jan_day[0])) - 2;
        break;
    case 2:
        startIndex = titleList.indexOf(QString("%1").arg(feb_day[0])) - 2;
        break;
    case 3:
        startIndex = titleList.indexOf(QString("%1").arg(mar_day[0])) - 2;
        break;
    case 4:
        startIndex = titleList.indexOf(QString("%1").arg(apr_day[0])) - 2;
        break;
    case 5:
        startIndex = titleList.indexOf(QString("%1").arg(may_day[0])) - 2;
        break;
    case 6:
        startIndex = titleList.indexOf(QString("%1").arg(jun_day[0])) - 2;
        break;
    case 7:
        startIndex = titleList.indexOf(QString("%1").arg(jul_day[0])) - 2;
        break;
    case 8:
        startIndex = titleList.indexOf(QString("%1").arg(aug_day[0])) - 2;
        break;
    case 9:
        startIndex = titleList.indexOf(QString("%1").arg(sep_day[0])) - 2;
        break;
    case 10:
        startIndex = titleList.indexOf(QString("%1").arg(oct_day[0])) - 2;
        break;
    case 11:
        startIndex = titleList.indexOf(QString("%1").arg(nov_day[0])) - 2;
        break;
    case 12:
        startIndex = titleList.indexOf(QString("%1").arg(dec_day[0])) - 2;
        break;
    default:
        break;
    }
    //取得半天及整天所在的位置
    int halfIndex = titleList.indexOf(QString("%1").arg(half_day)) - 2;
    int wholeIndex = titleList.indexOf(QString("%1").arg(whole_day)) - 2;

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
            emit sendMessage("0.5", row, titleList.indexOf("完全可飞"), 1, 1);
            kfttjValue[0] = 0.5;
            kfttjHash[kfttjKey] = kfttjValue;
        }else{
            int halfPos2 = halfRegExp2.indexIn(valueStr);
            if(halfPos2 >= 0){
                emit sendMessage("0.5", row, titleList.indexOf("限制可飞"), 1, 1);
                kfttjValue[1] = 0.5;
                kfttjHash[kfttjKey] = kfttjValue;

                if(effectHash.contains(kfttjKey)){
                    QStringList xzkfEffectList = effectHash[kfttjKey][0];
                    emit sendMessage(xzkfEffectList.join("\n"), row, titleList.size() - 1, 1, 1);
                    xzkfEffectHash[kfttjKey] = xzkfEffectList;
                }
            }else{
                emit sendMessage("0.5", row, titleList.indexOf("不可飞"), 1, 1);
                kfttjValue[2] = 0.5;
                kfttjHash[kfttjKey] = kfttjValue;

                if(effectHash.contains(kfttjKey)){
                    QStringList bkfEffectList = effectHash[kfttjKey][1];
                    emit sendMessage(bkfEffectList.join("\n"), row, titleList.size() - 1, 1, 1);
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
            emit sendMessage("1", row, titleList.indexOf("完全可飞"), 1, 1);
            kfttjValue[0] = 1;
            kfttjHash[kfttjKey] = kfttjValue;
        }else{
            int wholePos2 = wholeRegExp2.indexIn(valueStr);
            if(wholePos2 >= 0){
                emit sendMessage("1", row, titleList.indexOf("限制可飞"), 1, 1);
                kfttjValue[1] = 1;
                kfttjHash[kfttjKey] = kfttjValue;

                if(effectHash.contains(kfttjKey)){
                    QStringList xzkfEffectList = effectHash[kfttjKey][0];
                    emit sendMessage(xzkfEffectList.join("\n"), row, titleList.size() - 1, 1, 1);
                    xzkfEffectHash[kfttjKey] = xzkfEffectList;
                }
            }else{
                int wholePos3 = wholeRegExp3.indexIn(valueStr);
                if(wholePos3 >= 0){
                    emit sendMessage("0.5", row, titleList.indexOf("完全可飞"), 1, 1);
                    emit sendMessage("0.5", row, titleList.indexOf("不可飞"), 1, 1);
                    kfttjValue[0] = 0.5;
                    kfttjValue[2] = 0.5;
                    kfttjHash[kfttjKey] = kfttjValue;

                    if(effectHash.contains(kfttjKey)){
                        QStringList bkfEffectList = effectHash[kfttjKey][1];
                        emit sendMessage(bkfEffectList.join("\n"), row, titleList.size() - 1, 1, 1);
                        bkfEffectHash[kfttjKey] = bkfEffectList;
                    }
                }else{
                    int wholePos4 = wholeRegExp4.indexIn(valueStr);
                    if(wholePos4 >= 0){
                        emit sendMessage("0.5", row, titleList.indexOf("限制可飞"), 1, 1);
                        emit sendMessage("0.5", row, titleList.indexOf("不可飞"), 1, 1);
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
                            emit sendMessage(sumEffectList.join("\n"), row, titleList.size() - 1, 1, 1);
                            xzkfEffectHash[kfttjKey] = xzkfEffectList;
                            bkfEffectHash[kfttjKey] = bkfEffectList;
                        }
                    }else{
                        int wholePos5 = wholeRegExp5.indexIn(valueStr);
                        if(wholePos5 >= 0){
                            emit sendMessage("0.5", row, titleList.indexOf("完全可飞"), 1, 1);
                            emit sendMessage("0.5", row, titleList.indexOf("限制可飞"), 1, 1);
                            kfttjValue[0] = 0.5;
                            kfttjValue[1] = 0.5;
                            kfttjHash[kfttjKey] = kfttjValue;

                            if(effectHash.contains(kfttjKey)){
                                QStringList xzkfEffectList = effectHash[kfttjKey][0];
                                emit sendMessage(xzkfEffectList.join("\n"), row, titleList.size() - 1, 1, 1);
                                xzkfEffectHash[kfttjKey] = xzkfEffectList;
                            }
                        }else{
                            int wholePos6 = wholeRegExp6.indexIn(valueStr);
                            if(wholePos6 < 0){
                                emit sendMessage("1", row, titleList.indexOf("不可飞"), 1, 1);
                                kfttjValue[2] = 1;
                                kfttjHash[kfttjKey] = kfttjValue;

                                if(effectHash.contains(kfttjKey)){
                                    QStringList bkfEffectList = effectHash[kfttjKey][1];
                                    emit sendMessage(bkfEffectList.join("\n"), row, titleList.size() - 1, 1, 1);
                                    bkfEffectHash[kfttjKey] = bkfEffectList;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    resAll.clear();
}
