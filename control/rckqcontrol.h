#ifndef RCKQCONTROL
#define RCKQCONTROL

#include <QThread>
#include "database/pgdatabase.h"
#define PI 3.1415926

class RckqControl : public QThread{
    Q_OBJECT

public:
    explicit RckqControl(QObject *parent = 0);
    ~RckqControl();

    void run() Q_DECL_OVERRIDE;

    void setData(QString code, QString runway, int type, int fhour, int thour, QList<QString> dateList, QList<QString> weatherList);

signals:
    void sendMessage(int row, QString weather, QString dateStr, QList<QString> valueList, QString keyStr);
    void setProgressValue(int);
    void execute(bool isEnd);

private:
    QList<QString> createDatetimeSql(QString dateStr, int fhour, int thour);

private:
    QString m_code;
    QString m_runway;
    int m_type;
    int m_fhour;
    int m_thour;
    QList<QString> m_dateList;
    QList<QString> m_weatherList;

    PgDataBase *pgdb;

};

#endif // RCKQCONTROL

