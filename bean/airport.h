#ifndef AIRPORT
#define AIRPORT

#include <QObject>

class Airport{

public:
    Airport();
    ~Airport();

    QString code() const;
    void setCode(const QString &code);

    int planeCode() const;
    void setPlaneCode(int planeCode);

    QString name() const;
    void setName(const QString &name);

    QString planeName() const;
    void setPlaneName(const QString &planeName);

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
    int m_planeCode;
    QString m_name;
    QString m_planeName;
    float m_longitude;
    float m_latitude;
    float m_altitude;
    float m_direction;
    QString m_type;
};

#endif // AIRPORT

