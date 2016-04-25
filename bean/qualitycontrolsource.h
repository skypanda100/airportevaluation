#ifndef QUALITYCONTROLSOURCE
#define QUALITYCONTROLSOURCE

#include <QObject>

class QualityControlSource{

public:
    QualityControlSource();
    ~QualityControlSource();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QString fields() const;
    void setFields(const QString &fields);

private:
    int m_id;
    QString m_name;
    QString m_fields;
};

#endif // QUALITYCONTROLSOURCE

