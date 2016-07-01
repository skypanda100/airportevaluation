#ifndef KFTTJCONTROL
#define KFTTJCONTROL

#include <QThread>
#include "ctlcons.h"
#include "database/pgdatabase.h"
#include "bean/monthsummary.h"
#include "bean/extremum.h"
#include "bean/weatherparam.h"
#include "bean/weatherparamsetup.h"

class KfttjControl : public QThread{
    Q_OBJECT

public:
    explicit KfttjControl(QObject *parent = 0);
    ~KfttjControl();

    void setTitleList(QStringList);
    void setSummarySql(QString);
    void setExtremumSql(QString);
    void setAirportCode(QString);
    void setMultiWeather(bool);
    void setWeatherParamList(QList<WeatherParam>);

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
    QList<QString> getDataFromJson(QString);
    bool isMatchArgs(float, QString, QString);

    /*** 风.非强风模式.矢量风.顺风 ***/
    QString analysisMultiSf(const Monthsummary &, int, int);
    /*** 风.非强风模式.矢量风.逆风 ***/
    QString analysisMultiNf(const Monthsummary &, int, int);
    /*** 风.非强风模式.矢量风.侧风 ***/
    QString analysisMultiCf(const Monthsummary &, int, int);
    /*** 风.非强风模式.标量风 ***/
    QString analysisMultiBlf(const Monthsummary &, int, int);
    /*** 温度.常规温 ***/
    QString analysisMultiCgw(const Monthsummary &, int, int);
    /*** 气压.场面气压 ***/
    QString analysisMultiCmqy(const Monthsummary &, int, int);
    /*** 气压.修正海面气压 ***/
    QString analysisMultiXzhmqy(const Monthsummary &, int, int);
    /*** 湿度.常规湿.比湿 ***/
    QString analysisMultiCgs(const Monthsummary &, int, int);
    /*** 湿度.极湿.比湿 ***/
    QString analysisMultiJs(const Monthsummary &, int, int);
    /*** 能见度.水平能见度 ***/
    QString analysisMultiSpnjd(const Monthsummary &, int, int);
    /*** 云 ***/
    QString analysisMultiYlyg(const Monthsummary &, int, int);
    /*** 降水.非强降水模式 ***/
    QString analysisMultiFqjs(const Monthsummary &, int, int);
    /*** 分析降水 ***/
    QHash< QString, int > analysisJs(QString evolution, QDateTime today, QDateTime yeasterday);
    /*** 危险天气.浓积云 ***/
    QString analysisMultiNjy(const Monthsummary &, int, int);
    /*** 危险天气.积雨云 ***/
    QString analysisMultiJyy(const Monthsummary &, int, int);
    /*** 危险天气.闪电 ***/
    QString analysisMultiSd(const Monthsummary &, int, int);
    /*** 危险天气.雷暴 ***/
    QString analysisMultiLb(const Monthsummary &, int, int);
    /*** 分析雷暴 ***/
    QHash< QString, int > analysisLb(QString evolution, QDateTime today, QDateTime yeasterday);
    /*** 危险天气.冰雹 ***/
    QString analysisMultiBb(const Monthsummary &, int, int);
    /*** 危险天气.飑线 ***/
    QString analysisMultiBx(const Monthsummary &, int, int);
    /*** 危险天气.龙卷 ***/
    QString analysisMultiLj(const Monthsummary &, int, int);
    /*** 危险天气.沙尘暴 ***/
    QString analysisMultiScb(const Monthsummary &, int, int);
    /*** 危险天气.风切变 ***/
    QString analysisMultiFqb(const Monthsummary &, int, int);
    /*** 风.强风模式.矢量风.顺风 ***/
    QString analysisSingleSf(const Monthsummary &, int, int);
    /*** 风.强风模式.矢量风.逆风 ***/
    QString analysisSingleNf(const Monthsummary &, int, int);
    /*** 风.强风模式.矢量风.侧风 ***/
    QString analysisSingleCf(const Monthsummary &, int, int);
    /*** 风.强风模式.标量风 ***/
    QString analysisSingleBlf(const Monthsummary &, int, int);
    /*** 温度.极高温 ***/
    QString analysisSingleJgw(const Monthsummary &, int, int);
    /*** 温度.极低温 ***/
    QString analysisSingleJdw(const Monthsummary &, int, int);
    /*** 降水.强降水 ***/
    QString analysisSingleQjs(const Monthsummary &, int, int);

    /////////
    QString analysisAll(QDateTime, QStringList, int, int);
    void analysisDay(QDateTime, int);

    QString guessVisibilityEvolution(const Extremum &, int, QList<QString>);
    QString guessCloudEvolution(const Extremum &, int);

private:
    //机场code
    QString m_airportCode;
    //机场跑道方向
    float m_runwayDirection;
    //是否为多要素
    bool m_isMultiWeather;
    //气象要素
    QList<WeatherParam> m_wpList;
    //气象要素阀值
    QList<WeatherParamSetup> weatherParamSetupList;
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

    QList<QString> m_titleList;
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

