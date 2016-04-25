#ifndef AIRPORT
#define AIRPORT

#include <QObject>

class Airport{

public:
    Airport();
    ~Airport();

    QString code() const;
    void setCode(const QString &code);

    QString name() const;
    void setName(const QString &name);

    float longitude() const;
    void setLongitude(float longitude);

    float latitude() const;
    void setLatitude(float latitude);

    float altitude() const;
    void setAltitude(float altitude);

    float direction() const;
    void setDirection(float direction);

    QString type() const;
    void setType(const QString &type);

private:
    QString m_code;
    QString m_name;
    float m_longitude;
    float m_latitude;
    float m_altitude;
    float m_direction;
    QString m_type;
};

#endif // AIRPORT

