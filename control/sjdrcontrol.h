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

    void addTask(SjdrElement);

    void setAirport(const Airport &airport);

    void setLoop(bool);

    void run() Q_DECL_OVERRIDE;

signals:
    void sendMessage(QStringList);

private:
    void controlSummary(const SjdrElement &);
    void controlExtremum(const SjdrElement &);
    void controlMainlandAutomaticWind(const SjdrElement &);
    void controlMacaoAutomaticWind(const SjdrElement &);
    void controlMainlandAutomaticTemperature(const SjdrElement &);
    void controlUnknown(const SjdrElement &);

private:
    Airport m_airport;
    QVector<SjdrElement> m_tasks;
    bool m_loop;
};

#endif // SJDRCONTROL

