#ifndef SJDRCONTROL
#define SJDRCONTROL

#include <QThread>
#include <QFileInfo>
#include "bean/airport.h"
#include "database/asdatabase.h"
#include "database/pgdatabase.h"
#include "ctlcons.h"

class SjdrElement{
public:
    SjdrElement();
    SjdrElement(SourceType, QString, QFileInfo);
    SourceType m_sourceType;
    QString m_type;
    QFileInfo m_fileInfo;
};

class SjdrControl : public QThread{
    Q_OBJECT

public:
    explicit SjdrControl(QObject *parent = 0);

    void addTask(SjdrElement);

    void setAirport(const Airport &airport);

    void run() Q_DECL_OVERRIDE;

signals:
    void sendMessage(QStringList);

private:
    Airport m_airport;
    QVector<SjdrElement> m_tasks;
};

#endif // SJDRCONTROL

