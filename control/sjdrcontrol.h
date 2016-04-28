#ifndef SJDRCONTROL
#define SJDRCONTROL

#include <QThread>
#include <QFileInfo>
#include "bean/airport.h"
#include "bean/qualitycontrolsource.h"
#include "database/asdatabase.h"
#include "database/pgdatabase.h"
#include "ctlcons.h"

class SjdrElement{
public:
    SjdrElement();
    SjdrElement(QualityControlSource, QFileInfo);
    ~SjdrElement();
    QualityControlSource m_qualityControlSource;
    QFileInfo m_fileInfo;
};

class SjdrControl : public QThread{
    Q_OBJECT

public:
    explicit SjdrControl(QObject *parent = 0);
    ~SjdrControl();

    void addTask(SjdrElement);

    void setAirport(const Airport &airport);

    void setLoop(bool);

    void run() Q_DECL_OVERRIDE;

signals:
    void sendMessage(QStringList);
    void execute(bool isEnd);
    void singleExecute(bool isEnd);

private:
    void message(const QString &info, const SjdrElement &sjdrElement, ReturnCode code = MSG_UNDEFINE);
    void controlSummary(const SjdrElement &);
    void controlExtremum(const SjdrElement &);
    void controlMainlandAutomaticWind(const SjdrElement &);
    void controlMacaoAutomaticWind(const SjdrElement &);
    void controlMainlandAutomaticTemperature(const SjdrElement &);
    void controlUnknown(const SjdrElement &);

private:
    QList<QString> monthsummary_fields;
    QList<QString> extremum_fields;
    QList<QString> mainland_automaticwind_fields;
    QList<QString> macao_automaticwind_fields;
    QList<QString> automatictemperature_fields;

    QString insertMonthsummary;
    QString insertExtremum;
    QString insertMainlandAutomaticWind;
    QString insertMacaoAutomaticWind;
    QString insertAutomaticTeperature;

    Airport m_airport;
    QVector<SjdrElement> m_tasks;
    bool m_loop;
    bool m_execute;
    PgDataBase *pgdb;
};

#endif // SJDRCONTROL

