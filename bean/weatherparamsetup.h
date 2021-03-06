#ifndef WEATHERPARAMSETUP
#define WEATHERPARAMSETUP

#include <QObject>

class WeatherParamSetup{
public:
    WeatherParamSetup();
    ~WeatherParamSetup();

    QString code() const;
    void setCode(const QString &code);

    QString planeName() const;
    void setPlaneName(const QString &planeName);

    int paramid() const;
    void setParamid(int paramid);

    QString limits() const;
    void setLimits(const QString &limits);

private:
    QString m_code;
    QString m_planeName;
    int m_paramid;
    QString m_limits;
};

#endif // WEATHERPARAMSETUP

