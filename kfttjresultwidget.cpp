#include "kfttjresultwidget.h"

KfttjResultWidget::KfttjResultWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initData();
}

KfttjResultWidget::~KfttjResultWidget(){
    delete pgdb;
}

void KfttjResultWidget::initData(){
    pgdb = new PgDataBase;
    this->query();
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

    }
    delete extremumPlainModel;
}
