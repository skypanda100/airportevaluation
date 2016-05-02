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
    pgdb = new PgDataBase;
    this->query();
}

void KfttjResultWidget::initUI(){
    tableModel = new QStandardItemModel(1, 26, this);
    tableModel->setHeaderData(0, Qt::Horizontal, "日期");
    tableModel->setHeaderData(1, Qt::Horizontal, "气象要素");
    tableModel->setHeaderData(2, Qt::Horizontal, "17");
    tableModel->setHeaderData(3, Qt::Horizontal, "18");
    tableModel->setHeaderData(4, Qt::Horizontal, "19");
    tableModel->setHeaderData(5, Qt::Horizontal, "20");
    tableModel->setHeaderData(6, Qt::Horizontal, "21");
    tableModel->setHeaderData(7, Qt::Horizontal, "22");
    tableModel->setHeaderData(8, Qt::Horizontal, "23");
    tableModel->setHeaderData(9, Qt::Horizontal, "0");
    tableModel->setHeaderData(10, Qt::Horizontal, "1");
    tableModel->setHeaderData(11, Qt::Horizontal, "2");
    tableModel->setHeaderData(12, Qt::Horizontal, "3");
    tableModel->setHeaderData(13, Qt::Horizontal, "4");
    tableModel->setHeaderData(14, Qt::Horizontal, "5");
    tableModel->setHeaderData(15, Qt::Horizontal, "6");
    tableModel->setHeaderData(16, Qt::Horizontal, "7");
    tableModel->setHeaderData(17, Qt::Horizontal, "8");
    tableModel->setHeaderData(18, Qt::Horizontal, "9");
    tableModel->setHeaderData(19, Qt::Horizontal, "10");
    tableModel->setHeaderData(20, Qt::Horizontal, "11");
    tableModel->setHeaderData(21, Qt::Horizontal, "12");
    tableModel->setHeaderData(22, Qt::Horizontal, "13");
    tableModel->setHeaderData(23, Qt::Horizontal, "14");
    tableModel->setHeaderData(24, Qt::Horizontal, "15");
    tableModel->setHeaderData(25, Qt::Horizontal, "16");
    tableView = new QTableView;
    tableView->setModel(tableModel);


    this->setOrientation(Qt::Vertical);
    this->addWidget(tableView);
}

void KfttjResultWidget::initConnect(){

}

void KfttjResultWidget::query(){
    //查询月总簿表
    QString summaryStartDatetime = "2012-12-31 17:00:00";
    QString summaryEndDatetime = "2013-02-01 16:00:00";

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

void KfttjResultWidget::analysis(){

}
