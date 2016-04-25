#ifndef QUALITYCONTROLAUTOMATIC
#define QUALITYCONTROLAUTOMATIC

#include <QObject>

class QualityControlAutomatic{

public:
    QualityControlAutomatic();
    ~QualityControlAutomatic();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    float minimum() const;
    void setMinimum(float minimum);

    float maximum() const;
    void setMaximum(float maximum);

    float suspiciousLimit() const;
    void setSuspiciousLimit(float suspiciousLimit);

    float errorLimit() const;
    void setErrorLimit(float errorLimit);

    QString unit() const;
    void setUnit(const QString &unit);

private:
    int m_id;
    QString m_name;
    float m_minimum;
    float m_maximum;
    float m_suspiciousLimit;
    float m_errorLimit;
    QString m_unit;
};

#endif // QUALITYCONTROLAUTOMATIC

