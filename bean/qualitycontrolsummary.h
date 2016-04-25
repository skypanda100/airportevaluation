#ifndef QUALITYCONTROLSUMMARY
#define QUALITYCONTROLSUMMARY

#include <QObject>

class QualityControlSummary{

public:
    QualityControlSummary();
    ~QualityControlSummary();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    float minimum() const;
    void setMinimum(float minimum);

    float maximum() const;
    void setMaximum(float maximum);

    QString unit() const;
    void setUnit(const QString &unit);

private:
    int m_id;
    QString m_name;
    float m_minimum;
    float m_maximum;
    QString m_unit;
};

#endif // QUALITYCONTROLSUMMARY

