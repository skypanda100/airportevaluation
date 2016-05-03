#ifndef KFTTJCONTROL
#define KFTTJCONTROL

#include <QThread>
#include "ctlcons.h"
#include "database/pgdatabase.h"
#include "bean/monthsummary.h"
#include "bean/extremum.h"

class KfttjControl : public QThread{
    Q_OBJECT

public:
    explicit KfttjControl(QObject *parent = 0);
    ~KfttjControl();

    void setSummarySql(QString);
    void setExtremumSql(QString);

    void run() Q_DECL_OVERRIDE;

signals:
    void sendMessage(const QString &message, int, int, int, int);
    void sendMessage(int, int);
    void execute(bool isEnd);
    void singleExecute(bool isEnd);
    void setProgressValue(int);

private:
    void query();
    bool isDayTime(QDateTime);
    int getHalfOrWholeDay(QDateTime);
    int getExtremumIndex(QDateTime);
    void analysis();
    QString analysisVisibility(const Monthsummary &, int, int);
    QString analysisCloud(const Monthsummary &, int, int);
    QString analysisCrossWind(const Monthsummary &, int, int);
    QString analysisHeadWind(const Monthsummary &, int, int);
    QString analysisAll(QStringList, int, int);

    QString guessVisibilityEvolution(const Extremum &, int);
    QString guessCloudEvolution(const Extremum &, int);

private:
    QList<QString> titleList;
    QString m_summarySql;
    QString m_extremumSql;

    PgDataBase *pgdb;
    QList<Monthsummary> summaryList;
    QList<Extremum> extremumList;

    int elementCount;

    //推测用
    QString resVisibilityStr1;  //提前2小时的能见度
    QString resVisibilityStr2;  //提前1小时的能见度
    QString resCloudStr1;   //提前2小时的云
    QString resCloudStr2;   //提前1小时的云
};

#endif // KFTTJCONTROL

