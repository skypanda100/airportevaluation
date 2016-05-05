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

    QHash< QString, QList<float> > getKfttjHash() const;
    QHash< QString, QStringList > getXzkfEffectHash() const;
    QHash< QString, QStringList > getBkfEffectHash() const;
    void run() Q_DECL_OVERRIDE;

signals:
    void sendMessage(const QString &message, int, int, int, int);
    void sendMessage(int, int);
    void execute(bool isEnd);
    void singleExecute(bool isEnd);
    void setProgressValue(int);

private:
    void initData();
    void query();
    bool isDayTime(QDateTime);
    int getHalfOrWholeDay(QDateTime);
    int getExtremumIndex(QDateTime);
    void analysis();
    QString analysisVisibility(const Monthsummary &, int, int);
    QString analysisCloud(const Monthsummary &, int, int);
    QString analysisCrossWind(const Monthsummary &, int, int);
    QString analysisHeadWind(const Monthsummary &, int, int);
    QString analysisAll(QDateTime, QStringList, int, int);
    void analysisDay(QDateTime, int);

    QString guessVisibilityEvolution(const Extremum &, int);
    QString guessCloudEvolution(const Extremum &, int);

private:
    //白天
    const int *jan_day;
    int jan_day_len;
    const int *feb_day;
    int feb_day_len;
    const int *mar_day;
    int mar_day_len;
    const int *apr_day;
    int apr_day_len;
    const int *may_day;
    int may_day_len;
    const int *jun_day;
    int jun_day_len;
    const int *jul_day;
    int jul_day_len;
    const int *aug_day;
    int aug_day_len;
    const int *sep_day;
    int sep_day_len;
    const int *oct_day;
    int oct_day_len;
    const int *nov_day;
    int nov_day_len;
    const int *dec_day;
    int dec_day_len;
    //半天时
    int half_day;
    //整天时
    int whole_day;

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
    //日可飞天统计
    QList<int> resAll;  //当前这天中综合要素的集合
    //影响原因
    QHash< QString, QList<QStringList> > effectHash; //QList index0:限制可飞的气象要素 index1:不可飞的气象要素
    //出图用
    QHash< QString, QList<float> > kfttjHash; //QList index0:完全可飞 index1:限制可飞 index2:不可飞
    QHash< QString, QStringList > xzkfEffectHash;
    QHash< QString, QStringList > bkfEffectHash;
};

#endif // KFTTJCONTROL

