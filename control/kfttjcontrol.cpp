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
              << "不可飞";
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

void KfttjControl::run(){
    query();
    analysis();
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
        if(JAN_DAY_E[0] > JAN_DAY_E[sizeof(JAN_DAY_E) / sizeof(JAN_DAY_E[0]) - 1]){
            if(hour >= JAN_DAY_E[0] || hour <= JAN_DAY_E[sizeof(JAN_DAY_E) / sizeof(JAN_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= JAN_DAY_E[0] && hour <= JAN_DAY_E[sizeof(JAN_DAY_E) / sizeof(JAN_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(2 == month){
        if(FEB_DAY_E[0] > FEB_DAY_E[sizeof(FEB_DAY_E) / sizeof(FEB_DAY_E[0]) - 1]){
            if(hour >= FEB_DAY_E[0] || hour <= FEB_DAY_E[sizeof(FEB_DAY_E) / sizeof(FEB_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= FEB_DAY_E[0] && hour <= FEB_DAY_E[sizeof(FEB_DAY_E) / sizeof(FEB_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(3 == month){
        if(MAR_DAY_E[0] > MAR_DAY_E[sizeof(MAR_DAY_E) / sizeof(MAR_DAY_E[0]) - 1]){
            if(hour >= MAR_DAY_E[0] || hour <= MAR_DAY_E[sizeof(MAR_DAY_E) / sizeof(MAR_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= MAR_DAY_E[0] && hour <= MAR_DAY_E[sizeof(MAR_DAY_E) / sizeof(MAR_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(4 == month){
        if(APR_DAY_E[0] > APR_DAY_E[sizeof(APR_DAY_E) / sizeof(APR_DAY_E[0]) - 1]){
            if(hour >= APR_DAY_E[0] || hour <= APR_DAY_E[sizeof(APR_DAY_E) / sizeof(APR_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= APR_DAY_E[0] && hour <= APR_DAY_E[sizeof(APR_DAY_E) / sizeof(APR_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(5 == month){
        if(MAY_DAY_E[0] > MAY_DAY_E[sizeof(MAY_DAY_E) / sizeof(MAY_DAY_E[0]) - 1]){
            if(hour >= MAY_DAY_E[0] || hour <= MAY_DAY_E[sizeof(MAY_DAY_E) / sizeof(MAY_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= MAY_DAY_E[0] && hour <= MAY_DAY_E[sizeof(MAY_DAY_E) / sizeof(MAY_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(6 == month){
        if(JUN_DAY_E[0] > JUN_DAY_E[sizeof(JUN_DAY_E) / sizeof(JUN_DAY_E[0]) - 1]){
            if(hour >= JUN_DAY_E[0] || hour <= JUN_DAY_E[sizeof(JUN_DAY_E) / sizeof(JUN_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= JUN_DAY_E[0] && hour <= JUN_DAY_E[sizeof(JUN_DAY_E) / sizeof(JUN_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(7 == month){
        if(JUL_DAY_E[0] > JUL_DAY_E[sizeof(JUL_DAY_E) / sizeof(JUL_DAY_E[0]) - 1]){
            if(hour >= JUL_DAY_E[0] || hour <= JUL_DAY_E[sizeof(JUL_DAY_E) / sizeof(JUL_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= JUL_DAY_E[0] && hour <= JUL_DAY_E[sizeof(JUL_DAY_E) / sizeof(JUL_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(8 == month){
        if(AUG_DAY_E[0] > AUG_DAY_E[sizeof(AUG_DAY_E) / sizeof(AUG_DAY_E[0]) - 1]){
            if(hour >= AUG_DAY_E[0] || hour <= AUG_DAY_E[sizeof(AUG_DAY_E) / sizeof(AUG_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= AUG_DAY_E[0] && hour <= AUG_DAY_E[sizeof(AUG_DAY_E) / sizeof(AUG_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(9 == month){
        if(SEP_DAY_E[0] > SEP_DAY_E[sizeof(SEP_DAY_E) / sizeof(SEP_DAY_E[0]) - 1]){
            if(hour >= SEP_DAY_E[0] || hour <= SEP_DAY_E[sizeof(SEP_DAY_E) / sizeof(SEP_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= SEP_DAY_E[0] && hour <= SEP_DAY_E[sizeof(SEP_DAY_E) / sizeof(SEP_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(10 == month){
        if(OCT_DAY_E[0] > OCT_DAY_E[sizeof(OCT_DAY_E) / sizeof(OCT_DAY_E[0]) - 1]){
            if(hour >= OCT_DAY_E[0] || hour <= OCT_DAY_E[sizeof(OCT_DAY_E) / sizeof(OCT_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= OCT_DAY_E[0] && hour <= OCT_DAY_E[sizeof(OCT_DAY_E) / sizeof(OCT_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else if(11 == month){
        if(NOV_DAY_E[0] > NOV_DAY_E[sizeof(NOV_DAY_E) / sizeof(NOV_DAY_E[0]) - 1]){
            if(hour >= NOV_DAY_E[0] || hour <= NOV_DAY_E[sizeof(NOV_DAY_E) / sizeof(NOV_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= NOV_DAY_E[0] && hour <= NOV_DAY_E[sizeof(NOV_DAY_E) / sizeof(NOV_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
    }else{
        if(DEC_DAY_E[0] > DEC_DAY_E[sizeof(DEC_DAY_E) / sizeof(DEC_DAY_E[0]) - 1]){
            if(hour >= DEC_DAY_E[0] || hour <= DEC_DAY_E[sizeof(DEC_DAY_E) / sizeof(DEC_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= DEC_DAY_E[0] && hour <= DEC_DAY_E[sizeof(DEC_DAY_E) / sizeof(DEC_DAY_E[0]) - 1]){
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
    if(hour - HALF_DAY_E == 1 || hour - HALF_DAY_E == 2){
        return 1;
    }else if(hour - WHOLE_DAY_E == 1 || hour - WHOLE_DAY_E == 2){
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
            analysisAll(results, dateCount * elementCount + 4, titleList.indexOf(QString::number(hour)));
        }else{
            resAll.append(0);
        }
        //更新进度
        emit setProgressValue((int)(((qreal)(i + 1)/(qreal)summaryCount) * 100));
    }
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
                resultStr = guessVisibilityEvolution(extremum, HALF_DAY_E);
            }
        }else if(how == 2){
            int index = getExtremumIndex(currentDateTime_utc);
            if(index > -1){
                Extremum extremum = extremumList[index];
                resultStr = guessVisibilityEvolution(extremum, WHOLE_DAY_E);
            }
        }
    }
    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }
    //
    if(!(hour - HALF_DAY_E == 1 || hour - HALF_DAY_E == 2 || hour - WHOLE_DAY_E == 1 || hour - WHOLE_DAY_E == 2)){
        if(hour - HALF_DAY_E == -1){
            resVisibilityStr1 = resultStr;
        }else if(hour - HALF_DAY_E == 0){
            resVisibilityStr2 = resultStr;
        }else if(hour - WHOLE_DAY_E == -1){
            resVisibilityStr1 = resultStr;
        }else if(hour - WHOLE_DAY_E == 0){
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
                        resultStr = guessCloudEvolution(extremum, HALF_DAY_E);
                    }
                }else if(how == 2){
                    int index = getExtremumIndex(currentDateTime_utc);
                    if(index > -1){
                        Extremum extremum = extremumList[index];
                        resultStr = guessCloudEvolution(extremum, WHOLE_DAY_E);
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
                resultStr = guessCloudEvolution(extremum, HALF_DAY_E);
            }
        }else if(how == 2){
            int index = getExtremumIndex(currentDateTime_utc);
            if(index > -1){
                Extremum extremum = extremumList[index];
                resultStr = guessCloudEvolution(extremum, WHOLE_DAY_E);
            }
        }
    }

    if(resultStr.compare("") != 0){
        emit sendMessage(resultStr, row, col, 1, 1);
    }

    //
    if(!(hour - HALF_DAY_E == 1 || hour - HALF_DAY_E == 2 || hour - WHOLE_DAY_E == 1 || hour - WHOLE_DAY_E == 2)){
        if(hour - HALF_DAY_E == -1){
            resCloudStr1 = resultStr;
        }else if(hour - HALF_DAY_E == 0){
            resCloudStr2 = resultStr;
        }else if(hour - WHOLE_DAY_E == -1){
            resCloudStr1 = resultStr;
        }else if(hour - WHOLE_DAY_E == 0){
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
QString KfttjControl::analysisAll(QStringList results, int row, int col){
    QString resultStr("");
    int resultCount = results.size();
    for(int i = 0;i < resultCount;i++){
        QString resStr = results[i];
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
        emit sendMessage(resultStr, row, col, 1, 1);
    }

    //日可飞天统计添加
    resAll.append(resultStr.toInt());

    return resultStr;
}

void KfttjControl::analysisDay(QDateTime lastDateTime_local, int row){
    //取得对应月份开始统计的位置
    int month = lastDateTime_local.toString("M").toInt();
    int startIndex = 0;
    switch (month) {
    case 1:
        startIndex = titleList.indexOf(QString("%1").arg(JAN_DAY_E[0])) - 2;
        break;
    case 2:
        startIndex = titleList.indexOf(QString("%1").arg(FEB_DAY_E[0])) - 2;
        break;
    case 3:
        startIndex = titleList.indexOf(QString("%1").arg(MAR_DAY_E[0])) - 2;
        break;
    case 4:
        startIndex = titleList.indexOf(QString("%1").arg(APR_DAY_E[0])) - 2;
        break;
    case 5:
        startIndex = titleList.indexOf(QString("%1").arg(MAY_DAY_E[0])) - 2;
        break;
    case 6:
        startIndex = titleList.indexOf(QString("%1").arg(JUN_DAY_E[0])) - 2;
        break;
    case 7:
        startIndex = titleList.indexOf(QString("%1").arg(JUL_DAY_E[0])) - 2;
        break;
    case 8:
        startIndex = titleList.indexOf(QString("%1").arg(AUG_DAY_E[0])) - 2;
        break;
    case 9:
        startIndex = titleList.indexOf(QString("%1").arg(SEP_DAY_E[0])) - 2;
        break;
    case 10:
        startIndex = titleList.indexOf(QString("%1").arg(OCT_DAY_E[0])) - 2;
        break;
    case 11:
        startIndex = titleList.indexOf(QString("%1").arg(NOV_DAY_E[0])) - 2;
        break;
    case 12:
        startIndex = titleList.indexOf(QString("%1").arg(DEC_DAY_E[0])) - 2;
        break;
    default:
        break;
    }
    //取得半天及整天所在的位置
    int halfIndex = titleList.indexOf(QString("%1").arg(HALF_DAY_E)) - 2;
    int wholeIndex = titleList.indexOf(QString("%1").arg(WHOLE_DAY_E)) - 2;

    //判断是采用半天的统计算法还是整天的统计算法
    int resCount = resAll.size();
    QList<int> valueList;
    QStringList valueStrList;
    for(int i = startIndex;i < resCount;i++){
        int val = resAll[i];
        if(val == 0){
            break;
        }else{
            valueList.append(val);
            valueStrList.append(QString("%1").arg(val));
        }
    }
    int minHalfCount = halfIndex - startIndex + 1;
    int minWholeCount = wholeIndex - startIndex + 1;
    int valueCount = valueList.size();
    if(valueCount < minHalfCount){
        //缺测
    }else if(valueCount >= minHalfCount && valueCount < minWholeCount){
        //半天统计
        int wqkfCount = 0;
        for(int i = 0;i < valueCount;i++){
            if(valueList[i] == 3){
                wqkfCount += 1;
            }else{
                if(wqkfCount > 0){
                    break;
                }
            }
        }
        if(wqkfCount >= 4){
            emit sendMessage("0.5", row, titleList.indexOf("完全可飞"), 1, 1);
        }else{
            int bkfCount = 0;
            for(int i = 0;i < valueCount;i++){
                if(valueList[i] == 1){
                    bkfCount += 1;
                    break;
                }
            }
            if(bkfCount > 0){
                emit sendMessage("0.5", row, titleList.indexOf("不可飞"), 1, 1);
            }else{
                emit sendMessage("0.5", row, titleList.indexOf("完全可飞"), 1, 1);
                emit sendMessage("0.5", row, titleList.indexOf("限制可飞"), 1, 1);
            }
        }

    }else{
        //整天统计
        int wqkfCount1 = 0;
        int wqkfCount2 = 0;
        bool wqkfChange = false;
        for(int i = 0;i < valueCount;i++){
            if(valueList[i] == 3){
                if(!wqkfChange){
                    wqkfCount1 += 1;
                }else{
                    wqkfCount2 += 1;
                }
            }else{
                if(!wqkfChange){
                    if(wqkfCount1 > 0){
                        wqkfChange = true;
                    }
                }else{
                    if(wqkfCount2 > 0){
                        break;
                    }
                }
            }
        }

        if(wqkfCount1 >= 7 || (wqkfCount1 >= 4 && wqkfCount2 >= 4)){
            emit sendMessage("1", row, titleList.indexOf("完全可飞"), 1, 1);
        }else{
            int xzkfCount1 = 0;
            int xzkfCount2 = 0;
            bool xzkfChange = false;
            for(int i = 0;i < valueCount;i++){
                if(valueList[i] == 3 || valueList[i] == 2){
                    if(!xzkfChange){
                        xzkfCount1 += 1;
                    }else{
                        xzkfCount2 += 1;
                    }
                }else{
                    if(!xzkfChange){
                        if(xzkfCount1 > 0){
                            xzkfChange = true;
                        }
                    }else{
                        if(xzkfCount2 > 0){
                            break;
                        }
                    }
                }
            }
            if(xzkfCount1 >= 7 || (xzkfCount1 >= 4 && xzkfCount2 >= 4)){
                emit sendMessage("1", row, titleList.indexOf("限制可飞"), 1, 1);
            }else if(wqkfCount1 >= 4 && wqkfCount2 < 4 || wqkfCount1 < 4 && wqkfCount2 >= 4){
                emit sendMessage("0.5", row, titleList.indexOf("完全可飞"), 1, 1);
                emit sendMessage("0.5", row, titleList.indexOf("不可飞"), 1, 1);
            }else if(xzkfCount1 >= 4 && xzkfCount2 < 4 || xzkfCount1 < 4 && xzkfCount2 >= 4){
                emit sendMessage("0.5", row, titleList.indexOf("限制可飞"), 1, 1);
                emit sendMessage("0.5", row, titleList.indexOf("不可飞"), 1, 1);
            }else{

                QString valueStr = valueStrList.join("");
                QRegExp regExp("(3{4,}[1|2|3]*[2|3]{4,}){2}|([2|3]{4,}[1|2|3]*3{4,}){2}");
                regExp.setMinimal(true);
                int pos = regExp.indexIn(valueStr);
                if(pos >= 0){
                    emit sendMessage("0.5", row, titleList.indexOf("完全可飞"), 1, 1);
                    emit sendMessage("0.5", row, titleList.indexOf("限制可飞"), 1, 1);
                }else{
                    QRegExp regExp("[2|3]{4}");
                    int pos = regExp.indexIn(valueStr);
                    if(pos < 0){
                        emit sendMessage("1", row, titleList.indexOf("不可飞"), 1, 1);
                    }
                }
                qDebug() << valueStr << row;
            }
        }
    }
    resAll.clear();
}
