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
    void analysis();
    QString analysisVisibility(const Monthsummary &, int, int);
    QString analysisCloud(const Monthsummary &, int, int);
    QString analysisCrossWind(const Monthsummary &, int, int);
    QString analysisHeadWind(const Monthsummary &, int, int);
    QString analysisAll(QStringList, int, int);

private:
    QList<QString> titleList;
    QString m_summarySql;
    QString m_extremumSql;

    PgDataBase *pgdb;
    QList<Monthsummary> summaryList;
    QList<Extremum> extremumList;

    int elementCount;
};

#endif // KFTTJCONTROL

