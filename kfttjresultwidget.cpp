#include "kfttjresultwidget.h"

KfttjResultWidget::KfttjResultWidget(QWidget *parent)
    :QSplitter(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

KfttjResultWidget::~KfttjResultWidget(){
    delete pgdb;
    delete tableModel;
    delete tableView;
}

void KfttjResultWidget::initData(){
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
    this->query();
}

void KfttjResultWidget::initUI(){
    tableModel = new TableModel(0, titleList.size(), this);
    int titleCount = titleList.size();
    for(int i = 0;i < titleCount;i++){
        tableModel->setHeaderData(i, Qt::Horizontal, titleList[i]);
    }

    tableView = new QTableView;
    tableView->setModel(tableModel);

    this->setOrientation(Qt::Vertical);
    this->addWidget(tableView);

    this->analysis();
}

void KfttjResultWidget::initConnect(){

}

void KfttjResultWidget::query(){
    //查询月总簿表
    QString summaryStartDatetime = "2012-12-31 17:00:00";
    QString summaryEndDatetime = "2013-01-31 16:00:00";

    QString summarySql = QString("select * from zsdy_monthsummary where datetime >= '%1' and datetime <= '%2' order by datetime")
            .arg(summaryStartDatetime)
            .arg(summaryEndDatetime);

    QSqlQueryModel *summaryPlainModel = pgdb->queryModel(summarySql);
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
    QString extremumStartDatetime = "2013-01-01 00:00:00";
    QString extremumEndDatetime = "2013-01-31 23:59:59";

    QString extremumSql = QString("select * from zsdy_extremum where datetime >= '%1' and datetime <= '%2' order by datetime")
            .arg(extremumStartDatetime)
            .arg(extremumEndDatetime);

    QSqlQueryModel *extremumPlainModel = pgdb->queryModel(extremumSql);
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

//能见度->云->侧风->逆风
void KfttjResultWidget::analysis(){
    int summaryCount = summaryList.size();
    int dateCount = 0;
    QDateTime lastDateTime_local;
    for(int i = 0;i < summaryCount;i++){
        /***表格整体布局***/
        Monthsummary monthsummary = summaryList[i];
        QDateTime currentDateTime_local = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss").addSecs(3600 * 8 - 1);
        QDateTime currentDateTime_utc = QDateTime::fromString(monthsummary.datetime(), "yyyy-MM-ddThh:mm:ss");
        if(i == 0){
            lastDateTime_local = currentDateTime_local;
            tableModel->insertRows(dateCount * 4, 4, QModelIndex()); //4为4个气象要素
            //日期
            tableModel->setData(tableModel->index(dateCount * 4, 0, QModelIndex()), currentDateTime_local.toString("yyyy年MM月dd日"));
            tableView->setSpan(dateCount * 4, 0, 4, 1);
            //能见度
            tableModel->setData(tableModel->index(dateCount * 4, 1, QModelIndex()), "能见度");
            //云
            tableModel->setData(tableModel->index(dateCount * 4 + 1, 1, QModelIndex()), "云");
            //侧风
            tableModel->setData(tableModel->index(dateCount * 4 + 2, 1, QModelIndex()), "侧风");
            //逆风
            tableModel->setData(tableModel->index(dateCount * 4 + 3, 1, QModelIndex()), "逆风");
            //完全可飞
            tableView->setSpan(dateCount * 4, titleList.indexOf("完全可飞"), 4, 1);
            //限制可飞
            tableView->setSpan(dateCount * 4, titleList.indexOf("限制可飞"), 4, 1);
            //不可飞
            tableView->setSpan(dateCount * 4, titleList.indexOf("不可飞"), 4, 1);
        }else{
            if(lastDateTime_local.daysTo(currentDateTime_local) >= 1){
                dateCount += 1;
                tableModel->insertRows(dateCount * 4, 4, QModelIndex()); //4为4个气象要素
                //日期
                tableModel->setData(tableModel->index(dateCount * 4, 0, QModelIndex()), currentDateTime_local.toString("yyyy年MM月dd日"));
                tableView->setSpan(dateCount * 4, 0, 4, 1);
                //能见度
                tableModel->setData(tableModel->index(dateCount * 4, 1, QModelIndex()), "能见度");
                //云
                tableModel->setData(tableModel->index(dateCount * 4 + 1, 1, QModelIndex()), "云");
                //侧风
                tableModel->setData(tableModel->index(dateCount * 4 + 2, 1, QModelIndex()), "侧风");
                //逆风
                tableModel->setData(tableModel->index(dateCount * 4 + 3, 1, QModelIndex()), "逆风");
                //完全可飞
                tableView->setSpan(dateCount * 4, titleList.indexOf("完全可飞"), 4, 1);
                //限制可飞
                tableView->setSpan(dateCount * 4, titleList.indexOf("限制可飞"), 4, 1);
                //不可飞
                tableView->setSpan(dateCount * 4, titleList.indexOf("不可飞"), 4, 1);
            }
        }
        lastDateTime_local = currentDateTime_local;

        /***数据分析***/
        bool canExecute = false;
        int hour = currentDateTime_utc.toString("h").toInt();
        if(JAN_DAY_E[0] > JAN_DAY_E[sizeof(JAN_DAY_E) / sizeof(JAN_DAY_E[0]) - 1]){
            if(hour >= JAN_DAY_E[0] || hour <= JAN_DAY_E[sizeof(JAN_DAY_E) / sizeof(JAN_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }else{
            if(hour >= JAN_DAY_E[0] && hour <= JAN_DAY_E[sizeof(JAN_DAY_E) / sizeof(JAN_DAY_E[0]) - 1]){
                canExecute = true;
            }
        }
        if(canExecute){
            //能见度
            QString leadingvisibilityStr = monthsummary.leadingvisibility().trimmed();
            if(leadingvisibilityStr.compare("///") == 0){
                tableModel->setData(tableModel->index(dateCount * 4, titleList.indexOf(QString::number(hour)), QModelIndex()), "1");
            }else if(leadingvisibilityStr.compare("") != 0){
                int leadingvisibility = leadingvisibilityStr.toInt();
                if(leadingvisibility < 3000){
                    tableModel->setData(tableModel->index(dateCount * 4, titleList.indexOf(QString::number(hour)), QModelIndex()), "1");
                }else if(leadingvisibility >= 3000 && leadingvisibility < 5000){
                    tableModel->setData(tableModel->index(dateCount * 4, titleList.indexOf(QString::number(hour)), QModelIndex()), "2");
                }else if(leadingvisibility >= 5000){
                    tableModel->setData(tableModel->index(dateCount * 4, titleList.indexOf(QString::number(hour)), QModelIndex()), "3");
                }else{

                }
            }else{

            }
            //云
            //风
            QString windspeedStr = monthsummary.windspeed().trimmed();
            QString gustspeedStr = monthsummary.gustspeed().trimmed();
            QString winddirectionStr = monthsummary.winddirection().trimmed();
            if(windspeedStr.compare("") != 0 || gustspeedStr.compare("") != 0){
                float windspeed = windspeedStr.toFloat();
                float gustspeed = gustspeedStr.toFloat();
                float speed = qMax(windspeed, gustspeed);
                float crossspeed = speed;
                float headspeed = speed;
                if(winddirectionStr.compare("C") != 0 && winddirectionStr.compare("VRB") != 0){
                    float winddirection = winddirectionStr.toFloat();
                    crossspeed = qAbs(qSin(winddirection - 180) * speed);
                    headspeed = qCos(winddirection - 180) * speed;
                }
                //侧风
                if(crossspeed >= 12){
                    tableModel->setData(tableModel->index(dateCount * 4 + 2, titleList.indexOf(QString::number(hour)), QModelIndex()), "1");
                }else if(crossspeed < 12 && crossspeed >= 8){
                    tableModel->setData(tableModel->index(dateCount * 4 + 2, titleList.indexOf(QString::number(hour)), QModelIndex()), "2");
                }else if(crossspeed < 8){
                    tableModel->setData(tableModel->index(dateCount * 4 + 2, titleList.indexOf(QString::number(hour)), QModelIndex()), "3");
                }else{

                }
                //逆风
                if(headspeed >= 15){
                    tableModel->setData(tableModel->index(dateCount * 4 + 3, titleList.indexOf(QString::number(hour)), QModelIndex()), "1");
                }else if(headspeed < 15 && headspeed >= 10){
                    tableModel->setData(tableModel->index(dateCount * 4 + 3, titleList.indexOf(QString::number(hour)), QModelIndex()), "2");
                }else if(headspeed < 10){
                    tableModel->setData(tableModel->index(dateCount * 4 + 3, titleList.indexOf(QString::number(hour)), QModelIndex()), "3");
                }else{

                }
            }
        }
    }

    //调整列宽
    for (int column = 2; column < tableModel->columnCount() - 3; ++column){
        tableView->resizeColumnToContents(column);
    }
}
